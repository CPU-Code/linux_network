/*
 * @Author: cpu_code
 * @Date: 2020-05-07 10:38:14
 * @LastEditTime: 2020-05-07 11:40:04
 * @FilePath: \linux_network\UNIX_net\tcp\tcp_serv_sig_waitpid.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

void str_echo(int sockfd);
void sig_chld(int signo);

int main(int argc, char **argv)
{
	int					listenfd, connfd;
    pid_t				childpid;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;
    void				sig_chld(int);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_chld);	/* must call waitpid() */

    for ( ; ; )
    {
        clilen = sizeof(cliaddr);

        if( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0)
        {
            if(errno == EINTR)
            {
				continue;		/* back to for() */
            }
            else
            {
                err_sys("accept error");
            }
        }
        /* child process */
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
    else if(n < 0)
    {
        err_sys("str_echo: read error");
    }
}

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

    //等待任何子进程,= wait()
	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
		printf("child %d terminated\n", pid);
    }
	return;
}