/*
 * @Author: cpu_code
 * @Date: 2020-05-06 09:37:00
 * @LastEditTime: 2020-05-06 16:31:11
 * @FilePath: \linux_network\UNIX_net\lib\wrapsock.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 * Socket wrapper functions.
 * These could all go into separate files, so only the ones needed cause
 * the corresponding function to be added to the executable.  If sockets
 * are a library (SVR4) this might make a difference (?), but if sockets
 * are in the kernel (BSD) it doesn't matter.
 *
 * These wrapper functions also use the same prototypes as POSIX.1g,
 * which might differ from many implementations (i.e., POSIX.1g specifies
 * the fourth argument to getsockopt() as "void *", not "char *").
 *
 * If your system's headers are not correct [i.e., the Solaris 2.5
 * <sys/socket.h> omits the "const" from the second argument to both
 * bind() and connect()], you'll get warnings of the form:
 *warning: passing arg 2 of `bind' discards `const' from pointer target type
 *warning: passing arg 2 of `connect' discards `const' from pointer target type
 */
#include	"unp.h"

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);


/**
 * @function: 阻塞进程，直到有一个客户连接建立后返回
 * @parameter: 
 *      fd: 监听套接字
 *      sa: 指定客户端的地址
 *      salenptr:sa结构的大小
 * @return: 
 *      success:新套接字
 *      error: -1
 * @note: 
 */
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int		n;

again:
    if( (n = accept(fd, sa, salenptr)) < 0)
    {
#ifdef	EPROTO
        if(errno == EPROTO || errno == ECONNABORTED)
#else
        if(errno == ECONNABORTED)
#endif
        {
            goto again;
        }
        else
        {
            err_sys("accept error");
        }
    }
    return(n);
}

/**
 * @function: 将套接字与特定的IP地址和端口绑定起来
 * @parameter: 
 *      fd: 文件描述符
 *      sa: 结构体变量的指针
 *      salen:  sa 变量的大小
 * @return: 
 *      success: 0
 *      error: -1
 * @note: 
 */
void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
    {
		err_sys("bind error");
    }
}

/**
 * @function: 将文件描述符fd的套接字连接到sa指定的地址
 * @parameter: 
 *      fd: 文件描述符
 *      sa: SOCK_DGRAM, SOCK_STREAM, SOCK_SEQPACKET
 *      salen: sa的长度
 * @return: 
 *      success: 0
 *      error: -1
 * @note: 
 */
void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if(connect(fd, sa, salen) < 0)
    {
        err_sys("connect error");
    }
}

/**
 * @function: 获取连接的对等套接字的名称
 * @parameter: 
 * @return: 
 *      success: 0
 *      error: 1
 * @note: 
 */
void Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if(getpeername(fd, sa, salenptr) < 0)
    {
        err_sys("getpeername error");
    }
}

/**
 * @function: 获取套接字名称
 * @parameter: 
 * @return: 
 *     success: 0
 *     error: -1
 * @note: 
 */
void Getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if(getsockname(fd, sa, salenptr) < 0)
    {
        err_sys("getsockname error");
    }
}

/**
 * @function: 在套接字上获取选项
 * @parameter: 
 * @return: 
 *     success: 0
 *     error: -1
 * @note: 
 */
void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
    if(getsockopt(fd, level, optname, optval, optlenptr) < 0)
    {
        err_sys("getsockopt error");
    }
}

#ifdef	HAVE_INET6_RTH_INIT

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
int 
(int type, int segments)
{
    int ret;

    ret = inet6_rth_space(type, segments);
    if (ret < 0)
    {
        err_quit("inet6_rth_space error");
    }
    return ret;
}

/**
 * @function: 初始化IPv6路由头缓冲区
 * @parameter: 
 *      rthbuf：指向的缓冲区
 *      type：  路由头
 * @return: 
 *      success: rthbuf指向的缓冲区
 *      error:   NULL
 * @note: 
 */
