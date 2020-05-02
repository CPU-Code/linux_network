/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:57:30
 * @FilePath: \linux_network\introduction\tcp\tcp_recv.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main(int argc, char *argv[])
{
    unsigned short port = 8000;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }
    //创建TCP套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }
    //组织本地信息
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定信息
    int err_log = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("binding");
        close(sockfd);
        exit(-1);
    }
    //“主动”变“被动”
    err_log = listen(sockfd, 10);
    if(err_log != 0)
    {
        perror("listen");
        close(sockfd);
        exit(-1);
    }
    printf("listen client &poort = %d...\n",port);
    while(1)
    {
        struct sockaddr_in client_addr;
        char cli_ip[INET_ADDRSTRLEN] ="";
        socklen_t cliaddr_len sizeof(client_addr);
        
        int connfd;
        //等待连接的到来
        connfd = accept(sockfd, (struct sockaddr *)&client_addr, &cliaddr_len);
        if(connfd < 0)
        {
            perror("accept");
            continue;
        }
        //转换并打印信息
        inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
        printf("--------------------------------\n");
        printf("client ip = %s, port = %d\n",cli_ip, ntohs(client_addr.sin_port));
 
        char recv_buf[2048] = "";
        //接收消息
        while(recv(connfd, recv_buf, sizeof(recv_buf), 0) > 0)
        {
            printf("\n recv data: \n");
            printf("%s\n",recv_buf);
        }
        
        close(connfd);        //关闭已连接的套接字
        printf("client closed \n");
    }
    close(sockfd);        //关闭监听的套接字
    return 0;
}