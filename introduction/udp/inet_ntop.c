/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-03 22:39:08
 * @FilePath: \linux_network\introduction\udp\inet_ntop.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h> 
#include <arpa/inet.h> 

/**
 * @function: 将主机字符转换为网络点式
 * @parameter: 
 * @return: 
 * @note: 
 *      ip_str = 172.20.223.75
 */
int main() 
{ 
    unsigned char ip[] = {172,20,223,75};
    char ip_str[16] = "NULL";

    inet_ntop(AF_INET,(unsigned int *)ip,ip_str,16);
    printf("ip_str = %s\n",ip_str); 

    return 0; 
}