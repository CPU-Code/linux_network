/*
 * @Author: cpu_code
 * @Date: 2020-05-06 21:16:23
 * @LastEditTime: 2020-05-06 21:18:33
 * @FilePath: \linux_network\UNIX_net\lib\sigchldwait.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

/**
 * @function: wait的SIGCHLD信号处理函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void sig_chld(int signo)
{
    pid_t	pid;
    int		stat;

    pid = wait(&stat);
    printf("child %d terminated\n", pid);
    return;
}