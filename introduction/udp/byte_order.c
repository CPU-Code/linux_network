/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:57:47
 * @FilePath: \linux_network\introduction\udp\byte_order.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 *un.s = 102H
 *un.c[0] = 2
 *un.c[1] = 1
 *little-endian
 */
#include <stdio.h>

int main(int argc, char *argv[])
 { 
    union 
    { 
        short s; 
        char c[sizeof(short)]; 
    }un; 
    un.s = 0x0102; 

    if((un.c[0] == 1) && (un.c[1] == 2))
    { 
        printf("un.s = %x H\n", un.s);

        printf("un.c[0] = %d\n",un.c[0]);
        printf("un.c[1] = %d\n",un.c[1]);

        printf("big-endian\n"); 
    } 
    else if((un.c[0] == 2) &&(un.c[1] == 1)) 
    {
        printf("un.s = %x H\n", un.s);

        printf("un.c[0] = %d\n",un.c[0]);
        printf("un.c[1] = %d\n",un.c[1]);
        
        printf("little-endian\n"); 
    }

    return 0; 
}
