#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <iconv.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

inline void string_copy(char* dst, size_t dst_len, const char* src, size_t src_len)
{
    size_t n = dst_len - 1;
    if (n > src_len) {
        n = src_len;
    }
    memcpy(dst, src, n);
    dst[n] = 0;
}

inline void string_copy(char* dst, size_t dst_len, const char* src)
{
    // 耗时和strncpy基本相等，是snprintf的一半
    string_copy(dst, dst_len, src, strlen(src));
}

int get_public_ip(unsigned int len, char* ip)
{
    if (NULL == ip) {
        return -1;
    }

    int fd = 0;
    struct ifreq ifq[16];
    struct ifconf ifc;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0) {
        printf("%s socket failed\n", __PRETTY_FUNCTION__);
        return -1;
    }

    //获取接口清单
    ifc.ifc_len = sizeof(ifq);
    ifc.ifc_buf = (caddr_t)ifq;

    if (0 != ioctl(fd, SIOCGIFCONF, reinterpret_cast<char*>(&ifc))) {
        printf("%s ioctl failed\n", __PRETTY_FUNCTION__);
        close(fd);
        return -1;
    }

    char* temp_ip = NULL;
    int num = ifc.ifc_len / sizeof(struct ifreq);

    for (int i = 0; i < num; ++i) {
        if (0 == ioctl(fd, SIOCGIFADDR, reinterpret_cast<char*>(&ifq[i]))) {
            temp_ip = inet_ntoa((reinterpret_cast<struct sockaddr_in*>
                                 (&ifq[i].ifr_addr))->sin_addr);

            if (NULL != temp_ip && 0 != strcmp(temp_ip, "127.0.0.1")) {
                if (strlen(temp_ip) >= len) {
                    printf("%s len is too short\n", __PRETTY_FUNCTION__);
                } else {
                    string_copy(ip, len, temp_ip);
                    close(fd);
                    printf("public ip: %s\n", ip);
                    return -1;
                }
            }
        } else {
            printf("%s\n", strerror(errno));
        }
    }

    printf("%s can not find public ip\n", __PRETTY_FUNCTION__);
    close(fd);
    return -1;
}

int main() {
    char public_ip[16] = {0};
    int ret = get_public_ip(16, public_ip);
    return 0;
}
