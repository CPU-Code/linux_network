/*
 * @Author: cpu_code
 * @Date: 2020-05-07 16:54:42
 * @LastEditTime: 2020-05-07 17:10:29
 * @FilePath: \linux_network\UNIX_net\tcp\str_echo_sum_bin.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"
#include	"sum.h"

/**
 * @function: 对两个二进制整数求和
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void str_echo(int sockfd)
{
    ssize_t			n;
    struct args		args;
    struct result	result;

    for( ; ; )
    {
        //读入参数
        if( (n = Readn(sockfd, &args, sizeof(args))) == 0)
        {
            return;		/* connection closed by other end */
        }

        //计算并存储两数之和
        result.sum = args.arg1 + args.arg2;

        //把结果结构发回
        Writen(sockfd, &result, sizeof(result));
    }
}