/*
 * @Author: cpu_code
 * @Date: 2020-05-06 22:15:24
 * @LastEditTime: 2020-05-06 22:22:31
 * @FilePath: \linux_network\UNIX_net\tcp\tcp_5_cli.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_cli(FILE *fp, int sockfd);

/**
 * @function: 与服务器建立了5个连接的TCP客户
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
int main(int argc, char **argv)
{
    int					i, sockfd[5];
    struct sockaddr_in	servaddr;

    if(argc != 2)
    {
        err_quit("usage: tcpcli <IPaddress>");
    }
    for(i = 0; i < 5; i++)
    {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port   = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

        Connect(sockfd[i], (SA *) &servaddr, sizeof(servaddr));
    }
    str_cli(stdin, sockfd[0]);		/* do it all */
    exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL) 
    {
		Writen(sockfd, sendline, strlen(sendline));

		if (Readline(sockfd, recvline, MAXLINE) == 0)
        {
			err_quit("str_cli: server terminated prematurely");
        }
		Fputs(recvline, stdout);
	}
}