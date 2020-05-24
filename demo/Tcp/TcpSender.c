#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "soft_types.h"
#include "Socket/soft_socket_pub.h"

INT main(void)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    INT   iClientSock = 0;
    CHAR  szReadMsg[128] = {0};
    SOFT_SOCKET_s stTcpSocket;
    struct sockaddr_in clientAddr;

    memset(&stTcpSocket, 0, sizeof(stTcpSocket));
    stTcpSocket.enConType = SOCKET_TYPE_TCP;
    stTcpSocket.usPort = 8888;
    stTcpSocket.ulListen = 5;
    ulRet = soft_socket_server_init(&stTcpSocket);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] soft_socket_init Ret = %lu\n",__LINE__, ulRet);
        return -1;
    }

    while (1)
    {
        iClientSock = soft_socket_accpet(&stTcpSocket, &clientAddr);
        if (iClientSock < 0)
        {
            printf("[%d] soft_socket_accpet fail\n",__LINE__);
            continue;
        }

        printf("accpet Client %s %u\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        while (1)
        {
            memset(szReadMsg, 0, sizeof(szReadMsg));
            if (soft_socket_tcp_read(iClientSock, szReadMsg, sizeof(szReadMsg)) < 0)
            {
                perror("soft_socket_tcp_read: ");
                continue;
            }
            
            soft_socket_tcp_write(iClientSock, szReadMsg, strlen(szReadMsg) + 1);
        }
    }

    soft_socket_destory(&stTcpSocket);
    
    return 0;
}

