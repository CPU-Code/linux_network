/*
 * @Author: cpu_code
 * @Date: 2020-05-04 15:36:02
 * @LastEditTime: 2020-05-05 12:36:50
 * @FilePath: \linux_network\UNIX_net\lib\inet_ntop_ipv4.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifndef	INET_ADDRSTRLEN
#define	INET_ADDRSTRLEN		16
#endif

const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);

/**
 * @function: 将 “二进制整数” －> “点分十进制”  
 * @parameter: 
 * @return: 
 * @note: 
 *      strptr = 154.45.2.52 
 */
int main()
{
    const unsigned char addrptr[] = {154, 45, 02, 52};
    char strptr[16] = " NULL";

    inet_ntop(AF_INET, addrptr, strptr, 16);

    printf("strptr = %s \n", strptr);
    return 0;
}
/* include inet_ntop */
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
    const u_char *p = (const u_char *) addrptr;

    if (family == AF_INET)
    {
        char temp[INET_ADDRSTRLEN];
        snprintf(temp, sizeof(temp), "%d.%d.%d.%d", p[0], p[1],
                                                    p[2], p[3]);
        if (strlen(temp) >= len)
        {
            errno = ENOSPC;
            return (NULL);
        }
        strcpy(strptr, temp);
        return (strptr);
    }
    errno = EAFNOSUPPORT;
	return (NULL);
}
