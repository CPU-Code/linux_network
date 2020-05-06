/*
 * @Author: cpu_code
 * @Date: 2020-05-06 20:43:44
 * @LastEditTime: 2020-05-06 21:15:54
 * @FilePath: \linux_network\UNIX_net\lib\signal.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include signal */
#include	"unp.h"

Sigfunc *signal(int signo, Sigfunc *func);
Sigfunc *Signal(int signo, Sigfunc *func);

Sigfunc *signal(int signo, Sigfunc *func)
{
    struct sigaction	act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM)
    {
#ifdef	SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
    }
    else
    {
#ifdef	SA_RESTART
       //相应信号中断的系统调用将由内核自动重启
        act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
    }
    if(sigaction(signo, &act, &oact) < 0)
    {
        return(SIG_ERR);
    }
    return(oact.sa_handler);
}

Sigfunc *Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
    Sigfunc	*sigfunc;
    if( (sigfunc = signal(signo, func)) == SIG_ERR)
    {
        err_sys("signal error");
    }
    return(sigfunc);
}