#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "soft_types.h"
#include "config.h"

#if defined(CONFIG_COMMON_LINUX_MSG_QUEUE)
#include "linux_msg/linux_msgQueue.h"
#endif

INT main(INT argc, CHAR **args)
{
    UCHAR aucRecv[10] = {0};
    ULONG ulRet = PUBLIC_STATUS_SUCCEED;
    INT iProj_id = 0;
    LINUX_MSG_QUEUE_st stQueue;

    FUNCTION_UNUSED_PARA(argc);
    FUNCTION_UNUSED_PARA(args);

    LINUX_MSG_QUEUE_init(&stQueue);

    /* Projid [0 ~ 255] */
    iProj_id = getpid() % 256;
    printf("[%s] pid %d\n", __FUNCTION__, iProj_id);

    ulRet = stQueue.create(iProj_id, &stQueue);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%s %d] create fail, status %lu\n", __FUNCTION__, __LINE__, ulRet);
        return -1;
    }

    ulRet = stQueue.send(&stQueue, 123, "hello", 5);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%s %d] send fail, status %lu\n", __FUNCTION__, __LINE__, ulRet);
        return -1;
    }

    ulRet = stQueue.recv(&stQueue, 123, 5, aucRecv);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%s %d] recv fail, status %lu\n", __FUNCTION__, __LINE__, ulRet);
        return -1;
    }

    printf("[%s %d] aucRecv = [%s]\n", __FUNCTION__, __LINE__, aucRecv);

    ulRet = stQueue.RemoveID(&stQueue);
    if (ulRet != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%s %d] RemoveID fail, status %lu\n", __FUNCTION__, __LINE__, ulRet);
        return -1;
    }

    return 0;
}

