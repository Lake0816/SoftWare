#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    struct hostent *host;
    char hostname[] = "www.163.com";
    struct in_addr in;
    struct sockaddr_in addr_in;

    host = gethostbyname(hostname);
    if (host == NULL)
    {
        printf("Get host ip failed!\n");
        return -1;
    }

    memcpy(&addr_in.sin_addr.s_addr,host->h_addr,4);
    in.s_addr = addr_in.sin_addr.s_addr;
    printf("Domain name : %s \n",hostname);
    printf("IP length is: %d \n",host->h_length);
    printf("Type : %d \n",host->h_addrtype);
    printf("IP : %s \n",inet_ntoa(in));

    return 0;
}

