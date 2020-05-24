#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soft_types.h"
#include "linux_msg/linux_msgQueue.h"

ULONG LINUX_MSG_QUEUE_create(IN INT iProID, INOUT LINUX_MSG_QUEUE_st *pstQueue);
ULONG LINUX_MSG_QUEUE_delete(INOUT LINUX_MSG_QUEUE_st *pstQueue);
ULONG LINUX_MSG_QUEUE_send(IN LINUX_MSG_QUEUE_st *pstQueue, IN LONG lType, IN VOID *pbuff, IN ULONG ulSize);
ULONG LINUX_MSG_QUEUE_recv(IN LINUX_MSG_QUEUE_st *pstQueue, IN LONG lType, IN ULONG ulSize, OUT VOID *pbuff);

/*************************************************
Function:     LINUX_MSG_QUEUE_init
Description:  初始化消息队列结构体
Input:        INOUT LINUX_MSG_QUEUE_st *pstQueue
Output:       INOUT LINUX_MSG_QUEUE_st *pstQueue
Return:       无
Author:       liuke
Date:         2019-01-17
Change: 
*************************************************/
VOID LINUX_MSG_QUEUE_init(INOUT LINUX_MSG_QUEUE_st *pstQueue)
{
    memset(pstQueue, 0, sizeof(*pstQueue));

    pstQueue->create = LINUX_MSG_QUEUE_create;
    pstQueue->send = LINUX_MSG_QUEUE_send;
    pstQueue->recv = LINUX_MSG_QUEUE_recv;
    pstQueue->RemoveID = LINUX_MSG_QUEUE_delete;

    return;
}

/*************************************************
Function:     LINUX_MSG_QUEUE_create
Description:  创建消息队列
Input:        IN CHAR cProID
              INOUT LINUX_MSG_QUEUE_st *pstQueue
Output:       INOUT LINUX_MSG_QUEUE_st *pstQueue
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-17
Change: 
*************************************************/
ULONG LINUX_MSG_QUEUE_create(IN INT iProID, INOUT LINUX_MSG_QUEUE_st *pstQueue)
{
    key_t key = 0;

    key = ftok("./", iProID);
    if (key == -1)
    {
        perror("[LINUX_MSG_QUEUE_create] ftok ");
        return PUBLIC_STATUS_COMMON_FAIL_FTOK;
    }
    
    pstQueue->iMsgQueueId = msgget(key, IPC_CREAT |0666| IPC_EXCL);
    if (pstQueue->iMsgQueueId < 0)
    {
        printf("[%s %d] key %d exist!\n", __FUNCTION__, __LINE__, key);
        pstQueue->iMsgQueueId = msgget(key, 0);
        if (pstQueue->iMsgQueueId < 0)
        {
            perror("msgget ");
            return PUBLIC_STATUS_SYSTEM_V_FAIL_MSGGET;
        }    
    }

    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************
Function:     LINUX_MSG_QUEUE_recv
Description:  消息队列发送
Input:        IN LINUX_MSG_QUEUE_send *pstQueue
              IN LONG lType
              IN UCHAR *pucbuff
              IN ULONG ulSize
Output:       无
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-17
Change: 
*************************************************/
ULONG LINUX_MSG_QUEUE_send(IN LINUX_MSG_QUEUE_st *pstQueue, IN LONG lType, 
                                   IN VOID *pbuff, IN ULONG ulSize)
{
    ULONG ulSendLen = 0;
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    LINUX_QUEUE_ITEM_st *pstSendItem;

    pstSendItem = (LINUX_QUEUE_ITEM_st *)calloc(1, sizeof(*pstSendItem) + ulSize);
    if (pstSendItem == NULL)
    {
        perror("calloc ");
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_SEND;
        return ulRet;
    }

    pstSendItem->lSendType = lType;
    pstSendItem->stData.ulLen = ulSize;

    memcpy(pstSendItem->stData.aucQueueData, pbuff, ulSize);
    ulSendLen = sizeof(pstSendItem->stData) + pstSendItem->stData.ulLen;

    if (msgsnd(pstQueue->iMsgQueueId, pstSendItem, ulSendLen, IPC_NOWAIT) < 0)
    {
        perror("LINUX_MSG_QUEUE_send ");
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_SEND;
    }

    free(pstSendItem);

    return ulRet;
}

/*************************************************
Function:     LINUX_MSG_QUEUE_recv
Description:  消息队列读取
Input:        IN LINUX_MSG_QUEUE_st *pstQueue
              IN LONG lType
Output:       OUT UCHAR *pucbuff
              OUT ULONG ulSize
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-17
Change: 
*************************************************/
ULONG LINUX_MSG_QUEUE_recv(IN LINUX_MSG_QUEUE_st *pstQueue, IN LONG lType, 
                                   IN ULONG ulSize, OUT VOID *pbuff)
{
    ULONG ulRecvLen = 0;
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    LINUX_QUEUE_ITEM_st *pstSendItem;

    pstSendItem = (LINUX_QUEUE_ITEM_st *)calloc(1, sizeof(*pstSendItem) + ulSize);
    if (pstSendItem == NULL)
    {
        perror("calloc ");
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_SEND;
        return ulRet;
    }
    
    ulRecvLen = sizeof(pstSendItem->stData) + ulSize;

    if (msgrcv(pstQueue->iMsgQueueId, pstSendItem, ulRecvLen, lType, IPC_NOWAIT) < 0)
    {
        perror("LINUX_MSG_QUEUE_recv ");
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_RECV;
        free(pstSendItem);
        return ulRet;
    }

    memcpy(pbuff, pstSendItem->stData.aucQueueData, ulSize);
    free(pstSendItem);

    return ulRet;
}

/*************************************************
Function:     LINUX_MSG_QUEUE_delete
Description:  删除消息队列
Input:        INOUT LINUX_SHM_st *pstQueue
Output:       INOUT LINUX_SHM_st *pstQueue
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-17
Change: 
*************************************************/
ULONG LINUX_MSG_QUEUE_delete(INOUT LINUX_MSG_QUEUE_st *pstQueue)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;

    if(msgctl(pstQueue->iMsgQueueId, IPC_RMID, NULL) != 0)
    {
        perror("LINUX_MSG_QUEUE_delete ");        
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_RMID;
    }

    return ulRet;
}

