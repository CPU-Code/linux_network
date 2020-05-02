/*
 * @Author: cpu_code
 * @Date: 2020-05-02 10:29:30
 * @LastEditTime: 2020-05-02 21:53:43
 * @FilePath: \linux_network\introduction\libpcap\libpcap_filter_capture.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
/*
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ gcc libpcap_filter_capture.c -lpcap
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ ./a.out 
 *Segmentation fault (core dumped)
 *cpucode@ubuntu:~/cpucodefile/network/libpcap$ sudo ./a.out 
 *[sudo] password for cpucode: 
 *-----------------------------------------------
 *capture a Packet from p_net_interface_name :ens33
 *Capture Time is :Thu Apr 23 19:29:20 2020
 *Packet Lenght is :216
 *Mac Source Address is 48:e2:44:7b:b0:c5
 *Mac Destination Address is 01:00:5e:7f:ff:fa
 *Ethernet type is :0800
 *The network layer is IP protocol
 */

/*
 *function：
 *  安排规则,接收一个数据包
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <pcap.h>

#define BUFSIZE 1514

struct ether_header
{
    unsigned char ether_dhost[6];	//目的mac
    unsigned char ether_shost[6];	//源mac
    unsigned short ether_type;		//以太网类型
};

int main(int argc,char *argv[])
{
    pcap_t * pcap_handle;
    int ret = 0;
    char error_content[512] = "";	// 出错信息
    const unsigned char *p_packet_content = NULL; // 保存接收到的数据包的起始地址
    unsigned char *p_mac_string = NULL;			// 保存mac的地址，临时变量
    unsigned short ethernet_type = 0;			// 以太网类型
    char *p_net_interface_name = "eth0";		// 接口名字
    struct pcap_pkthdr protocol_header;
    struct ether_header *ethernet_protocol = NULL;
    struct bpf_program bpf_filter;
    char *bpf_filter_string = "arp or ip";
    bpf_u_int32 netp = 0, maskp = 0;

    //打开网络接口
    p_net_interface_name = pcap_lookupdev(error_content);
    if(NULL == p_net_interface_name)
    {
        perror("pcap_lookupdev");
		exit(-1);
    }

    pcap_handle = pcap_open_live(p_net_interface_name, 1024, 1, 0, error_content);

    //获得网络号和掩码
    ret = pcap_lookupnet(p_net_interface_name, &netp, &maskp, error_content);
    if(ret == -1)
    {
        perror("pcap_lookupnet");
        exit(-1);
    }

    //编译BPF过滤规则
    if(pcap_compile(pcap_handle, &bpf_filter, bpf_filter_string, 0, maskp) < 0)
    {
        perror("pcap_compile");
    }

    //设置过滤规则
    if(pcap_setfilter(pcap_handle,&bpf_filter) < 0)
    {
        perror("pcap_setfilter");
    }

    while(1)
    {
        //所捕获的数据包的地址
        p_packet_content = pcap_next(pcap_handle, &protocol_header);
        printf("-----------------------------------------------\n");
        printf("capture a Packet from p_net_interface_name :%s\n",p_net_interface_name);
        printf("Capture Time is :%s",ctime((const time_t *)&protocol_header.ts.tv_sec));
        printf("Packet Lenght is :%d\n",protocol_header.len);
        //分析以太网中的 源mac、目的mac
        ethernet_protocol = (struct ether_header *)p_packet_content;  //以太网帧头部
        p_mac_string = (unsigned char *)ethernet_protocol->ether_shost;//获取源mac
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
                printf("The network layer unknow!!!\n");
                break;
        }
    }
    pcap_close(pcap_handle);
    return 0;
}