void *Inet6_rth_init(void *rthbuf, socklen_t rthlen, int type, int segments)
{
    void *ret;

    ret = inet6_rth_init(rthbuf, rthlen, type, segments);
    if (ret == NULL)
    {
        err_quit("inet6_rth_init error");
    }
    return ret;
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
void Inet6_rth_add(void *rthbuf, const struct in6_addr *addr)
{
    if(inet6_rth_add(rthbuf, addr) < 0)
    {
        err_quit("inet6_rth_add error");
    }
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
void Inet6_rth_reverse(const void *in, void *out)
{
    if(inet6_rth_reverse(in, out) < 0)
    {
        err_quit("inet6_rth_reverse error");
    }
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
int Inet6_rth_segments(const void *rthbuf)
{
    int ret;

    ret = inet6_rth_segments(rthbuf);
    if(ret < 0)
    {
        err_quit("inet6_rth_segments error");
    }
    return ret;
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
struct in6_addr *Inet6_rth_getaddr(const void *rthbuf, int idx)
{
    struct in6_addr *ret;

    ret = inet6_rth_getaddr(rthbuf, idx);
    if(ret == NULL)
    {
        err_quit("inet6_rth_getaddr error");
    }
    return ret;
}

#endif

#ifdef HAVE_KQUEUE

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
int Kqueue(void)
{
    int ret;

    if( (ret = kqueue()) < 0)
    {
        err_sys("kqueue error");
    }
    return ret;
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
int Kevent(int kq, const struct kevent *changelist, 
           int nchanges, struct kevent *eventlist, 
           int nevents, const struct timespec *timeout)
{
    int ret;

    if ((ret = kevent(kq, changelist, 
                      nchanges, eventlist, 
                      nevents, timeout)) < 0)
    {
        err_sys("kevent error");
    }
	return ret;
}

#endif

/* include Listen */
void Listen(int fd, int backlog)
{
    char *ptr;

    /*4can override 2nd argument with environment variable */
    if( (ptr = getenv("LISTENQ")) != NULL)
    {
        backlog = atoi(ptr);
    }
    if(listen(fd, backlog) < 0)
    {
        err_sys("listen error");
    }
}

#ifdef	HAVE_POLL

int Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int	n;

    if( (n = poll(fdarray, nfds, timeout)) < 0)
    {
        err_sys("poll error");
    }
    return(n);
}

#endif

ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags)
{
    ssize_t	n;

    if( (n = recv(fd, ptr, nbytes, flags)) < 0)
    {
        err_sys("recv error");
    }
    return(n);
}

ssize_t Recvfrom(int fd, void *ptr, 
                 size_t nbytes, int flags,
		         struct sockaddr *sa, socklen_t *salenptr)
{
	ssize_t	n;

	if ( (n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
    {
		err_sys("recvfrom error");
    }
	return(n);
}

ssize_t Recvmsg(int fd, struct msghdr *msg, int flags)
{
	ssize_t		n;

	if ( (n = recvmsg(fd, msg, flags)) < 0)
    {
		err_sys("recvmsg error");
    }
	return(n);
}

int Select(int nfds, fd_set *readfds, 
           fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout)
{
	int	n;
    if( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
    {
        err_sys("select error");
    }
    return(n);		/* can return 0 on timeout */
}

void Send(int fd, const void *ptr, size_t nbytes, int flags)
{
    if(send(fd, ptr, nbytes, flags) != (ssize_t)nbytes)
    {
        err_sys("send error");
    }
}

void Sendto(int fd, const void *ptr, 
            size_t nbytes, int flags,
	        const struct sockaddr *sa, socklen_t salen)
{
    if(sendto(fd, ptr, nbytes, flags, sa, salen) != (ssize_t)nbytes)
    {
        err_sys("sendto error");
    }
}

void Sendmsg(int fd, const struct msghdr *msg, int flags)
{
    unsigned int i;
    ssize_t nbytes;

    nbytes = 0;	/* must first figure out what return value should be */

    for(i = 0; i < msg->msg_iovlen; i++)
    {
        nbytes += msg->msg_iov[i].iov_len;
    }
    if(sendmsg(fd, msg, flags) != nbytes)
    {
        err_sys("sendmsg error");
    }
}

void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    if(setsockopt(fd, level, optname, optval, optlen) < 0)
    {
        err_sys("setsockopt error");
    }
}

void Shutdown(int fd, int how)
{
    if(shutdown(fd, how) < 0)
    {
        err_sys("shutdown error");
    }
}

int Sockatmark(int fd)
{
    int n;

    if( (n = sockatmark(fd)) < 0)
    {
        err_sys("sockatmark error");
    }
    return(n);
}

/* include Socket */
int Socket(int family, int type, int protocol)
{
    int		n;

    if( (n = socket(family, type, protocol)) < 0)
    {
        err_sys("socket error");
    }
    return(n);
}

void Socketpair(int family, int type, int protocol, int *fd)
{
    int		n;

    if( (n = socketpair(family, type, protocol, fd)) < 0)
    {
        err_sys("socketpair error");
    }
}