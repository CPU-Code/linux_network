/*
 * @Author: cpu_code
 * @Date: 2020-05-05 20:14:04
 * @LastEditTime: 2020-05-06 20:41:35
 * @FilePath: \linux_network\UNIX_net\lib\writen.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include writen */
#include	"unp.h"

void Writen(int fd, void *ptr, size_t nbytes);
ssize_t writen(int fd, const void *vptr, size_t n);

/* Write "n" bytes to a descriptor. */
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;		/* and call write() again */
            }
            else
            {
                return(-1);			/* error */
            }
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
    {
		err_sys("writen error");
    }

}
