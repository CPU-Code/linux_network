/*
 * @Author: cpu_code
 * @Date: 2020-05-08 09:47:21
 * @LastEditTime: 2020-05-08 10:03:44
 * @FilePath: \linux_network\UNIX_net\I_O\str_cli_select_EOF.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_cli(FILE *fp, int sockfd)
{
    int			maxfdp1, stdineof;
    fd_set		rset;
    char		buf[MAXLINE];
    int		    n;

    //初始化为0
    stdineof = 0;
    FD_ZERO(&rset);

    for( ; ; )
    {
        //标准输入的可读性
        if(stdineof == 0)
        {
            FD_SET(fileno(fp), &rset);
        }

        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;

        Select(maxfdp1, &rset, NULL, NULL, NULL);

        /* socket is readable */
        if(FD_ISSET(sockfd, &rset))
        {
            if( (n = Read(sockfd, buf, MAXLINE)) == 0)
            {
                
                if(stdineof == 1)
                {
                    //套接字上读到EOF
                    return;		/* normal termination */
                }
                else
                {
                    err_quit("str_cli: server terminated prematurely");
                }
            }
            Write(fileno(stdout), buf, n);
        }

        /* input is readable */
        if(FD_ISSET(fileno(fp), &rset))
        {
            if( (n = Read(fileno(fp), buf, MAXLINE)) == 0)
            {
                //标准输入上碰到EOF
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);	/* send FIN */
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Writen(sockfd, buf, n);
        }
    }
}
