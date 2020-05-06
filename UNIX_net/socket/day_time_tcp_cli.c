/*
 * @Author: cpu_code
 * @Date: 2020-05-06 17:18:26
 * @LastEditTime: 2020-05-06 18:01:57
 * @FilePath: \linux_network\UNIX_net\socket\day_time_tcp_cli.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */

#include	"unp.h"

/**
 * @function: 
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 * 配套使用
 *  FilePath: \linux_network\UNIX_net\socket\day_time_tcp_srv.c
 * ./a.out 192.168.43.243
 *    Wed May  6 02:24:39 2020
 * ./a.out 127.0.0.1
 *    Wed May  6 02:25:00 2020
 */
int main(int argc, char **argv)
{
    int					sockfd, n;
    char				recvline[MAXLINE + 1];
    struct sockaddr_in	servaddr;

    if (argc != 2)
    {
        err_quit("usage: a.out <IPaddress>");
    }

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(13);	/* daytime server */
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
    {
        err_sys("connect error");
    }

    while( (n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;	/* null terminate */
        
        if(fputs(recvline, stdout) == EOF)
        {
            err_sys("fputs error");
        }
    }

    if(n < 0)
    {
        err_sys("read error");
    }
    exit(0);
}