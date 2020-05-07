/*
 * @Author: cpu_code
 * @Date: 2020-05-07 10:55:50
 * @LastEditTime: 2020-05-07 11:58:04
 * @FilePath: \linux_network\UNIX_net\tcp\str_echo.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_echo(int sockfd)
{
    ssize_t		n;
    char		buf[MAXLINE];

again:
    //接收到客户的FIN将导致服务器子进程的read函数返回0
    //从套接字读入数据
    while( (n = read(sockfd, buf, MAXLINE)) > 0)
    {
        //把其中内容回射给客户
        Writen(sockfd, buf, n);
    }
    
    if(n < 0 && errno == EINTR)
    {
        goto again;
    }
    else if(n < 0)
    {
        err_sys("str_echo: read error");
    }
}