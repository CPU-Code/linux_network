/*
 * @Author: cpu_code
 * @Date: 2020-05-06 19:53:46
 * @LastEditTime: 2020-05-06 20:37:14
 * @FilePath: \linux_network\UNIX_net\tcp\tcp_cli.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

/**
 * @function: TCP回射客户
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 * 配套使用
 * FilePath: \linux_network\UNIX_net\tcp\tcp_serv.c
 */
int main(int argc, char **argv)
{
    int					sockfd;
    struct sockaddr_in	servaddr;

    if(argc != 2)
    {
        err_quit("usage: tcpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //服务器的端口号
    servaddr.sin_port = htons(SERV_PORT);
    //命令行参数取得服务器的IP地址
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    //建立与服务器的连接
    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_cli(stdin, sockfd);		/* do it all */

    exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
    char	sendline[MAXLINE], recvline[MAXLINE];

    //读入一行文本
    while(Fgets(sendline, MAXLINE, fp) != NULL)
    {
        //该行发送给服务器
        Writen(sockfd, sendline, strlen(sendline));

        //服务器读入回射行
        if(Readline(sockfd, recvline, MAXLINE) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }
        //它写到标准输出
        Fputs(recvline, stdout);
    }
}