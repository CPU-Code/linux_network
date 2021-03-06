/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-03 22:45:12
 * @FilePath: \linux_network\introduction\udp\inet_pton.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h> 
#include <arpa/inet.h> 

/**
 * @function:  将网络点式转换为主机字符
 * @parameter: 
 * @return: 
 * @note: 
 *       ip_uint = 199365804
 *       ip_uint = 172.20.226.11
 */
int main(int argc, char *argv[]) 
{ 
    char ip_str[] = "172.20.226.11"; 
    unsigned int ip_uint = 0; 
    unsigned char *ip_p = NULL; 

    inet_pton(AF_INET, ip_str, &ip_uint); 
    printf("ip_uint = %d\n", ip_uint); 

    ip_p = (unsigned char *)&ip_uint; 
    printf("ip_uint = %d.%d.%d.%d\n", *ip_p , *(ip_p + 1), *(ip_p +2), *(ip_p + 3)); 

    return 0; 
}
