/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:57:13
 * @FilePath: \linux_network\introduction\tcp\getaddrinfo.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{
    struct addrinfo hints, *res = NULL;
    int rc;

    memset(&hints, 0, sizeof(hints));

    /*设置 addrinfo 结构体中各参数*/
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    /*调用 getaddinfo 函数*/
    rc = getaddrinfo("127.0.0.1", "123", &hints, &res);
    if(rc != 0)
    {
        perror("getaddrinfo");
        exit(1);
    }
    else
    {
        printf("getaddrinfo success\n");
    }
}