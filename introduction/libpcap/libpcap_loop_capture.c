/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:55:16
 * @FilePath: \linux_network\introduction\libpcap\libpcap_loop_capture.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ gcc libpcap_loop_capture.c -lpcap
 *capcpucode@ubuntu:~/cpucodefile/network/libpcap$ ./a.out 
 *Segmentation fault (core dumped)
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ sudo ./a.out 
 *[sudo] password for cpucode: 
 *----------------------------------------------------
 *Thu Apr 23 20:30:32 2020
 *Mac Source Address is 48:e2:44:7b:b0:c5
 *Mac Destination Address is ff:ff:ff:ff:ff:ff
 *Ethernet type is :0800
 *The network layer is IP protocol
 */

/*
 *function:
 *  通过回调函数的方式，来抓取多个网络数据包
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1514

struct ether_header
{
    unsigned char ether_dhost[6];	//目的mac
    unsigned char ether_shost[6];	//源mac
    unsigned short ether_type;		//以太网类型
};

void ethernet_protocol_callback(unsigned char *argument,
                                const struct pcap_pkthdr *packet_heaher,
                                const unsigned char *packet_content);

int main(int argc, char *argv[])
{
    char error_content[100];	//出错信息
    pcap_t * pcap_handle;
    unsigned char *mac_string;
    unsigned short ethernet_type;    //以太网类型
    unsigned int net_ip;			//网络地址
    unsigned int net_mask;			//子网掩码
    char *net_interface = NULL;	    //接口名字
    int res = 0;
    struct pcap_pkthdr protocol_header;
    struct ether_header *ethernet_protocol;
    struct bpf_program bpf_filter;
    char bpf_filter_string[] = "ip";

    //获取网络接口
    net_interface = pcap_lookupdev(error_content);
    if(NULL == net_interface)
    {
        perror("pcap_lookupdev");
        exit(-1);
    }

    res = pcap_lookupnet(net_interface, &net_ip, &net_mask, error_content);
    if(-1 == res)
    {
        perror("pcap_loopupnet");
        exit(-1);
    }

    //打开网络接口
    pcap_handle = pcap_open_live(net_interface, BUFSIZE, 1, 0, error_content);
    //编译BPF过滤规则
    pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string, 0, net_mask);
    pcap_setfilter(pcap_handle, &bpf_filter);//设置过滤规则

    if(pcap_loop(pcap_handle, -1, ethernet_protocol_callback, NULL) < 0)
    {
        perror("pcap_loop");
    }

    pcap_close(pcap_handle);
	return 0;
}

// 回调函数
void ethernet_protocol_callback(unsigned char *argument,
                                const struct pcap_pkthdr *packet_heaher,
                                const unsigned char *packet_content)
{
    unsigned char *mac_string;
    struct ether_header *ethernet_protocol;
    unsigned short ethernet_type;	//以太网类型

    printf("----------------------------------------------------\n");
    printf("%s\n", ctime((time_t *)&(packet_heaher->ts.tv_sec))); //转换时间
    ethernet_protocol = (struct ether_header *)packet_content;

    mac_string = (unsigned char *)ethernet_protocol->ether_shost;   //获取源mac地址
    printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string + 0),
                                                                   *(mac_string + 1),
                                                                   *(mac_string + 2),
                                                                   *(mac_string + 3),
                                                                   *(mac_string + 4),
                                                                   *(mac_string + 5));

    mac_string = (unsigned char *)ethernet_protocol->ether_dhost;  //获取目的mac
    printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string + 0),
                                                                        *(mac_string + 1),
                                                                        *(mac_string + 2),
                                                                        *(mac_string + 3),
                                                                        *(mac_string + 4),
                                                                        *(mac_string + 5));

    ethernet_type = ntohs(ethernet_protocol->ether_type);    //获得以太网的类型
    printf("Ethernet type is :%04x\n",ethernet_type);
    switch(ethernet_type)
    {
        case 0x0800:
            printf("The network layer is IP protocol\n");    //ip
            break;
        case 0x0806:
            printf("The network layer is ARP protocol\n");    //arp
            break;
        case 0x0835:
            printf("The network layer is RARP protocol\n");    //rarp
            break;
        default:
            break;
    }

    usleep(800*1000);
}
 