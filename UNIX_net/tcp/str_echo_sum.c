/*
 * @Author: cpu_code
 * @Date: 2020-05-07 16:33:01
 * @LastEditTime: 2020-05-07 16:40:07
 * @FilePath: \linux_network\UNIX_net\tcp\str_echo_sum.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

/**
 * @function: 两个数求和
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void str_echo(int sockfd)
{
    long		arg1, arg2;
    ssize_t		n;
    char		line[MAXLINE];

    for( ; ; )
    {
        if( (n = Readline(sockfd, line, MAXLINE)) == 0)
        {
            return;		/* connection closed by other end */
        }

        //两个参数转换为长整数
        if(sscanf(line, "%ld%ld", &arg1, &arg2) == 2)
        {
            //结果转换为文本串
            snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
        }
        else
        {
            snprintf(line, sizeof(line), "input error\n");
        }

        n = strlen(line);
        Writen(sockfd, line, n);
    }
}