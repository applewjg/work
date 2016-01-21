#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define MAXSIZE     1024
#define IPADDRESS   "10.95.44.13"
#define SERV_PORT   12340
#define FDSIZE        1024
#define EPOLLEVENTS 20

static void handle_connection(int sockfd);
static int create_connection(int epollfd);
static void handle_events(int epollfd,struct epoll_event *events,int num,char *buf);
static int do_read(int fd,char *buf);
static int do_write(int fd,char *buf);
static void add_event(int epollfd,int fd,int state);
static void delete_event(int epollfd,int fd,int state);
static void modify_event(int epollfd,int fd,int state);


typedef struct {
    unsigned int len:16;    //包长度, 低16位, 共24位
    unsigned int version:4;  //版本
    unsigned int compress:4; //保留
    unsigned int exlen:8;   //包长度, 高8位
} idl_res_head_t;



int main(int argc, char *argv[])
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE];
    int ret;
    epollfd = epoll_create(FDSIZE);
    add_event(epollfd, STDIN_FILENO,EPOLLIN);
    while(true) {
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, 2);
        handle_events(epollfd,events,ret,buf);
    }
    close(epollfd);
    return 0;
}

static int create_connection() 
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
    struct timeval timeo = {3,0};
    socklen_t len = sizeof(timeo);
    int flags = -1;
    int n = 0;
    errno = 0;
    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        printf("fcntl() return error flags[%d]\n", flags);
        return -1;
    }
    n = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    if (n < 0)
    {
        printf("fcntl() return error ret[%d]\n", flags);
        return -1;
    } 
    n = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    while (  n < 0 && errno == EINTR )
    {
        printf("[EINTR]:\n");
        n = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    }

    if ( n < 0 && errno != EINPROGRESS )
    {
        printf("connection is error: !EINPROGRESS\n");
        return -1;
    }
    if ( n == 0 )
    {
        printf("imediately\n");
    }
    
    if ( sockfd <= 0 ) {
        printf ("create connection error:\n");
        return -1;
    }

    printf("create a new connection : %d\n", sockfd);
    return sockfd;
}

static void handle_events(int epollfd, struct epoll_event *events, int num, char *buf)
{
    int fd;
    for (int i = 0; i < num; ++i)
    {
        fd = events[i].data.fd;
        if ( events[i].events & EPOLLHUP) 
        {
            delete_event(epollfd, fd, EPOLLOUT);
            close(fd);
            printf ("EVENT: EPOLLHUP: %d\n",fd);
            continue;
        }
        else if (events[i].events & EPOLLERR)
        {
            delete_event(epollfd, fd, EPOLLOUT);
            close(fd);
            printf ("EVENT: ERROR: %d\n",fd);
            continue;
        }
        else if (events[i].events & EPOLLIN)
        {
            printf("EVENT: EPOLLIN\n");
            if ( fd == STDIN_FILENO )
            {
                printf("read from stdin\n");
                do_read(fd, buf);
                int sockfd = create_connection();
                if (sockfd <=0) {
                    continue;
                }
                add_event(epollfd, sockfd, EPOLLOUT);
            } 
            else 
            {
                printf("read from sockfd: %d\n", fd);
                //read from sockfd, and write the result to thre stdout
                do_read(fd, buf);
                delete_event(epollfd, fd, EPOLLIN);
                close(fd);
                add_event(epollfd, STDOUT_FILENO, EPOLLOUT); 
            }
        }
        else if ( events[i].events & EPOLLOUT )
        {
            printf("EVENT: EPOLLOUT\n");
            if ( fd == STDOUT_FILENO )
            {
                printf("write stdout\n");
                do_write( fd, buf);
                delete_event(epollfd, fd, EPOLLOUT);
            }
            else
            {
                printf("write sockfd: %d\n", fd);
                do_write(fd, buf);
                modify_event(epollfd, fd, EPOLLIN);
                printf("write sockfd: %d END.\n", fd);
            }
        }
    }
}


static int do_read(int fd, char* buf)
{
    int nread = 0;
    if ( fd == STDIN_FILENO )
    {
        nread = read(fd, buf, MAXSIZE);
    }
    else
    {
        nread = recv(fd,buf,sizeof(idl_res_head_t),MSG_WAITALL);
        idl_res_head_t *status = (idl_res_head_t*)buf;
        int len = -1;
        len = status->len + (status->exlen<<16);
        printf("the response data len: %d\n", len);
        memset(buf,0,MAXSIZE);
        nread = recv(fd,buf,len,MSG_WAITALL);
    }    
    printf("read from fd(%d): %s\n", fd, buf);
    if ( nread == -1 )
    {
        perror("read error:");
        close(fd);
    }
    else if ( nread == 0 )
    {
        fprintf(stderr, "server close.\n");
        close(fd);
    }
    return nread;
}

static int do_write(int fd, char* buf) 
{
   int nwrite = 0;
   if (fd == STDOUT_FILENO)
   {
        nwrite =  write(fd, buf, strlen(buf));
   }
   else
   {
       char res_data[1024]={0};
       *(int*)res_data = strlen(buf);
       sprintf(res_data+sizeof(idl_res_head_t),"%s",buf);
       int len = sizeof(idl_res_head_t) + strlen(buf);
       //nwrite = write(fd,buf,strlen(buf));
       printf("res_data: %s\n", res_data + sizeof(idl_res_head_t));
       nwrite = send(fd,res_data,len,0);
   }
   if ( nwrite == -1 )
   {
       perror("write error:");
       close(fd);
   }
   memset(buf, 0, MAXSIZE);
   return nwrite;
}

static void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

static void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

static void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}
