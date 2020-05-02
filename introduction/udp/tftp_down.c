/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:58:05
 * @FilePath: \linux_network\introduction\udp\tftp_down.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

static int sockfd;

void sig_dispose(int sig)
{
    if(SIGINT == sig)
    {
        close(sockfd);
        puts("\n close!");
        exit(0);
    }
}

/*
 *function：
 *  tftp客户端程序，从tftp服务器下载程序
 */
int main(int argc, char *argv[])
{
    unsigned short p_num = 0;
    unsigned char cmd = 0;
    char cmd_buf[512] = "";
    char recv_buf[516] = "";
    struct sockaddr_in client_addr;
    socklen_t cliaddr_len = sizeof(client_addr);
    struct sockaddr_in dest_addr;
    int len;

    signal(SIGINT, sig_dispose);

    if(argc < 3)
    {
        /* 参数个数小于3，认为输入命令错误 */
        printf("cmd example: ./tftpc 192.168.x.x hello.text \n");
       return 0;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        perror("socket error");
        exit(-1);
    } 

    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(69);
    inet_pton(AF_INET, argv[1], &dest_addr.sin_addr);
    /* 构造下载请求， argv[2]为文件名 */
    len = sprintf(cmd_buf, 
                  "%c%c%s%c%s%c", 
                  0, 
                  1, 
                  argv[2], 
                  0, 
                  "octet", 
                  0);
    /* 发送读数据包请求 */
    sendto(sockfd,
           cmd_buf, 
           len, 
           0, 
           (struct sockaddr *)&dest_addr, 
           sizeof(dest_addr));
    int fd = open(argv[2], O_WRONLY | O_CREAT, 0666);
    if(fd < 0)
    {
        perror("open error");
        close(sockfd);
        exit(-1);
    }

    do
    {
        len =recvfrom(sockfd, 
                      recv_buf, 
                      sizeof(recv_buf),
                      0,
                      (struct sockaddr *)&client_addr,
                      &cliaddr_len);
        cmd = recv_buf[1];
        /* 是否为数据包 */
        if(cmd == 3)
        {
            //接收的包编号是否为上次包的编号+1
            if((unsigned short)(p_num + 1) == ntohs(*(unsigned short *)(recv_buf + 2)))
            {
                write(fd, recv_buf + 4, len -4);
                p_num = ntohs(*(unsigned short *)(recv_buf + 2));
                printf("recv:%d\n",p_num);
            }
            recv_buf[1] = 4;
            sendto(sockfd, 
                   recv_buf, 
                    4, 
                    0, 
                    (struct sockaddr *)&client_addr, 
                    sizeof(client_addr));
        }
        else if(cmd == 5)
        {
            close(sockfd);
            close(fd);
            unlink(argv[2]);        //删除文件
            printf("error:%s \n",recv_buf + 4);
            exit(-1);
        }
    }while(len == 516);
    //如果收到的数据小于516，则出错
    close(fd);
    return 0;
}
    

    