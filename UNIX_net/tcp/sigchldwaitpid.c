/*
 * @Author: cpu_code
 * @Date: 2020-05-06 22:34:27
 * @LastEditTime: 2020-05-06 22:38:02
 * @FilePath: \linux_network\UNIX_net\tcp\sigchldwaitpid.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

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
