/*
 * @Author: cpu_code
 * @Date: 2020-05-07 17:17:25
 * @LastEditTime: 2020-05-07 17:40:49
 * @FilePath: \linux_network\UNIX_net\I_O\str_cil_select.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_cli(FILE *fp, int sockfd)
{
    int			maxfdp1;
    fd_set		rset;
    char		sendline[MAXLINE], recvline[MAXLINE];

    //初始化
    FD_ZERO(&rset);

    for( ; ; )
    {
        //fileno: 把标准I/O文件指针转换为对应的描述符
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        //计算出两个描述符中的较大值
        maxfdp1 = max(fileno(fp), sockfd) + 1;

        //处理可读套接字, 写集合指针和异常集合指针: 空指针, 时间限制:空指针
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        /* socket is readable */
        if(FD_ISSET(sockfd, &rset))
        {
            //读入回射文本行
            if(Readline(sockfd, recvline, MAXLINE) == 0)
            {
                err_quit("str_cli: server terminated prematurely");
            }
            //输出它
            Fputs(recvline, stdout);
        }

        /* input is readable */
        if(FD_ISSET(fileno(fp), &rset))
        {
            //读入一行文本
            if(Fgets(sendline, MAXLINE, fp) == NULL)
            {
                return;		/* all done */
            }
            //把它写到套接字
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}