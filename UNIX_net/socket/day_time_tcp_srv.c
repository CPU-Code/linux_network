/*
 * @Author: cpu_code
 * @Date: 2020-05-06 16:39:38
 * @LastEditTime: 2020-05-06 19:12:25
 * @FilePath: \linux_network\UNIX_net\socket\day_time_tcp_srv.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"
#include	<time.h>

/**
 * @function:  显示客户IP地址和端口号的时间
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 *  配套使用
 * FilePath: \linux_network\UNIX_net\socket\day_time_tcp_cli.c
 *  sudo
 * connection from 192.168.43.243, port 43488
 * connection from 127.0.0.1, port 50336
 */
int main(int argc, char **argv)
{
    int					listenfd, connfd;
    socklen_t			len;
    struct sockaddr_in	servaddr, cliaddr;
    char				buff[MAXLINE];
    time_t				ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(13);	/* daytime server */

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    for ( ; ; )
    {
        len = sizeof(cliaddr);

        connfd = Accept(listenfd, (SA *) &cliaddr, &len);

        printf("connection from %s, port %d\n",
			   Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        Write(connfd, buff, strlen(buff));
        Close(connfd);
    }
}