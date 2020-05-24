#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "soft_types.h"
#include "Socket/soft_socket_pub.h"

INT main(void)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    CHAR  szReadMsg[128] = {0};
    SOFT_SOCKET_s stTcpSocket;

    memset(&stTcpSocket, 0, sizeof(stTcpSocket));
    snprintf(stTcpSocket.szIpAddr, sizeof(stTcpSocket.szIpAddr), "127.0.0.1");
    stTcpSocket.usPort = 8888;
    stTcpSocket.enConType = SOCKET_TYPE_TCP;
    stTcpSocket.ulListen = 0;
    ulRet = soft_socket_client_init(&stTcpSocket);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] soft_socket_init Ret = %lu\n",__LINE__, ulRet);
        return -1;
    }

    if (soft_socket_client_connect(&stTcpSocket) < 0)
    {
        printf("[%d] soft_socket_client_connect fail\n",__LINE__);
        return -1;
    }
        
    while (1)
    {
        scanf("%s", szReadMsg);
        if (soft_socket_tcp_write(stTcpSocket.iSockId, szReadMsg, strlen(szReadMsg) + 1) < 0)
        {
            printf("Send to Server Fail\n");
            continue;
        }

        memset(szReadMsg, 0, sizeof(szReadMsg));
       	if (soft_socket_tcp_read(stTcpSocket.iSockId, szReadMsg, sizeof(szReadMsg)) <= 0)
        {
            perror("soft socket_tcp_read ");
            continue;
        }

        printf("Client received: %s\n", szReadMsg);
    }

    soft_socket_destory(&stTcpSocket);
    
    return 0;
}

