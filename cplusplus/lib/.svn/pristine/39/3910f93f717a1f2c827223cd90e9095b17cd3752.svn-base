#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define IPADDRESS   "10.95.44.13"
#define PORT       12340
#define MAXSIZE     1024
#define LISTENQ     5
#define FDSIZE      1000
#define EPOLLEVENTS 100


//函数声明
//创建套接字并进行绑定
static int socket_bind(const char* ip,int port);
//IO多路复用epoll
static void do_epoll(int listenfd);
//事件处理函数
static void
handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf);
//处理接收到的连接
static void handle_accpet(int epollfd,int listenfd);
//读处理
static void do_read(int epollfd,int fd,char *buf);
//写处理
static void do_write(int epollfd,int fd,char *buf);
//添加事件
static void add_event(int epollfd,int fd,int state);
//修改事件
static void modify_event(int epollfd,int fd,int state);
//删除事件
static void delete_event(int epollfd,int fd,int state);

typedef struct {
    unsigned int len:16;    //包长度, 低16位, 共24位
    unsigned int version:4;  //版本
    unsigned int compress:4; //保留
    unsigned int exlen:8;   //包长度, 高8位
} idl_res_head_t;

static char confpath[256] = "./conf/";  // 配置目录
static char confname[128] = "sched.conf";  // 配置文件

int main(int argc,char *argv[])
{
    char arg;
    //get options which user input
    while((arg = getopt(argc,argv,"d:f:")) != -1) {
        switch (arg) {
            case 'd':
                strncpy(confpath, optarg, sizeof(confpath));
                break;
            case 'f':
                strncpy(confname, optarg, sizeof(confname));
                break;
            default :
                printf("hello world~");
                return -1;
        }
    }
    
    printf("confdir:%s\n",confpath);
    printf("confname:%s\n",confname);
    int  listenfd;
    listenfd = socket_bind(IPADDRESS,PORT);
    listen(listenfd,LISTENQ);
    do_epoll(listenfd);
    return 0;
}

static int socket_bind(const char* ip,int port)
{
    int  listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd == -1)
    {
        perror("socket error:");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}

static void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    //创建一个描述符
    epollfd = epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(epollfd,listenfd,EPOLLIN);
    int epoll_tm = 2;
    for ( ; ; )
    {
        //获取已经准备好的描述符事件
        ret = epoll_wait(epollfd,events,EPOLLEVENTS,epoll_tm);
        handle_events(epollfd,events,ret,listenfd,buf);
    }
    close(epollfd);
}

static void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf)
{
    int i;
    int fd;
    //进行选好遍历
    for (i = 0;i < num;i++)
    {
        fd = events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if ((fd == listenfd) &&(events[i].events & EPOLLIN))
            handle_accpet(epollfd,listenfd);
        else if (events[i].events & EPOLLIN)
            do_read(epollfd,fd,buf);
        else if (events[i].events & EPOLLOUT)
            do_write(epollfd,fd,buf);
    }
}
static void handle_accpet(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen = sizeof(struct sockaddr_in);
    clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if (clifd == -1)
        perror("accpet error:");
    else
    {
        //cliaddr.sin_addr.s_addr=inet_addr("192.168.1.1");
        /*char ipstr[16];
        inet_ntop(AF_INET,&(((struct sockaddr_in*)&cliaddr)->sin_addr),ipstr, 16);
        printf("accept a new client: %s:%d\n",ipstr,cliaddr.sin_port);
        */
        printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);
    }
}

static void do_read(int epollfd,int fd,char *buf)
{
    int nread;
    //nread = read(fd,buf,MAXSIZE);
    nread = recv(fd,buf,sizeof(idl_res_head_t),MSG_WAITALL);
    idl_res_head_t *status = (idl_res_head_t*)buf;
    int len = -1;
    len = status->len + (status->exlen<<16);
    nread = recv(fd,(unsigned char*)buf+sizeof(idl_res_head_t),len,MSG_WAITALL);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else if (nread == 0)
    {
        fprintf(stderr,"client close.\n");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("read message is : %s",(unsigned char*)buf+sizeof(idl_res_head_t));
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd,fd,EPOLLOUT);
    }
}

static void do_write(int epollfd,int fd,char *buf)
{
    int nwrite;
    char res_data[1024]={0};
    char * response = "hello from server.";
    *(int*)res_data = strlen(response);
    sprintf(res_data+sizeof(idl_res_head_t),"%s",response);
    printf("the message is : %s\n", res_data+sizeof(idl_res_head_t));
    int len = strlen(response) + sizeof(idl_res_head_t);
    
    //nwrite = write(fd,buf,strlen(buf));
    nwrite = send(fd,res_data,len,0);
    if (nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLOUT);
    }
    else
        modify_event(epollfd,fd,EPOLLIN);
    memset(buf,0,MAXSIZE);
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
