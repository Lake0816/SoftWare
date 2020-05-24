#ifndef __SOFT_THREAD_POOL_H__
#define __SOFT_THREAD_POOL_H__

typedef VOID *(*PF_CALL_BACK)(VOID *);

/* for debug */
typedef struct _SOFT_MODULE_ID_MAP_s_
{
    UINT            uModuleID;
    CHAR            szModuleName[COMMON_STRING_LEN_32];
}SOFT_MODULE_MSG_ID_MAP_s;

/* msg body */
typedef struct _SOFT_THREAD_POOL_MSG_s_
{
    UINT            uSrcModuleID;
    UINT            uDstModuleID;
    UINT            uMsgID;     /* every thread Msg ID begin with 0 */
    UINT            uMsgLen;
    UCHAR           *pucMsgData;
}SOFT_THREAD_POOL_MSG_s;

/* contains: ID function function_name */
typedef struct _SOFT_THREAD_POOL_MSG_ID_MAP_s_
{
    UINT            uMsgID;     /* every thread Msg ID begin with 0 */
    PF_CALL_BACK    pfFunction;
    CHAR            szMsgName[COMMON_STRING_LEN_16];
}SOFT_THREAD_POOL_MSG_ID_MAP_s;

/* Sender Malloc, Execl Free */
typedef struct _SOFT_THREAD_POOL_TASK_s_
{
    PF_CALL_BACK                        pfFunction;
    void                                *argv;
    struct _SOFT_THREAD_POOL_TASK_s_    *pstNext;
}SOFT_THREAD_POOL_TASK_s;

typedef struct _SOFT_THREAD_POOL_s_
{
    pthread_mutex_t                     pool_mutex;
    pthread_cond_t                      pool_cond;
    SOFT_THREAD_POOL_TASK_s             *pstFirst;
    SOFT_THREAD_POOL_TASK_s             *pstLast;
    UINT                                uThreadTotal;
    UINT                                uMaxThreads;
    UINT                                uIdleThreads;
    BOOL_T                              bExitThreadPool;
}SOFT_THREAD_POOL_s;

/********************************
 创建线程池
 增加处理线程
 增加任务
 删除处理线程
 销毁线程池
*********************************/

#endif
