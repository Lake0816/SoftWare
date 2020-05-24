#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soft_types.h"
#include "linux_sem/linux_sem.h"

ULONG LINUX_SEM_create(IN INT iProID, IN INT iSemNum, INOUT LINUX_SEM_st *pstSem);
ULONG LINUX_SEM_option(IN struct _LINUX_SEM_s_ *pstSem, IN SHORT sID, IN LINUX_SEM_OPTION_en);
ULONG LINUX_SEM_delete(INOUT LINUX_SEM_st *pstSem);

/*************************************************
Function:     LINUX_SEM_init
Description:  初始化信号量结构体
Input:        INOUT LINUX_SEM_st *pstSem
Output:       INOUT LINUX_SEM_st *pstSem
Return:       无
Author:       liuke
Date:         2019-01-18
Change: 
*************************************************/
VOID LINUX_SEM_init(INOUT LINUX_SEM_st *pstSem)
{
    memset(pstSem, 0, sizeof(*pstSem));

    pstSem->create   = LINUX_SEM_create;
    pstSem->option   = LINUX_SEM_option;
    pstSem->RemoveID = LINUX_SEM_delete;

    return;
}

/*************************************************
Function:     LINUX_SEM_create
Description:  创建信号量
Input:        IN CHAR cProID
              INOUT LINUX_SEM_st *pstSem
Output:       INOUT LINUX_SEM_st *pstSem
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-18
Change: 
*************************************************/
ULONG LINUX_SEM_create(IN INT iProID, IN INT iSemNum, INOUT LINUX_SEM_st *pstSem)
{
    key_t key = 0;
    union semun sem_union;

    key = ftok("./", iProID);
    if (key == -1)
    {
        perror("[LINUX_MSG_QUEUE_create] ftok ");
        return PUBLIC_STATUS_COMMON_FAIL_FTOK;
    }
    
    pstSem->iSemId = semget(key, iSemNum, IPC_CREAT |0666| IPC_EXCL);
    if (pstSem->iSemId < 0)
    {
        printf("[%s %d] key %d exist!\n", __FUNCTION__, __LINE__, key);
        pstSem->iSemId = semget(key, 0, 0);
        if (pstSem->iSemId < 0)
        {
            perror("semget ");
            return PUBLIC_STATUS_SYSTEM_V_FAIL_SEMGET;
        }    
    }

    sem_union.val = 1;
    if(semctl(pstSem->iSemId, 0, SETVAL, sem_union) == -1)
    {
        perror("semctl ");
        return PUBLIC_STATUS_SYSTEM_V_FAIL_SEM_INIT;
    }

    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************
Function:     LINUX_SEM_option
Description:  信号量操作
Input:        INOUT LINUX_SEM_st *pstSem
Output:       INOUT LINUX_SEM_st *pstSem
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-18
Change: 
*************************************************/
ULONG LINUX_SEM_option(IN struct _LINUX_SEM_s_ *pstSem, IN SHORT sID, 
                              IN LINUX_SEM_OPTION_en enOption)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    struct sembuf stSemOption;
    
    stSemOption.sem_num = 0;
    stSemOption.sem_op = enOption;
    stSemOption.sem_flg = SEM_UNDO;
    if(semop(pstSem->iSemId, &stSemOption, 1) == -1)
    {
        perror("[LINUX_SEM_option] semop ");
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_SEM_OPTION;
    }

    return ulRet;
}

/*************************************************
Function:     LINUX_SEM_delete
Description:  删除信号量
Input:        INOUT LINUX_SEM_st *pstSem
Output:       INOUT LINUX_SEM_st *pstSem
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2019-01-18
Change: 
*************************************************/
ULONG LINUX_SEM_delete(INOUT LINUX_SEM_st *pstSem)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;

    if(semctl(pstSem->iSemId, 0, IPC_RMID) != 0)
    {
        perror("LINUX_MSG_QUEUE_delete ");        
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_MSG_RMID;
    }

    return ulRet;
}

