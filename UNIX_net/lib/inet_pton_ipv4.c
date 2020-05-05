/*
 * @Author: cpu_code
 * @Date: 2020-05-04 18:05:44
 * @LastEditTime: 2020-05-04 21:57:05
 * @FilePath: \linux_network\UNIX_net\lib\inet_pton_ipv4.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
/* Delete following line if your system's headers already DefinE this
   function prototype */
int inet_aton(const char *, struct in_addr *);

int inet_pton(int family, const char *strptr, void *addrptr);

/**
 * @function: 将“点分十进制” －> “二进制整数”
 * @parameter: 
 * @return: 
 * @note: 
 *   addrptr = 1108060352
 *   addrptr = 192.168.11.66 
 */
int main()
{
    char strptr[] = "192.168.11.66";
    unsigned int addrptr = 0;
    unsigned char *ptr = NULL;
    inet_pton(AF_INET, (const char *)&strptr, &addrptr);
    
    printf("addrptr = %d\n",addrptr);

    ptr = (unsigned char *)&addrptr;
    printf("addrptr = %d.%d.%d.%d \n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
    return 0;
}


/* include inet_pton */
int inet_pton(int family, const char *strptr, void *addrptr)
{
    if(family == AF_INET)
    {
        struct in_addr in_val;

        if(inet_aton(strptr, &in_val))
        {
            memcpy(addrptr, &in_val, sizeof(struct in_addr));
            return (1);
        }
        return(0);
    }
    errno = EAFNOSUPPORT;
    return (-1);
}