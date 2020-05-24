#ifndef __LINUX_SEM_H__
#define __LINUX_SEM_H__

union semun {
    int                 val;      /* Value for SETVAL */
    struct semid_ds    *buf;      /* Buffer for IPC_STAT, IPC_SET */
    unsigned short     *array;  /* Array for GETALL, SETALL */
    struct seminfo     *__buf;  /* Buffer for IPC_INFO                            (Linux-specific) */
};


typedef enum _LINUX_SEM_OPTION_e_
{
    LINUX_SEM_OPTION_P = -1,
    LINUX_SEM_OPTION_V = 1,
}LINUX_SEM_OPTION_en;

typedef struct _LINUX_SEM_s_
{
    /* sem ID */
    INT    iSemId;                                
    ULONG  (*create)(IN INT iProID, IN INT iSemNum, INOUT struct _LINUX_SEM_s_  *pstSem);
    ULONG  (*option)(IN struct _LINUX_SEM_s_ *pstSem, IN SHORT sID, IN LINUX_SEM_OPTION_en);
    ULONG  (*RemoveID)(INOUT struct _LINUX_SEM_s_  *pstSem);
}LINUX_SEM_st;

extern VOID LINUX_SEM_init(INOUT LINUX_SEM_st *pstSem);

#endif

