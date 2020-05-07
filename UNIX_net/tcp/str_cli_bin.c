/*
 * @Author: cpu_code
 * @Date: 2020-05-07 16:43:41
 * @LastEditTime: 2020-05-07 16:51:16
 * @FilePath: \linux_network\UNIX_net\tcp\str_cli_bin.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"
#include	"sum.h"

/**
 * @function: 发送两个二进制整数给服务器
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void str_cli(FILE *fp, int sockfd)
{
    char			sendline[MAXLINE];
    struct args		args;
    struct result	result;

    while(Fgets(sendline, MAXLINE, fp) != NULL)
    {
        //文本串转换为二进制数
        if(sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2)
        {
            printf("invalid input: %s", sendline);
            continue;
        }

        //发送给服务器
        Writen(sockfd, &args, sizeof(args));

        //读回应答
        if(Readn(sockfd, &result, sizeof(result)) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }
        //输出结果
        printf("%ld\n", result.sum);
    }
}