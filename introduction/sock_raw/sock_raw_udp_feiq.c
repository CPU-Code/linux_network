#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>

unsigned short checksum(unsigned short *buf, int nword);

int main(int argc, char *argv[])
{
    int sockfd = 0;
    struct sockaddr_in dest_addr;
    //组织发送的信息；注意伪头部
    char udp_checksum_buf[] = {
        /****** 伪头部 开始 ******/
        172, 20, 223, 119,  //src ip
        172, 20, 223, 83,   //dst ip
        0x00,               //默认
        17,                 //udp
        0x00, 34,           //udp总长度(header lenth + udp data length)
        /****** 伪头部-结束 ******/
        /****** udp   首部 ******/
        0x09, 0x79,			//udp src port,2425(feiQ)
        0x09, 0x79,			//udp dst port,2425(feiQ)
        0x00, 34,			//udp总长度
        0x00, 0x00,			//udp data checksum,注意 校验和不计算也是可以的但是必须为0
        '1', ':',			//1 代表飞秋的版本号
        '1', '2', '3', ':',	//123 本次发送的飞秋的数据包的报编号
        't', 'o', 'm', ':',	//tom 发送者姓名
        'q', 'f', ':',	    //sun 发送者主机名
        '1', ':',			//1 代表上线
        'q', 'f', 'e', 'd', 'u'     //qfedu 发送者的名字
    };

    //创建 网络层原始套接字，并且指定将来作用udp相关
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("socket error");
        exit(-1);
    }

    //配置 结构体变量，代表着目的主机的信息
    bzero(&dest_addr, sizeof(dest_addr));   //初始化
    //套接字域AF_INET(网络套接字)
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(2425);
    //设置目的IP
    dest_addr.sin_addr.s_addr = inet_addr("172.20.223.83");

    //对发送的信息进行校验
    *((unsigned short *)&udp_checksum_buf[18]) = htons(checksum((unsigned short *)udp_checksum_buf, 
                                                                sizeof(udp_checksum_buf)/2));
    //发送数据到指定目的
    sendto(sockfd, udp_checksum_buf+12, 34, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

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