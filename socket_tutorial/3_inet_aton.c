#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(void)
{
    char *addr = "111.222.33.44";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("conv error");
    else
        printf("Network ordered integer addr: %#x\n",
               addr_inet.sin_addr.s_addr);
    return 0;
}