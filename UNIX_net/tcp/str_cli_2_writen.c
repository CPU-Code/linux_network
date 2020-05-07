/*
 * @Author: cpu_code
 * @Date: 2020-05-07 12:00:12
 * @LastEditTime: 2020-05-07 14:00:35
 * @FilePath: \linux_network\UNIX_net\tcp\str_cli_2_writen.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_cli(FILE *fp, int sockfd)
{
    char	sendline[MAXLINE], recvline[MAXLINE];

    while(Fgets(sendline, MAXLINE, fp) != NULL)
    {
        //文本行数据的第一个字节写入套接字
        Writen(sockfd, sendline, 1);
        //暂停一秒钟
        sleep(1);
        //同一文本行中剩余字节写入套接字
        Writen(sockfd, sendline + 1, strlen(sendline) - 1);

        if(Readline(sockfd, recvline, MAXLINE) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }
        Fputs(recvline, stdout);
    }
}