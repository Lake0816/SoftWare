#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "soft_types.h"
#include "config.h"

#if defined(CONFIG_COMMON_LINUX_SEM)
#include "linux_sem/linux_sem.h"
#endif

INT main(INT argc, CHAR **args)
{
    INT iProj_id = 0;
    INT iPid = 0;
    LINUX_SEM_st stSem = {0};
    
    FUNCTION_UNUSED_PARA(argc);
    FUNCTION_UNUSED_PARA(args);

    LINUX_SEM_init(&stSem);

    /* Projid [0 ~ 255] */
    iProj_id = getpid() % 256;
    printf("[%s] pid %d\n", __FUNCTION__, iProj_id);

    if (stSem.create(iProj_id, 1, &stSem) != PUBLIC_STATUS_SUCCEED)
    {
        printf("[%s %d]create sem fail\n", __FILE__, __LINE__);
        return -1;
    }

    iPid = fork();
    if (iPid > 0)
    {
        while (1)
        {
            if (stSem.option(&stSem, 0, LINUX_SEM_OPTION_P) != 
                PUBLIC_STATUS_SUCCEED)
            {
                continue;
            }

            printf("B\n");
            sleep(1);
            printf("B\n");

            stSem.option(&stSem, 0, LINUX_SEM_OPTION_V);
        }
    }
    else if (iPid == 0)
    {
        while (1)
        {
            if (stSem.option(&stSem, 0, LINUX_SEM_OPTION_P) != 
                PUBLIC_STATUS_SUCCEED)
            {
                continue;
            }

            printf("A\n");
            sleep(1);
            printf("A\n");

            stSem.option(&stSem, 0, LINUX_SEM_OPTION_V);
        }
    }
    else
    {
        stSem.RemoveID(&stSem);
    }

    return 0;
}

