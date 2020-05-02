/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:55:19
 * @FilePath: \linux_network\introduction\libpcap\libprap_capture.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ ./a.out 
 *Segmentation fault (core dumped)
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ sudo ./a.out 
 *[sudo] password for cpucode: 
 *----------------------------------------------------
 *capture a Packet from p_net_interface_name :ens33
 *Capture Time is :Thu Apr 23 07:33:06 2020
 *Packet Lenght is :216
 *Mac Source Address is 48:e2:44:7b:b0:c5
 *Mac Destination Address is 01:00:5e:7f:ff:fa
 *Ethernet type is :0800	The network layer is IP protocol
 */
/*
 *function:
 *  通过捕获一个网络数据包，然后对其进行数据的解析分析
 */
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 1514

struct ether_header
{
    unsigned char ether_dhost[6];	//目的mac
    unsigned char ether_shost[6];	//源mac
    unsigned short ether_type;		//以太网类型
};

/*
 *function:
 *  通过使用libpcap接收一个数据包，然后对数据包进行解析
 */
int main(int argc,char *argv[])
{
    pcap_t * pcap_handle = NULL;
    char error_content[100] = "";	                // 出错信息
    unsigned char *p_packet_content = NULL;		// 保存接收到的数据包的起始地址
    unsigned char *p_mac_string = NULL;			// 保存mac的地址，临时变量
    unsigned short ethernet_type = 0;			// 以太网类型
    char *p_net_interface_name = NULL;	    	// 接口名字
    struct pcap_pkthdr protocol_header;
    struct ether_header *ethernet_protocol;

    //获得接口名
    p_net_interface_name = pcap_lookupdev(error_content);
    if(NULL == p_net_interface_name)
    {
        perror("pcap_lookupdev");
        exit(-1);
    }

    //打开网络接口
    pcap_handle = pcap_open_live(p_net_interface_name, BUFSIZE, 1, 0, error_content);
    p_packet_content = pcap_next(pcap_handle, &protocol_header);

    printf("----------------------------------------------------\n");
    printf("capture a Packet from p_net_interface_name :%s\n", p_net_interface_name);
    printf("Capture Time is :%s", ctime((const time_t *)&protocol_header.ts.tv_sec));
    printf("Packet Lenght is :%d\n",protocol_header.len);

    //分析以太网中的 源mac、目的mac
    ethernet_protocol = (struct ether_header *)p_packet_content;

    p_mac_string = (unsigned char *)ethernet_protocol -> ether_shost;//获取源mac
    printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(p_mac_string + 0),
                                                                   *(p_mac_string + 1),
                                                                   *(p_mac_string + 2),
                                                                   *(p_mac_string + 3),
                                                                   *(p_mac_string + 4),
                                                                   *(p_mac_string + 5));

    p_mac_string = (unsigned char *)ethernet_protocol->ether_dhost;//获取目的mac
    printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(p_mac_string + 0),    
                                                                        *(p_mac_string + 1),
                                                                        *(p_mac_string + 2),
                                                                        *(p_mac_string + 3),
                                                                        *(p_mac_string + 4),
                                                                        *(p_mac_string + 5));

    //获得以太网的数据包的地址，然后分析出上层网络协议的类型
    ethernet_type = ntohs(ethernet_protocol->ether_type);
    printf("Ethernet type is :%04x\t",ethernet_type);
    switch(ethernet_type)
    {
        case 0x0800 : 
            printf("The network layer is IP protocol\n");    //ip
            break;    
        case 0x0806 :
            printf("The network layer is ARP protocol\n");    //arp
            break;
        case 0x0835 :
            printf("The network layer is RARP protocol\n");    //rarp
            break;
        default : 
            printf("The network layer unknow!\n");
            break;
    }

    pcap_close(pcap_handle);
    return 0;
}