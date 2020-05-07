/*
 * @Author: cpu_code
 * @Date: 2020-05-06 22:20:24
 * @LastEditTime: 2020-05-06 22:20:59
 * @FilePath: \linux_network\UNIX_net\tcp\str_cli.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

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

