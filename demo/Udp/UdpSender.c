#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "soft_socket_pub.h"

INT main(void)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    INT   iClientSock = 0;
    CHAR  szReadMsg[128] = {0};
    SOFT_SOCKET_s *pstUdpSocket = NULL;
    struct sockaddr_in clientAddr;
    SOFT_SOCKET_s stUdpSocket;

    memset(&stUdpSocket, 0, sizeof(stUdpSocket));
    stUdpSocket.enConType = SOCKET_TYPE_UDP;
    stUdpSocket.usPort = 8888;
    stUdpSocket.ulListen = 5; 
    ulRet = soft_socket_server_init(&stUdpSocket);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] soft_socket_init Ret = %lu\n",__LINE__, ulRet);
        return -1;
    }

    while (1)
    {
        memset(szReadMsg, 0, sizeof(szReadMsg));
        if (soft_socket_udp_read(stUdpSocket.iSockId, sizeof(szReadMsg), szReadMsg, &clientAddr) < 0)
        {
            continue;
        }

        printf("%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), szReadMsg);
        
        soft_socket_udp_write(stUdpSocket.iSockId, &clientAddr, szReadMsg, strlen(szReadMsg) + 1);
    }

    soft_socket_destory(&stUdpSocket);
    
    return 0;
}

