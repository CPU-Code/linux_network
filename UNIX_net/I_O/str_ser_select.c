/*
 * @Author: cpu_code
 * @Date: 2020-05-08 21:57:56
 * @LastEditTime: 2020-05-09 15:14:25
 * @FilePath: \linux_network\UNIX_net\I_O\str_ser_select.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include fig01 */
#include	"unp.h"
/**
 * @function: 单进程和select的TCP服务器程序： 循环
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
int main(int argc, char **argv)
{
    int					i, maxi;
    int                 maxfd, listenfd, connfd, sockfd;
    int					nready, client[FD_SETSIZE];
    ssize_t				n;
    char				buf[MAXLINE];
    fd_set				rset, allset;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    maxfd = listenfd;			/* 初始化 */
    maxi  = -1;					/* 索引到client[]数组 */

    for(i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;			/* -1表示可用条目 */
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    /* end fig01 */

    /* include fig02 */
    for( ; ; )
    {
        rset = allset;		/* structure assignment */

        //等待某个事件发生
        nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);

        /* 新客户端连接 */
        if(FD_ISSET(listenfd, &rset))
        {
            clilen = sizeof(cliaddr);

            //新的连接
            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

#ifdef	NOTDEF
            printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

            for(i = 0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    //第一个未用项记录这个已连接描述符
                    client[i] = connfd;	/* 保存描述符 */
                    break;
                }
            }

            if(i == FD_SETSIZE)
            {
                err_quit("too many clients");
            }

            FD_SET(connfd, &allset);	/* 添加要设置的新描述符 */

            if(connfd > maxfd)
            {
                maxfd = connfd;			/* for select */
            }

            if(i > maxi)
            {
                maxi = i;				/* client[]数组中的最大索引 */
            }

            if(--nready <= 0)
            {
                //避免进入下一个for循环
                continue;				/* 没有可读的描述符 */
            }
        }

        /* 检查所有客户端的数据 */
        for(i = 0; i <= maxi; i++)
        {
            if( (sockfd = client[i]) < 0)
            {
                continue;
            }
            
            if(FD_ISSET(sockfd, &rset))
            {
                //读入一行文本
                if( (n = Read(sockfd, buf, MAXLINE)) == 0)
                {
                    /*客户端关闭的连接 */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                {
                    //回射
                    Writen(sockfd, buf, n);
                }

                if(--nready <= 0)
                {
                    break;				/*没有可读的描述符 */
                }
                
            }
        }
    }
}