/*
 * @Author: cpu_code
 * @Date: 2020-05-06 19:37:41
 * @LastEditTime: 2020-05-06 20:39:28
 * @FilePath: \linux_network\UNIX_net\tcp\tcp_serv.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_echo(int sockfd);

/**
 * @function: TCP回射服务器
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 * 配套使用
 * FilePath: \linux_network\UNIX_net\tcp\tcp_cli.c
 */
int main(int argc, char **argv)
{
    int					listenfd, connfd;
    pid_t				childpid;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    //通配地址
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //众所周知端口
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for ( ; ; )
    {
        clilen = sizeof(cliaddr);
        //阻塞于accept调用， 等待客户连接的完成
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

        //为每个客户派生一个处理它们的子进程
        if( (childpid = Fork()) == 0)
        {
            Close(listenfd);	/* close listening socket */
            str_echo(connfd);	/* process the request */
            exit(0);
        }
        Close(connfd);			/* parent closes connected socket */
    }
}

void str_echo(int sockfd)
{
    ssize_t		n;
    char		buf[MAXLINE];

again:
    //接收到客户的FIN将导致服务器子进程的read函数返回0
    //从套接字读入数据
    while( (n = read(sockfd, buf, MAXLINE)) > 0)
    {
        //把其中内容回射给客户
        Writen(sockfd, buf, n);
    }
    if(n < 0 && errno == EINTR)
    {
        goto again;
    }
    else if (n < 0)
    {
        err_sys("str_echo: read error");
    }
}