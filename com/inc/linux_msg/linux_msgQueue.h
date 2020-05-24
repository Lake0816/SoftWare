#ifndef __LINUX_MSG_QUEUE_H__
#define __LINUX_MSG_QUEUE_H__

#pragma pack(1)
typedef struct _LINUX_QUEUE_DATA_s_
{
    ULONG ulLen;
    UCHAR aucQueueData[0];
}LINUX_QUEUE_DATA_st;

typedef struct _LINUX_QUEUE_ITEM_s_
{
    long lSendType;

    /* send Data */
    LINUX_QUEUE_DATA_st stData;
}LINUX_QUEUE_ITEM_st;
#pragma pack()

typedef struct _LINUX_MSG_QUEUE_s_
{
    /* shm ID */
    INT   iMsgQueueId;
    ULONG (*create)(INT iProID, struct _LINUX_MSG_QUEUE_s_ *pstQueue);
    ULONG (*send)(struct _LINUX_MSG_QUEUE_s_ *pstQueue, LONG lType, VOID *pucbuff, ULONG ulSize);
    ULONG (*recv)(struct _LINUX_MSG_QUEUE_s_ *pstQueue, LONG lType, ULONG ulSize, VOID *pucbuff);
    ULONG (*RemoveID)(struct _LINUX_MSG_QUEUE_s_ *pstQueue);
}LINUX_MSG_QUEUE_st;

extern VOID LINUX_MSG_QUEUE_init(LINUX_MSG_QUEUE_st *pstMsg);

#endif
