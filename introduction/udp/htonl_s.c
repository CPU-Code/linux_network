/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-03 22:36:57
 * @FilePath: \linux_network\introduction\udp\htonl_s.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 *htonl(0x01020304) = 0x04030201 
 *htons(0x0102) = 0x0201 
 */
#include <stdio.h> 
#include <arpa/inet.h>

/**
 * @function: 把主机改成网络顺序
 * @parameter: 
 * @return: 
 * @note: 
 *       htonl(0x01020304) = 0x04030201 
 *       htons(0x0102) = 0x0201 
 */
int main(int argc, char *argv[]) 
{ 
    int a = 0x01020304;
    short int b = 0x0102; 
    printf("htonl(0x%08x) = 0x%08x \n", a, htonl(a)); 
    printf("htons(0x%04x) = 0x%04x \n", b, htons(b)); 

    return 0; 
}
