#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Socket/soft_socket_pub.h"

/*************************************************************************************
 * Function Name:   soft_socket_check_ip
 * Description:     Check ip addr
 * Parameters[in]:  IN const char *ipaddr
 * Parameters[ou]: 
 * ReturnValue:     BOOL_T
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
BOOL_T soft_socket_check_ip(IN const char *ipaddr)
{
    CHAR szCheckIp[31] = {0};
    INT a, b, c, d;
 
    if ((sscanf(ipaddr, "%d.%d.%d.%d ", &a, &b, &c, &d) == 4) && 
         (a >= 0) && (a <= 255) && (b >= 0) && (b <= 255) && 
         (c >= 0) && (c <= 255) && (d >= 0) && (d <= 255))
    {
        sprintf(szCheckIp, "%d.%d.%d.%d", a, b, c, d); 
        if (strcmp(szCheckIp, ipaddr) == 0)
        {
            return BOOL_TRUE;
        }
    }
 
    return BOOL_FALSE;
}

/*************************************************************************************
 * Function Name:    soft_socket_tcp_server_init
 * Description:      Create TCP Server
 * Parameters[in]:   SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:   SOFT_SOCKET_s *pstSocket
 * ReturnValue:
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
static ULONG soft_socket_tcp_server_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    INT iSockOptVal = 1;
    
    if (strlen(pstSocket->szIpAddr) == 0)
    {
        pstSocket->stServer.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if (inet_pton(AF_INET, pstSocket->szIpAddr, &pstSocket->stServer.sin_addr) != 1)
        {
            return PUBLIC_STATUS_SOCKET_FAIL_IPADDR;
        }
    }

    pstSocket->stServer.sin_family = AF_INET;
    pstSocket->stServer.sin_port = htons(pstSocket->usPort);

    if((pstSocket->iSockId = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_CREAT;
    }

    if(setsockopt(pstSocket->iSockId, SOL_SOCKET, SO_REUSEADDR, (VOID *)&iSockOptVal, 
                  (socklen_t)sizeof(iSockOptVal)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_SETOPT;
    } 

    ulRet = bind(pstSocket->iSockId, (struct sockaddr *)&pstSocket->stServer,
                 sizeof(struct sockaddr));
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_BIND;
    }

    ulRet = listen(pstSocket->iSockId, pstSocket->ulListen);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_LISTEN;
    }

    return ulRet;
}

/*************************************************************************************
 * Function Name:    soft_socket_udp_server_init
 * Description:      Create UDP Server
 * Parameters[in]:   SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:   SOFT_SOCKET_s *pstSocket
 * ReturnValue:
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
static ULONG soft_socket_udp_server_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    INT iSockOptVal = 1;

    if ((pstSocket->iSockId = socket(PF_INET,SOCK_DGRAM , 0)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_CREAT;
    }

    if(setsockopt(pstSocket->iSockId, SOL_SOCKET, SO_REUSEADDR, (VOID *)&iSockOptVal, 
                   (socklen_t)sizeof(iSockOptVal)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_SETOPT;
    } 

    pstSocket->stServer.sin_family = AF_INET;
    pstSocket->stServer.sin_port=htons(pstSocket->usPort);

    if (strlen(pstSocket->szIpAddr) == 0)
    {
        pstSocket->stServer.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if (inet_pton(AF_INET, pstSocket->szIpAddr, &pstSocket->stServer.sin_addr) != 1)
        {
            return PUBLIC_STATUS_SOCKET_FAIL_IPADDR;
        }
    }

    ulRet = bind(pstSocket->iSockId, (struct sockaddr *)&pstSocket->stServer, 
                 sizeof(struct sockaddr));
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_BIND;
    }

    return ulRet;
}

/*************************************************************************************
 * Function Name:    soft_socket_server_init
 * Description:      Create Server
 * Parameters[in]:   IN CHAR *pcIpAddr
                     IN SOCKET_TYPE_e enType
                     IN USHORT usPort
                     IN ULONG ulListen
 * Parameters[ou]:   OUT SOFT_SOCKET_s **ppstSocket
 * ReturnValue:      success return: PUBLIC_STATUS_SUCCEED
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
ULONG soft_socket_server_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;

    if (strlen(pstSocket->szIpAddr) > 0)
    {
        if (soft_socket_check_ip(pstSocket->szIpAddr) == BOOL_FALSE)
        {
            return PUBLIC_STATUS_SOCKET_FAIL_IPADDR;
        }
    }

    if (pstSocket->enConType == SOCKET_TYPE_TCP)
    {
        ulRet = soft_socket_tcp_server_init(pstSocket);
        if (ulRet != PUBLIC_STATUS_SUCCEED)
        {
            return ulRet;
        }
    }
    else if (pstSocket->enConType == SOCKET_TYPE_UDP)
    {
        ulRet = soft_socket_udp_server_init(pstSocket);
        if (ulRet != PUBLIC_STATUS_SUCCEED)
        {
            return ulRet;
        }
    }
    else
    {
        return PUBLIC_STATUS_COMMON_FAIL_TYPE;
    }

    return ulRet;
}

/*************************************************************************************
 * Function Name:    soft_socket_tcp_client_init
 * Description:      Create TCP client
 * Parameters[in]:   INOUT SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:   INOUT SOFT_SOCKET_s *pstSocket
 * ReturnValue:      success return: PUBLIC_STATUS_SUCCEED
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
static ULONG soft_socket_tcp_client_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    
    if (strlen(pstSocket->szIpAddr) == 0)
    {
        pstSocket->stServer.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if (inet_pton(AF_INET, pstSocket->szIpAddr, &pstSocket->stServer.sin_addr) != 1)
        {
            return PUBLIC_STATUS_SOCKET_FAIL_IPADDR;
        }
    }

    pstSocket->stServer.sin_family = AF_INET;
    pstSocket->stServer.sin_port = htons(pstSocket->usPort);

    if((pstSocket->iSockId = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_CREAT;
    }

    return ulRet;
}

/*************************************************************************************
 * Function Name:    soft_socket_udp_client_init
 * Description:      Create UDP client
 * Parameters[in]:   INOUT SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:   INOUT SOFT_SOCKET_s *pstSocket
 * ReturnValue:      success return: PUBLIC_STATUS_SUCCEED
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
static ULONG soft_socket_udp_client_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;

    pstSocket->stServer.sin_family = AF_INET;
    pstSocket->stServer.sin_port = htons(pstSocket->usPort);

    if (strlen(pstSocket->szIpAddr) == 0)
    {
        pstSocket->stServer.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if (inet_pton(AF_INET, pstSocket->szIpAddr, &pstSocket->stServer.sin_addr) != 1)
        {
            return PUBLIC_STATUS_SOCKET_FAIL_IPADDR;
        }
    }

    if ((pstSocket->iSockId = socket(PF_INET,SOCK_DGRAM , 0)) < 0)
    {
        return PUBLIC_STATUS_SOCKET_FAIL_CREAT;
    }
    
    return ulRet;
}


/*************************************************************************************
 * Function Name:    soft_socket_client_connect
 * Description:      Connect Server
 * Parameters[in]:   INOUT SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:   INOUT SOFT_SOCKET_s *pstSocket
 * ReturnValue:      success return: 0
 * Author:           liuke
 * Date:             2018.12.4
*************************************************************************************/
INT soft_socket_client_connect(INOUT SOFT_SOCKET_s *pstSocket)
{
    return connect(pstSocket->iSockId, (struct sockaddr*)(&pstSocket->stServer), 
                   sizeof(pstSocket->stServer));
}

