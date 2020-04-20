/*
 *Binding server to port 8080
 *receive data ...
 *
 *ip:192.168.43.76,port:8080
 *data(2):sb
 *
 *ip:192.168.43.76,port:8080
 *data(7):cpucode
 *^C  
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
    unsigned short port = 8080;
    int sockfd;
    struct sockaddr_in my_addr; 	
    int err_log;

    if(argc > 1)                    //修改本程序的端口 
    { 
        port = atoi(argv[1]); 
    }

    sockfd =socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd < 0)                    //创建套接字 
    { 
        perror("socket"); 
        exit(-1); 
    }

  	//填充本程序的信息 
    bzero(&my_addr,sizeof(my_addr)); 

    my_addr.sin_family = AF_INET; 
    my_addr.sin_port = htons(port); 
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Binding server to port %d\n",port); 

    err_log = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if(err_log != 0) //绑定本程序要使用的信息 
    { 
        perror("bind"); 
        close(sockfd); 
        exit(-1); 
    }

    printf("receive data ...\n"); 
    while(1)                    //收取数据 
    { 
        int recv_len; 
        char recv_buf[512] = " "; 
        struct sockaddr_in client_addr; 
        char cli_ip[INET_ADDRSTRLEN] = " "; 
        socklen_t cliaddr_len = sizeof(client_addr);

        recv_len = recvfrom(sockfd, 
                            recv_buf, 
                            sizeof(recv_buf), 
                            0, 
                            (struct sockaddr *)&client_addr, 
                            &cliaddr_len);

        inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip,INET_ADDRSTRLEN); 

        printf("\nip:%s,port:%d\n",cli_ip ,ntohs(client_addr.sin_port));     
        printf("data(%d):%s\n",recv_len,recv_buf); 
    } 

    close(sockfd);

    return 0; 
}
