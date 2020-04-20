/*
 *与网络调试助手配合使用
 * send data to UDP server 192.168.43.76:8080!
 * sb	
 * cpucode
 * ^C
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
    char *server_ip = "192.168.43.76"; 
    int sockfd;  
    struct sockaddr_in dest_addr; 
    /* 指定server信息 */
    if(argc > 1)                    //服务器ip地址
    {  
        server_ip = argv[1]; 
    } 
    if(argc > 2)                    //服务器端口
    { 
        port = atoi(argv[2]); 
    } 
                           
    sockfd = socket(AF_INET,SOCK_DGRAM,0);        //创建UDP套接字
    if(sockfd < 0) 
    { 
        perror("socket"); 
		exit(-1); 
    } 

    /* 填充目的server的信息 */
    bzero(&dest_addr,sizeof(dest_addr)); 
    dest_addr.sin_family = AF_INET; 
    dest_addr.sin_port = htons(port); 

    inet_pton(AF_INET,server_ip,&dest_addr.sin_addr); 
    printf("send data to UDP server %s:%d!\n",server_ip,port); 

    while(1)                                         //发送数据到server
    { 
        char send_buf[512] = " "; 

        fgets(send_buf,sizeof(send_buf),stdin);       //获取输入
        send_buf[strlen(send_buf)-1] = '\0'; 

        sendto(sockfd, 
               send_buf, 
               strlen(send_buf), 
               0, 
               (struct sockaddr *)&dest_addr,
               sizeof(dest_addr)); 
    } 
    close(sockfd); 

    return 0; 
}