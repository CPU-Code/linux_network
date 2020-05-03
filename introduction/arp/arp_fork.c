/*
 * @Author: cpu_code
 * @Date: 2020-05-03 19:23:27
 * @LastEditTime: 2020-05-03 19:24:28
 * @FilePath: \linux_network\introduction\arp\arp_fork.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <libnet.h>
#include <unistd.h>

int sockfd = 0;
struct sockaddr_ll sll;

typedef struct arp_link
{
	unsigned char ip[4];
	unsigned char mac[6];
	struct arp_link *next;
}ARP_LINK;

ARP_LINK * head;

int main(int argc, char *argv[])
{
    pthread_t send_id = 0;
	pthread_t get_id = 0;
	pthread_t arp_id = 0;

    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
    pthread_create(&send_id, NULL, send_arp_ask, NULL);
	pthread_create(&get_id, NULL, get_arp_answer, NULL);
	pthread_create(&arp_id, NULL, send_arp_req, NULL);

    pthread_join(send_id, NULL);
	pthread_join(get_id, NULL);
	pthread_join(arp_id, NULL);

    return 0;
}