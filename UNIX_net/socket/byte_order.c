/*
 * @Author: cpu_code
 * @Date: 2020-05-03 22:15:53
 * @LastEditTime: 2020-05-04 15:32:16
 * @FilePath: \linux_network\UNIX_net\socket\byte_order.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include	"unp.h"

/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 *      x86_64-unknown-linux-gnu: little-endian
 */
int main(int argc, char **argv)
{
	union 
    {
	  short  s;
      char   c[sizeof(short)];
    } un;

	un.s = 0x0102;
    // 标识CPU类型、厂家和操作系统版本
	printf("%s: ", CPU_VENDOR_OS);

	if (sizeof(short) == 2) 
    {
		if (un.c[0] == 1 && un.c[1] == 2)
        {
			printf("big-endian\n");
        }
		else if (un.c[0] == 2 && un.c[1] == 1)
        {
			printf("little-endian\n");
        }
		else
        {
			printf("unknown\n");
        }
	} 
    else
    {
		printf("sizeof(short) = %d\n", sizeof(short));
    }

	exit(0);
}
