/*
 *htonl(0x01020304) = 0x04030201 
 *htons(0x0102) = 0x0201 
 */
#include <stdio.h> 
#include <arpa/inet.h>

int main(int argc, char *argv[]) 
{ 
    int a = 0x01020304;
    short int b = 0x0102; 

    printf("htonl(0x%08x) = 0x%08x \n",a,htonl(a)); 
    printf("htons(0x%04x) = 0x%04x \n",b,htons(b)); 

    return 0; 
}
