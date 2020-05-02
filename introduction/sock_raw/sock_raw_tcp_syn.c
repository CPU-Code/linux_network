/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:56:59
 * @FilePath: \linux_network\introduction\sock_raw\sock_raw_tcp_syn.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

unsigned short checksum(unsigned short *buf, int nword);

/*
 *function:
 *  创建原始套接字，然后手动组织tcp数据包，然后发送
 */
int main(int argc, char *argv[])
{
    int sockfd = 0;
    struct sockaddr_in dest_addr;
    /*组织发送的信息；
	 *note：
	 *	伪头部
	 *	0x50的高4位代表是首部长度，要注意其代表的是4Byte的个数，即0x50代表的首部长度为20（5*4）
	 *	本次发送的tcp数据包，只有tcp包头，不包含任何tcp数据
     */
    char tcp_checksum_buf[] = {
        /******* 伪头部 开始 ******/
        172, 20, 223, 119,			//src ip
        172, 20, 223, 83,			//dst ip
        0x00,					    //默认
        6,					    	//6 为tcp
        0x00, 20,				//tcp 头部长度
        /******* 伪头部 结束 ******/
        /******* TCP头部 开始 *****/
        0x55, 0x22,				//tcp src port
        0x00, 80,				//tcp dst port,port=80
        0x00, 0x00, 0x00, 0x01,	//tcp id
        0x00, 0x00, 0x00, 0x00,	//tcp ack
        0x50, 0x02, 0x17, 0x70,	//4位首部长度+6位保留+6个标志位（URG/ACK/PSH/PST/SYN/FIN）+16位窗口大小
        0x00, 0x00,				//16位tcp校验和
        0x00, 0x00				//16位紧急指针
        /******* TCP头部 结束 ******/
    };

    //创建 网络层原始套接字，并且指定将来作用tcp相关
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket error");
        //exit(1):异常退出
        //exit(0):正常退出
        exit(1);
    }
    //配置 结构体变量，代表着目的主机的信息
    bzero(&dest_addr, sizeof(dest_addr));    //初始化
    //套接字域是AF_INET(网络套接字)
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80);
    //设置目的ip
    dest_addr.sin_addr.s_addr = inet_addr("172.20.223.83");

    // 对发送的信息进行校验
    *((unsigned short *)&tcp_checksum_buf[28]) = htons(checksum((unsigned short *)tcp_checksum_buf, 
                                                                sizeof(tcp_checksum_buf)/2));
    // 发送数据到指定目的
    sendto(sockfd, tcp_checksum_buf + 12, 20, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

    //关闭套接字
    close(sockfd);
	return 0;
}

/*
 *function:
 *  对数据进行校验
 *note：
 *  注意伪头部的问题
 */
unsigned short checksum(unsigned short *buf, int nword)
{
    unsigned long sum;

    for(sum = 0; nword > 0; nword--)
    {
        sum += htons(*buf);
        buf++;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
}