/*************************************************************************************
 * Function Name:   soft_socket_client_connect
 * Description:     Create client
 * Parameters[in]:  IN CHAR *pcIpAddr
                    IN SOCKET_TYPE_e enType
                    IN USHORT usPort
 * Parameters[ou]:  OUT SOFT_SOCKET_s **ppstSocket
 * ReturnValue:     success return: PUBLIC_STATUS_SUCCEED
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
ULONG soft_socket_client_init(INOUT SOFT_SOCKET_s *pstSocket)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;

    if (pstSocket->enConType == SOCKET_TYPE_TCP)
    {
        ulRet = soft_socket_tcp_client_init(pstSocket);
        if (ulRet != PUBLIC_STATUS_SUCCEED)
        {
            return ulRet;
        }
    }
    else if (pstSocket->enConType == SOCKET_TYPE_UDP)
    {
        ulRet = soft_socket_udp_client_init(pstSocket);
        if (ulRet != PUBLIC_STATUS_SUCCEED)
        {
            return ulRet;
        }
    }
    else
    {
        return PUBLIC_STATUS_COMMON_FAIL_TYPE;
    }

    return ulRet;
}

/*************************************************************************************
 * Function Name:   soft_socket_accpet
 * Description:     Accpet client
 * Parameters[in]:  IN SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:  OUT struct sockaddr_in *pstClient
 * ReturnValue:     success return: PUBLIC_STATUS_SUCCEED
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
INT soft_socket_accpet(IN SOFT_SOCKET_s *pstSocket, OUT struct sockaddr_in *pstClient)
{
    INT iAddrLen = sizeof(*pstClient);
    return accept(pstSocket->iSockId, (struct sockaddr*)&pstClient, (socklen_t *)&iAddrLen);
}

/*************************************************************************************
 * Function Name:   VOID soft_socket_destory(INOUT SOFT_SOCKET_s *pstSocket)
 * Description:     Destory socket
 * Parameters[in]:  IN SOFT_SOCKET_s *pstSocket
 * Parameters[ou]:  OUT struct sockaddr_in *pstClient
 * ReturnValue:     success return: PUBLIC_STATUS_SUCCEED
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
VOID soft_socket_destory(INOUT SOFT_SOCKET_s *pstSocket)
{
    close(pstSocket->iSockId);
    
    return;
}

/*************************************************************************************
 * Function Name:   soft_socket_tcp_write
 * Description:     Tcp write
 * Parameters[in]:  IN INT iSockId
                    IN CHAR *pcSendBuf
                    IN ULONG ulSendLen
 * Parameters[ou]:   
 * ReturnValue:     success return: write byte total number
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
INT soft_socket_tcp_write(IN INT iSockId, IN CHAR *pcSendBuf, IN ULONG ulSendLen)
{
    return send(iSockId, pcSendBuf, ulSendLen, 0);
}

/*************************************************************************************
 * Function Name:   soft_socket_tcp_read
 * Description:     Tcp read
 * Parameters[in]:  IN INT iSockId
                    IN ULONG ulReadLen
 * Parameters[ou]:  OUT CHAR *pcReadBuf
 * ReturnValue:     success return: read byte total number
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
INT soft_socket_tcp_read(IN INT iSockId, OUT CHAR *pcReadBuf, IN ULONG ulReadLen)
{
    return recv(iSockId, pcReadBuf, ulReadLen, 0);
}

/*************************************************************************************
 * Function Name:   soft_socket_udp_write
 * Description:     Udp write
 * Parameters[in]:  IN INT iSockId
                    IN struct sockaddr_in *pstAddr
                    IN ULONG ulReadLen
 * Parameters[ou]:  OUT CHAR *pcReadBuf
 * ReturnValue:     success return: write byte total number
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
INT soft_socket_udp_write(IN INT iSockId, IN struct sockaddr_in *pstAddr, 
                                  IN CHAR *pcSendBuf, IN ULONG ulSendLen)
{
    return sendto(iSockId, pcSendBuf, ulSendLen, 0, (struct sockaddr*)pstAddr, sizeof(*pstAddr));
}

/*************************************************************************************
 * Function Name:   soft_socket_udp_read
 * Description:     Udp read
 * Parameters[in]:  IN INT iSockId
                    IN ULONG ulReadLen
 * Parameters[ou]:  OUT CHAR *pcReadBuf
                    OUT struct sockaddr_in *pstAddr
 * ReturnValue:     success return: read byte total number
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
INT soft_socket_udp_read(IN INT iSockId, IN ULONG ulReadLen, OUT CHAR *pcReadBuf, 
                                 OUT struct sockaddr_in *pstAddr)
{
    INT iAddrLen = sizeof(*pstAddr);
    return recvfrom(iSockId, pcReadBuf, ulReadLen - 1, 0, (struct sockaddr *)pstAddr, 
                    (socklen_t *)&iAddrLen);
}

