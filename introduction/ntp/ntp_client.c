#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
 
#define NTPPORT 123
#define TIMEPORT 37
#define NTPV1 "NTP/V1"
#define NTPV2 "NTP/V2"
#define NTPV3 "NTP/V3"
#define NTPV4 "NTP/V4"
#define TIME "TIME/UDP"
 
double SecondBef1970;
struct sockaddr_in sin;
struct addrinfo hints, *res=NULL;
int rc,sk;
char Protocol[32];
 
struct NTPPacket
{
    char Leap_Ver_Mode;
    /* client=0 */
    char Startum;
    char Poll;
    char Precision;
    double RootDelay;
    double Dispersion;
    char RefIdentifier[4];
    char RefTimeStamp[8];
    char OriTimeStamp[8];
    char RecvTimeStamp[8];
    char TransTimeStamp[8];
};
 
int ConstructPacket(char *Packet);
long GetSecondFrom1900(int End);
long GetNtpTime(int sk, struct addrinfo *res);
 
int main()
{
    memset(&hints,0,sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
 
    /* 调用 getaddrinfo 函数，获取地址信息 */
    rc = getaddrinfo("200.205.253.254", "123", &hints, &res);
    if (rc != 0) 
    {
        perror("getaddrinfo");
        return 0;
    }
 
    sk = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sk < 0)
    {
        perror("socket");
    }
    else
    {
        printf("socket success!\n");
    }
 
    /* 调用取得 NTP 时间函数 */
    GetNtpTime(sk, res);
}
 
/* 构建 NTP 协议包 */
int ConstructPacket(char *Packet)
{
    char Version = 1;
    long SecondFrom1900;
    long Zero = 0;
    int Port;
    time_t timer;
    
    strcpy(Protocol,NTPV1);
    /* 判断协议版本 */
    if(strcmp(Protocol, NTPV1) || strcmp(Protocol,NTPV2) || strcmp(Protocol, NTPV3) || strcmp(Protocol, NTPV4))
    {
        Port = NTPPORT;
        Version = Protocol[6] - 0x30;
        Packet[0] = (Version << 3) | 3; //LI--Version--Mode
        Packet[1] = 0;                  //Startum
        Packet[2] = 0;                  //Poll interval
        Packet[3] = 0;                  //Precision
 
        /* 包括 Root delay、Root disperse 和 Ref Indentifier */
        memset(&Packet[4], 0, 12);
        /* 包括 Ref timestamp、Ori timastamp 和 Receive Timestamp */
        memset(&Packet[16], 0, 24);
 
        time(&timer);
        SecondFrom1900 = SecondBef1970 + (long)timer;
        SecondFrom1900 = htonl(SecondFrom1900);
 
        memcpy(&Packet[40], &SecondFrom1900, 4);
        memcpy(&Packet[44], &Zero, 4);
        return 48;
    }
    else    // time/udp
    {
        Port = TIMEPORT;
        memset(Packet, 0, 4);
        return 4;
    }
    return 0;
}
 
/* 计算从 1900 年到现在一共有多少秒 */
long GetSecondFrom1900(int End)
{
    int Ordinal = 0;
    int Run = 0;
    long Result;
    int i;
 
    for(i = 1900; i < End; i++)
    {
        if(((i%4 == 0) && (i%100 != 0)) || (i%400 == 0)) 
        {
            Run++;
        }
        else
        {
            Ordinal++;
        }
    }
    Result = (Run * 366 + Ordinal * 365) * 24 * 3600;
    return Result;
}
 
/*获取 NTP 时间*/
long GetNtpTime(int sk, struct addrinfo *res)
{
    char Content[256];
    int PacketLen;
    fd_set PendingData;
    struct timeval BlockTime;
    int FromLen;
    int Count = 0;
    int result, i;
    int re;
    struct NTPPacket RetTime;
 
    PacketLen = ConstructPacket(Content);
    if(!PacketLen)
    {
        return 0;
    }
    /* 客户端给服务器端发送 NTP 协议数据包 */
    if((result = sendto(sk, Content, PacketLen, 0, res->ai_addr, res->ai_addrlen)) < 0)
    {
        perror("sendto");
    }
    else
    {
        printf("sendto success result=%d \n",result);
    }
    for(i = 0; i < 5; i++)
    {
        printf("in for\n");
        /* 调用 select 函数，并设定超时时间为 1s */
        FD_ZERO(&PendingData);
        FD_SET(sk, &PendingData);
        BlockTime.tv_sec = 1;
        BlockTime.tv_usec = 0;
 
        if(select(sk+1, &PendingData, NULL, NULL, &BlockTime) > 0)
        {
            FromLen = sizeof(sin);
            /* 接收服务器端的信息 */
            if((Count = recvfrom(sk, Content, 256, 0, res->ai_addr, &(res->ai_addrlen))) < 0)
            {
                perror("recvfrom");
            }
            else
            {
                printf("recvfrom success,Count=%d \n",Count);
            }
 
            if(Protocol == TIME)
            {
                memcpy(RetTime.TransTimeStamp, Content, 4);
 
                return 1;
            }
            else if(Count >= 48 && Protocol != TIME)
            {
                RetTime.Leap_Ver_Mode = Content[0];
                RetTime.Startum = Content[1];
                RetTime.Poll = Content[2];
                RetTime.Precision = Content[3];
 
                memcpy((void *)&RetTime.RootDelay, &Content[4], 4);
                memcpy((void *)&RetTime.Dispersion, &Content[8], 4);
                memcpy((void *)RetTime.RefIdentifier, &Content[12], 4);
 
                memcpy((void *)RetTime.RefTimeStamp, &Content[16], 8);
                memcpy((void *)RetTime.OriTimeStamp, &Content[24], 8);
                memcpy((void *)RetTime.RecvTimeStamp, &Content[32], 8);
                memcpy((void *)RetTime.TransTimeStamp, &Content[40], 8);
 
                return 1;
            }
 
        }
    }
    close(sk);
    return 0;
}