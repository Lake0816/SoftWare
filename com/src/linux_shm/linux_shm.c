#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include "soft_types.h"
#include "linux_shm/linux_shm.h"

ULONG LINUX_SHM_create(INT iProID, ULONG ulSize, LINUX_SHM_st *pstShm);
ULONG LINUX_SHM_write(LINUX_SHM_st *pstShm, UCHAR *pucbuff, ULONG ulSize);
ULONG LINUX_SHM_read(LINUX_SHM_st *pstShm, UCHAR *pucbuff, ULONG ulSize);
ULONG LINUX_SHM_detach(LINUX_SHM_st *pstShm);
ULONG LINUX_SHM_delete(LINUX_SHM_st *pstShm);

/*************************************************
Function:     LINUX_SHM_init
Description:  初始化共享内存结构体
Input:        INOUT LINUX_SHM_st *pstShm
Output:       INOUT LINUX_SHM_st *pstShm
Return:       无
Author:       liuke
Date:         2018-10-11
Change: 
*************************************************/
VOID LINUX_SHM_init(INOUT LINUX_SHM_st *pstShm)
{
    memset(pstShm, 0, sizeof(*pstShm));
    
    pstShm->create = LINUX_SHM_create;
    pstShm->write = LINUX_SHM_write;
    pstShm->read = LINUX_SHM_read;
    pstShm->detach = LINUX_SHM_detach;
    pstShm->RemoveID = LINUX_SHM_delete;
    
    return;
}

/*************************************************
Function:     LINUX_SHM_create
Description:  创建共享内存，并且进行映射
Input:        IN CHAR cProID
              IN ULONG ulSize
              INOUT LINUX_SHM_st *pstShm
Output:       INOUT LINUX_SHM_st *pstShm
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2018-10-11
Change: 
*************************************************/
ULONG LINUX_SHM_create(IN INT iProID, IN ULONG ulSize, INOUT LINUX_SHM_st *pstShm)
{
    key_t key = 0;

    /*  在UNIX系统上,它的取值是1到255 */
    key = ftok("./", iProID);
    if (key == -1)
    {
        perror("[LINUX_SHM_create] ftok ");
        return PUBLIC_STATUS_COMMON_FAIL_FTOK;
    }
    
    pstShm->iShmId = shmget(key, ulSize, IPC_CREAT |0666| IPC_EXCL);
    if (pstShm->iShmId < 0)
    {
        printf("[%s %d] key %d exist!\n", __FUNCTION__, __LINE__, key);
        pstShm->iShmId = shmget(key, 0, 0);
        if (pstShm->iShmId < 0)
        {
            return PUBLIC_STATUS_SYSTEM_V_FAIL_SHMGET;
        }    
    }
    
    // 映射共享内存，得到虚拟地址
    pstShm->pMapAddr = shmat(pstShm->iShmId, 0, 0);
    if((VOID *)-1 ==  pstShm->pMapAddr)
    {
        memset(pstShm, 0, sizeof(*pstShm));
        return PUBLIC_STATUS_SYSTEM_V_FAIL_SHMAT;
    }
    
    pstShm->ulSize = ulSize;
    
    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************
Function:     LINUX_SHM_write
Description:  将数据写入共享内存区域
Input:        IN LINUX_SHM_st *pstShm
              IN UCHAR *pucbuff
              IN ULONG ulSize
Output:       无
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2018-10-11
Change: 
Note:        写入数据长度超过定义内存长度，仅读取定义内存长度
*************************************************/
ULONG LINUX_SHM_write(IN LINUX_SHM_st *pstShm, IN UCHAR *pucbuff, IN ULONG ulSize)
{
    ULONG ulCopySize = 0;
    
    ulCopySize = (ulSize > pstShm->ulSize) ? pstShm->ulSize : ulSize;
    
    printf("[%d] CopySize %lu\n", __LINE__, ulCopySize);
    
    memcpy(pstShm->pMapAddr, (VOID *)pucbuff, ulCopySize);
    
    return ulCopySize;
}

/*************************************************
Function:     LINUX_SHM_read
Description:  将数据读出共享内存区域
Input:        IN LINUX_SHM_st *pstShm
              INOUT UCHAR *pucbuff
              IN ULONG ulSize
Output:       INOUT UCHAR *pucbuff
Return:       成功返回：读取到的数据个数
Author:       liuke
Date:         2018-10-11
Change: 
Note:         读取数据长度超过定义内存长度，仅读取定义内存长度
*************************************************/
ULONG LINUX_SHM_read(IN LINUX_SHM_st *pstShm, INOUT UCHAR *pucbuff, IN ULONG ulSize)
{
    ULONG ulCopySize = 0;
    
    ulCopySize = (ulSize > pstShm->ulSize) ? pstShm->ulSize : ulSize;
    
    memcpy(pucbuff, pstShm->pMapAddr, ulCopySize);
    
    return ulCopySize;
}

/*************************************************
Function:     LINUX_SHM_detach
Description:  取消共享内存映射
Input:        IN LINUX_SHM_st *pstShm
Output:       无
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2018-10-11
Change: 
Note:   
*************************************************/
ULONG LINUX_SHM_detach(IN LINUX_SHM_st *pstShm)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    
    // 解除映射
    if(-1 == shmdt(pstShm->pMapAddr))
    {
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_SHMDT;
    }
    
    return ulRet;
}

/*************************************************
Function:     LINUX_SHM_delete
Description:  删除共享内存
Input:        INOUT LINUX_SHM_st *pstShm
Output:       INOUT LINUX_SHM_st *pstShm
Return:       成功返回：PUBLIC_STATUS_SUCCEED，出错返回错误码
Author:       liuke
Date:         2018-10-11
Change: 
Note:   
*************************************************/
ULONG LINUX_SHM_delete(INOUT LINUX_SHM_st *pstShm)
{
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    
    if(-1 == shmctl(pstShm->iShmId, IPC_RMID, NULL))
    {
        ulRet = PUBLIC_STATUS_SYSTEM_V_FAIL_SHM_RMID;
    }
    
    memset(pstShm, 0, sizeof(*pstShm));
    
    return ulRet;
}

#if 0
INT    main(INT argc, CHAR **args)
{
    ULONG ulRet = 0;
    LINUX_SHM_st stTest;
    CHAR acArray[] = "123456789";
    CHAR read_array[20] = {0};
        
    LINUX_SHM_init(&stTest);
    printf("[%d] after LINUX_SHM_init\n", __LINE__);
    
    ulRet = stTest.create(120, 20, &stTest);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] create Err %lu\n", __LINE__, ulRet);
        return -1;
    }
    
    stTest.write(&stTest, acArray, 9);
    
    stTest.detach(&stTest);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] detach Err %lu\n", __LINE__, ulRet);
        return -1;
    }
    
    ulRet = stTest.create(120, 20, &stTest);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] create Err %lu\n", __LINE__, ulRet);
        return -1;
    }
    
    stTest.read(&stTest, read_array, 9);
    printf("[%d] read_array %s\n", __LINE__, read_array);
    
    ulRet = stTest.detach(&stTest);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] detach Err %lu\n", __LINE__, ulRet);
        return -1;
    }
    
    stTest.RemoveID(&stTest);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%d] RemoveID Err %lu\n", __LINE__, ulRet);
        return -1;
    }
    
    return 0;
}
#endif /* 示例代码 */
