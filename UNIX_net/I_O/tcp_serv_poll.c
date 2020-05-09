/*
 * @Author: cpu_code
 * @Date: 2020-05-09 17:45:39
 * @LastEditTime: 2020-05-09 18:13:30
 * @FilePath: \linux_network\UNIX_net\I_O\tcp_serv_poll.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include fig01 */
#include	"unp.h"
#include	<limits.h>		/* for OPEN_MAX */

/**
 * @function: poll函数的TCP服务器
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
int main(int argc, char **argv)
{
    int					i, maxi, listenfd, connfd, sockfd;
    int					nready;
    ssize_t				n;
    char				buf[MAXLINE];
    socklen_t			clilen;
    struct pollfd		client[FOPEN_MAX];
    struct sockaddr_in	cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    //第一项用于监听套接字
    client[0].fd = listenfd;
    //新的连接准备好被接受时poll将通知
    client[0].events = POLLRDNORM;

    for(i = 1; i < FOPEN_MAX; i++)
    {
        client[i].fd = -1;		/* -1 indicates available entry */
    }

    //client数组当前正在使用的最大下标值
    maxi = 0;					/* max index into client[] array */

    for( ; ; )
    {
        //等待新的连接或 现有连接上有数据可读
        nready = Poll(client, maxi + 1, INFTIM);

        /* new client connection */
        if(client[0].revents & POLLRDNORM)
        {
            clilen = sizeof(cliaddr);

            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
            printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif
            for(i = 1; i < FOPEN_MAX; i++)
            {
                //查找第一个描述符成员为负的可用项
                if(client[i].fd < 0)
                {
                    client[i].fd = connfd;	/* save descriptor */
                    break;
                }
            }

            if(i == FOPEN_MAX)
            {
                err_quit("too many clients");
            }

            //设置POLLRDNORM事件
            client[i].events = POLLRDNORM;
            if(i > maxi)
            {
                maxi = i;				/* max index in client[] array */
            }

            if(--nready <= 0)
            {
                continue;				/* no more readable descriptors */
            }
        }

        /* check all clients for data */
        for(i = 1; i <= maxi; i++)
        {
            if( (sockfd = client[i].fd) < 0)
            {
                continue;
            }

            //检查的两个返回事件是POLLRDNORM和POLLERR
            if(client[i].revents & (POLLRDNORM | POLLERR))
            {
                if( (n = read(sockfd, buf, MAXLINE)) < 0)
                {
                    if(errno == ECONNRESET)
                    {
                        /*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
                        Close(sockfd);
                        //客户终止,fd成员置为-1
                        client[i].fd = -1;
                    }
                    else
                    {
                        err_sys("read error");
                    }
                }
                else if(n == 0)
                {
                    /*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
                    Close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                    Writen(sockfd, buf, n);
                }
                
                if(--nready <= 0)
                {
                    break;				/* no more readable descriptors */
                }
            }
        }
    }
}