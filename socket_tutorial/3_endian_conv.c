#include <arpa/inet.h>
#include <stdio.h>

int main(void)
{
    unsigned short int host_port = 0x1234;
    unsigned short int net_port;

    unsigned long int host_addr = 0x12345678;
    unsigned long int net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host port: %#x\n", host_port);
    printf("Network port: %#x\n", host_port);
    printf("Host address: %#x\n", host_port);
    printf("Network address: %#x\n", host_port);
    return (0);
}