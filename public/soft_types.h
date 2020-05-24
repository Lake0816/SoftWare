#ifndef __SOFT_TYPES_H__
#define __SOFT_TYPES_H__

#define BOOL_TRUE                               1UL
#define BOOL_FALSE                              0UL

typedef char                                    CHAR;
typedef unsigned char                           UCHAR;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;
typedef int                                     INT;
typedef unsigned int                            UINT;
typedef long                                    LONG;
typedef unsigned long                           ULONG;
typedef long long                               DLONG;
typedef unsigned long long                      DULONG;
typedef float                                   FLOAT;
typedef double                                  DOUBLE;
typedef ULONG                                   BOOL_T;
typedef void                                    VOID;

/*define function paramater in/out type */
#define IN
#define OUT
#define INOUT
#define FUNCTION_UNUSED_PARA(x)                 (VOID)x;

/* define string len */
#define COMMON_STRING_LEN_8                     8
#define COMMON_STRING_LEN_16                    16
#define COMMON_STRING_LEN_32                    32
#define COMMON_STRING_LEN_64                    64
#define COMMON_STRING_LEN_128                   128
#define COMMON_STRING_LEN_256                   256
#define COMMON_STRING_LEN_512                   512
#define COMMON_STRING_LEN_1024                  1024
#define COMMON_STRING_LEN_2048                  2048
#define COMMON_STRING_LEN_4096                  4096
#define COMMON_STRING_LEN_8192                  8192

typedef enum _PUBLIC_RETURN_STATUS_E_
{
    /* common fail: 0x00 ~ 0x10 */
    PUBLIC_STATUS_SUCCEED                       = 0x00000000,
    PUBLIC_STATUS_COMMON_FAIL_MALLOC,
    PUBLIC_STATUS_COMMON_FAIL_CALLOC,
    PUBLIC_STATUS_COMMON_FAIL_EMPTY_STRING,
    PUBLIC_STATUS_COMMON_FAIL_POINTER_NULL,
    PUBLIC_STATUS_COMMON_FAIL_INVAILD_PARA,
    PUBLIC_STATUS_COMMON_FAIL_FTOK,

    /* File error: 0x10 ~ 0x20 */
    PUBLIC_STATUS_FILE_FAIL_OPEN                = 0x00000010,
    PUBLIC_STATUS_FILE_FAIL_CLOSE,
    PUBLIC_STATUS_FILE_FAIL_SEEK,
    PUBLIC_STATUS_FILE_FAIL_WRITE,
    PUBLIC_STATUS_FILE_FAIL_READ,

    /* socket error: 0x20 ~ 0x30 */
    PUBLIC_STATUS_COMMON_FAIL_TYPE              = 0x00000020,
    PUBLIC_STATUS_SOCKET_FAIL_CREAT,
    PUBLIC_STATUS_SOCKET_FAIL_CLOSE,
    PUBLIC_STATUS_SOCKET_FAIL_WRITE,
    PUBLIC_STATUS_SOCKET_FAIL_READ,
    PUBLIC_STATUS_SOCKET_FAIL_CONNECT,
    PUBLIC_STATUS_SOCKET_FAIL_BIND,
    PUBLIC_STATUS_SOCKET_FAIL_LISTEN,
    PUBLIC_STATUS_SOCKET_FAIL_ACCEPT,
    PUBLIC_STATUS_SOCKET_FAIL_IPADDR,
    PUBLIC_STATUS_SOCKET_FAIL_SETOPT,

    /* system v error: 0x30 ~ 0x40 */
    PUBLIC_STATUS_SYSTEM_V_FAIL_SHMGET          = 0x00000030,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SHMAT,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SHMDT,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SHM_RMID,
    PUBLIC_STATUS_SYSTEM_V_FAIL_MSGGET,
    PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_SEND,
    PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_RECV,
    PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_RMID,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SEMGET,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SEM_INIT,
    PUBLIC_STATUS_SYSTEM_V_FAIL_SEM_OPTION,

    /* IO multi error: 0x40 ~ 0x50 */
    PUBLIC_STATUS_SOCKET_SELECT_CHECK           = 0x00000040,
    PUBLIC_STATUS_SOCKET_TIME_OUT,
    PUBLIC_STATUS_SOCKET_FAIL_SELECT,

    PUBLIC_STATUS_END                           = 0xFFFFFFFF,
}PUBLIC_RETURN_STATUS_en;

#endif

