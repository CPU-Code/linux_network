/*
 * @Author: cpu_code
 * @Date: 2020-05-06 19:20:46
 * @LastEditTime: 2020-05-06 19:33:53
 * @FilePath: \linux_network\UNIX_net\lib\sockfd_to_family.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/* include sockfd_to_family */
#include	"unp.h"

int Sockfd_to_family(int sockfd);
int sockfd_to_family(int sockfd);

int sockfd_to_family(int sockfd)
{
    //通用结构
    struct sockaddr_storage ss;
    socklen_t	len;

    len = sizeof(ss);

    //返回地址族
    if (getsockname(sockfd, (SA *) &ss, &len) < 0)
    {
        return(-1);
    }
    return(ss.ss_family);
}

/**
 * @function:  获取套接字的地址族
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
int Sockfd_to_family(int sockfd)
{
    int		rc;

    if( (rc = sockfd_to_family(sockfd)) < 0)
    {
        err_sys("sockfd_to_family error");
    }
    return(rc);
}