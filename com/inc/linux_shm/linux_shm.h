#ifndef __LINUX_SHM_H__
#define __LINUX_SHM_H__

typedef struct _LINUX_SHM_s_
{
    /* shm ID */
    INT     iShmId;
    ULONG   ulSize;
    VOID *  pMapAddr;
    /* 映射共享内存 */
    ULONG   (*create)(INT, ULONG, struct _LINUX_SHM_s_ *);
    /* 创建共享内存，如果存在，则获取 */
    ULONG   (*write)(struct _LINUX_SHM_s_ *pstShm, UCHAR *pucbuff, ULONG ulSize);
    ULONG   (*read)(struct _LINUX_SHM_s_ *pstShm, UCHAR *pucbuff, ULONG ulSize);
    ULONG   (*detach)(struct _LINUX_SHM_s_ *pstShm);
    ULONG   (*RemoveID)(struct _LINUX_SHM_s_ *pstShm);
}LINUX_SHM_st;

extern VOID LINUX_SHM_init(LINUX_SHM_st *pstShm);

#endif

