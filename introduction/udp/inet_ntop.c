/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:57:56
 * @FilePath: \linux_network\introduction\udp\inet_ntop.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h> 
#include <arpa/inet.h> 

int main() 
{ 
    unsigned char ip[] = {172,20,223,75};
    char ip_str[16] = "NULL";

    inet_ntop(AF_INET,(unsigned int *)ip,ip_str,16);
    printf("ip_str = %s\n",ip_str); 

    return 0; 
}