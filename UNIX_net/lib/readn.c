/*
 * @Author: cpu_code
 * @Date: 2020-05-05 18:44:23
 * @LastEditTime: 2020-05-05 20:09:24
 * @FilePath: \linux_network\UNIX_net\lib\readn.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include readn */
#include	"unp.h"

ssize_t readn(int fd, void *vptr, size_t n);
ssize_t Readn(int fd, void *ptr, size_t nbytes);

/* Read "n" bytes from a descriptor. */
ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t	nleft;
    ssize_t	nread;
    char	*ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if((nread = read(fd, ptr, nleft)) < 0)
        {
            if(errno == EINTR)
            {
                nread = 0;		/* and call read() again */
            }
            else
            {
                return(-1);
            }
        }
        else if(nread == 0)
        {
            break;				/* EOF */
        }
        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);		/* return >= 0 */
}

ssize_t Readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if((n = readn(fd, ptr, nbytes)) < 0)
    {
        // err_sys("readn error");
    }
    return(n);
}