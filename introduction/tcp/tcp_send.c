/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:57:33
 * @FilePath: \linux_network\introduction\tcp\tcp_send.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
 
int main(int argc, char *argv[])
{
    unsigned short port = 8000;         //服务器的端口号
    char *server_ip = "172.20.226.11";    //服务器的IP
 
    //给main传参
    if(argc > 1)                        //函数参数，可以改服务器IP
    {
        server_ip = argv[1];
    }
    if(argc > 2)                        //函数参数，可以改服务器端口号
    {
        port = atoi(argv[2]);
    }
 
    //创建TCP套接字
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  //创建通行端点：套接字
    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }
    
    //设置连接的IP，端口
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));    //初始化服务器的地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    
    //连接服务器
    int err_log = connect(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr));
    if(err_log != 0)
    {
        perror("connect");
        close(sockfd);
        exit(-1);
    }
     
    char send_buf[512] = "";
    char recv_buf[512] = "";
    printf("send data to %s：%d \n", server_ip,port);
    
    //发送消息
    printf("send");
    fgets(send_buf, sizeof(send_buf), stdin);
    send_buf[strlen(send_buf) - 1] = 0;            //除去‘\n’
    send(sockfd, send_buf, strlen(send_buf), 0);   //向服务器发送数据
        
    //接收数据
    recv(sockfd, recv_buf, sizeof(recv_buf), 0);  //接受服务器的响应
    printf("recv: %s \n",recv_buf);
    close(sockfd);
 
}