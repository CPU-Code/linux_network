/*
 * @Author: cpu_code
 * @Date: 2020-05-05 20:38:21
 * @LastEditTime: 2020-05-05 20:57:40
 * @FilePath: \linux_network\UNIX_net\lib\readlinel.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include readline */
#include	"unp.h"

/* PAINFULLY SLOW VERSION -- example only */
ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t	n, rc;
    char	c, *ptr;

    ptr = vptr;

    for(n = 1; n < maxlen; n++)
    {
again:
        if((rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if(c == '\n')
            {
                break;  /* newline is stored, like fgets() */
            }
        }
        else if(rc == 0)
        {
            *ptr = 0;
            return(n - 1);	/* EOF, n - 1 bytes were read */
        }
        else
        {
            if(errno == EINTR)
            {
                goto again;
            }
            return(-1);		/* error, errno set by read() */
        }
    }
    *ptr = 0;	/* null terminate like fgets() */
	return(n);
}

ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t n;

    if((n = readline(fd, ptr, maxlen)) < 0)
    {
        err_sys("readline error");
    }
    return(n);
}