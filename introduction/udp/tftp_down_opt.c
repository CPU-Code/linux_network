/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:58:02
 * @FilePath: \linux_network\introduction\udp\tftp_down_opt.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

static int sockfd;
static struct sockaddr_in client_addr;

static void sig_dispose(int sig);
/**
 * @function: 
 * @parameter: 
 * @return: 
 * @note: 
 */
int main(int argc, char *argv[])
{
    struct sockaddr_in dest_addr;
    unsigned char send_buf[1024];
    int len;
    FILE *fp;
    char recv_buf[2048];
	char client_ip[INET_ADDRSTRLEN];
    unsigned short pack_num;
    socklen_t client_addr_len = sizeof(client_addr);

    signal(SIGINT, sig_dispose);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        exit(-1);
    }

    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(8080);
    inet_pton(AF_INET,"192.168.43.76", &dest_addr.sin_addr.s_addr);

    bzero(send_buf, sizeof(send_buf));
    len = sprintf(send_buf,"%c%c%s%c%s%c%s%c%s%c%s%c%d%c", 0, 1, "01.jpg", 0,
                                                                 "octet", 0,
                                                                 "tsize",0,
                                                                 "0", 0,
                                                                 "blksize", 0, 
                                                                 600, 0);
    sendto(sockfd, send_buf, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    len = recvfrom(sockfd,
                   recv_buf, 
                   sizeof(recv_buf), 
                   0, 
                   (struct sockaddr *)&client_addr,
                   &client_addr_len);
    if(recv_buf[1] != 6)
    {
        perror("fopen:");
        close(sockfd);
        return 0;
    }
    len = sprintf(send_buf, "%c%c%c%c", 0, 4, 0, 0);
    sendto(sockfd, send_buf, len, 0, (struct sockaddr *)&client_addr,sizeof(client_addr));
    
    fp = fopen("01.jpg", "wb");
    if(fp == NULL)
    {
        perror("fopen");
        close(sockfd);
        return 0;
    }
    printf("IP:%s port:%d: \n",inet_ntop(AF_INET, 
                                         &client_addr.sin_addr, 
                                         client_ip, 
                                         INET_ADDRSTRLEN),
                               ntohs(client_addr.sin_port));
    len = 604;
    while(len == 604)
    {
        len = recvfrom(sockfd, 
                       recv_buf, 
                       sizeof(recv_buf), 
                       0, 
                       (struct sockaddr *)&client_addr,
                       &client_addr_len);
        pack_num = *(unsigned short *)(recv_buf + 2);
        printf("pack_num = %d\n",ntohs(pack_num));
        if(recv_buf[1] == 3)
        {
            send_buf[0] = 0;
            send_buf[1] = 4;
            send_buf[2] = recv_buf[2];
            send_buf[3] = recv_buf[3];
            sendto(sockfd, 
                   send_buf,
                   4,
                   0,
                   (struct sockaddr *)&client_addr, 
                   sizeof(client_addr));
            fwrite(recv_buf + 4, 1, len -4, fp);
        }
        else if(recv_buf[1] == 5)
        {
            puts(recv_buf + 4);
            break;
        }
    }
    
    close(sockfd);
    fclose(fp);
    return 0;
}

static void sig_dispose(int sig)
{
    if(SIGINT == sig)
    {
        close(sockfd);
        puts("\n nclose");
        exit(0);
    }
}