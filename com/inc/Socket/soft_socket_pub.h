#ifndef __SOFT_SOCKET_PUB_H__
#define __SOFT_SOCKET_PUB_H__

#include "soft_types.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef enum _SOCKET_TYPE_e_
{
    SOCKET_TYPE_TCP = 0,
    SOCKET_TYPE_UDP,
}SOCKET_TYPE_e;

typedef struct _SOFT_SOCKET_s_
{
    INT     iSockId;
    USHORT  usPort;
    ULONG   ulListen;
    CHAR    szIpAddr[COMMON_STRING_LEN_16];
    SOCKET_TYPE_e enConType;
    struct sockaddr_in stServer;
}SOFT_SOCKET_s;

extern ULONG soft_socket_server_init(INOUT SOFT_SOCKET_s *pstSocket);
extern ULONG soft_socket_client_init(INOUT SOFT_SOCKET_s *pstSocket);
extern INT   soft_socket_tcp_read(IN INT iSockId, OUT CHAR *pcReadBuf, IN ULONG ulReadLen);
extern INT   soft_socket_tcp_write(IN INT iSockId, IN CHAR *pcSendBuf, IN ULONG ulSendLen);
extern INT   soft_socket_udp_read(IN INT iSockId, IN ULONG ulReadLen, OUT CHAR *pcReadBuf, OUT struct sockaddr_in *pstAddr);
extern INT   soft_socket_udp_write(IN INT iSockId, IN struct sockaddr_in *pstAddr, IN CHAR *pcSendBuf, IN ULONG ulSendLen);
extern VOID  soft_socket_destory(INOUT SOFT_SOCKET_s *pstSocket);
extern INT   soft_socket_client_connect(INOUT SOFT_SOCKET_s *pstSocket);
extern INT   soft_socket_accpet(IN SOFT_SOCKET_s *pstSocket, OUT struct sockaddr_in *pstClient);

#endif

