#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include "Socket/soft_select.h"

/*
    select 特点：
    优点：  具有很好的跨平台优势
    缺点：
          1. 单个线程最大监视文件描述符个数为1024个
          2. 采用轮询的方法，线性扫描，效率低
          3. 需要维护一个存放大量fd的数组，用户空间和内核空间传递消息开销大
注意：
    从select那里仅仅知道了，有I/O事件发生了，但却并不知道是那几个流
    （可能有一个，多个，甚至全部），我们只能无差别轮询所有流，找出能读出数据，
    或者写入数据的流，对他们进行操作。
    但是使用select，我们有O(n)的无差别轮询复杂度，同时处理的流越多，没一次无
    差别轮询时间就越长。
*/


/*************************************************************************************
 * Function Name:   soft_socket_select_init
 * Description:     初始化 fd_set
 * Parameters[in]:  INOUT fd_set *pstSelect
 * Parameters[ou]:  INOUT fd_set *pstSelect
 * ReturnValue:     PUBLIC_RETURN_STATUS
 * Author:          liuke
 * Date:            2019.2.13
*************************************************************************************/
PUBLIC_RETURN_STATUS_en soft_socket_select_init(INOUT SOFT_SELECT_st *pstSelect)
{
    if (pstSelect == NULL)
    {
        return PUBLIC_STATUS_COMMON_FAIL_POINTER_NULL;
    }
    
    //用来清除描述词组set的全部位
    FD_ZERO(pstSelect);

    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************************************************
 * Function Name:   soft_socket_select
 * Description:     检查文件描述符是否满足条件
 * Parameters[in]:  IN fd_set *pstWrite
                    IN fd_set * pstRead
                    IN INT iMaxfd
                    IN struct timeval *timeout
 * Parameters[ou]: 
 * ReturnValue:     PUBLIC_RETURN_STATUS
 * Author:          liuke
 * Date:            2019.2.13
 * Note:
    timeout:
    1. NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态
    2. 时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数(文件无变化返回0，有变化返回一个正值)
    3. timeout的值大于0，这就是等待的超时时间，即select在timeout时间内阻塞，超时时间之内有
       事件到来就返回了,否则在超时后不管怎样一定返回，返回值同上述
*************************************************************************************/
PUBLIC_RETURN_STATUS_en soft_socket_select(IN SOFT_SELECT_st *pstWrite, 
                                               IN SOFT_SELECT_st * pstRead, 
                                               IN INT iMaxfd, IN struct timeval *timeout)
{
    INT iRet = 0;
    PUBLIC_RETURN_STATUS_en enStatus = PUBLIC_STATUS_SUCCEED;

    if (pstWrite == NULL && pstRead == NULL && iMaxfd <= 0)
    {
        return PUBLIC_STATUS_COMMON_FAIL_POINTER_NULL;
    }
    
    /*
        返回值：
        负值：select错误
        正值：某些文件可读写或出错
        0：等待超时，没有可读写或错误的文件
    */
    iRet = select(iMaxfd, pstRead, pstWrite, NULL, timeout);
    if (iRet < 0)
    {
        perror("soft_socket_select ");
        enStatus = PUBLIC_STATUS_SOCKET_FAIL_SELECT;
    }
    else if (iRet == 0)
    {
        enStatus = PUBLIC_STATUS_SOCKET_TIME_OUT;
    }
    else
    {
        enStatus = PUBLIC_STATUS_SOCKET_SELECT_CHECK;
    }

    return enStatus;
}

/*************************************************************************************
 * Function Name:   soft_socket_select_add_fds
 * Description:     向集合中添加文件描述符
 * Parameters[in]:  IN fd_set *pstSelect
                    IN int fd
 * Parameters[ou]: 
 * ReturnValue:     PUBLIC_RETURN_STATUS
 * Author:          liuke
 * Date:            2019.2.13
*************************************************************************************/
PUBLIC_RETURN_STATUS_en soft_socket_select_add_fds(IN SOFT_SELECT_st *pstSelect, 
                                                           IN INT fd)
{
    if (pstSelect == NULL || fd < 0)
    {
        return PUBLIC_STATUS_COMMON_FAIL_INVAILD_PARA;
    }

    //用来设置描述词组set中相关fd的位
    FD_SET(fd, pstSelect); 

    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************************************************
 * Function Name:   soft_socket_select_check_fd
 * Description:     检查文件描述符是否满足条件
 * Parameters[in]:  IN fd_set *pstSelect
                    IN INT fd
 * Parameters[ou]:  OUT BOOL_T *pbStatus
 * ReturnValue:     BOOL_T
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
PUBLIC_RETURN_STATUS_en soft_socket_select_check_fd(IN SOFT_SELECT_st *pstSelect, 
                                                            IN INT fd, OUT BOOL_T *pbStatus)
{
    if ((pstSelect == NULL) || (pbStatus == NULL) || (fd < 0))
    {
        return PUBLIC_STATUS_COMMON_FAIL_INVAILD_PARA;
    }

    *pbStatus = BOOL_FALSE;

    //用来测试描述词组set中相关fd 的位是否为真
    if (FD_ISSET(fd, pstSelect) != 0)
    {
        *pbStatus = BOOL_TRUE;
    }

    return PUBLIC_STATUS_SUCCEED;
}

/*************************************************************************************
 * Function Name:   soft_socket_select_clear_fd
 * Description:     清理被置位的文件描述符
 * Parameters[in]:  IN fd_set *pstSelect
                    IN int fd
 * Parameters[ou]: 
 * ReturnValue:     VOID
 * Author:          liuke
 * Date:            2018.12.4
*************************************************************************************/
PUBLIC_RETURN_STATUS_en soft_socket_select_clear_fd(IN SOFT_SELECT_st *pstSelect, 
                                                               IN INT fd)
{
    if (pstSelect == NULL || fd < 0)
    {
        return PUBLIC_STATUS_COMMON_FAIL_INVAILD_PARA;
    }

    //用来清除描述词组set中相关fd 的位
    FD_CLR(fd, pstSelect);

    return PUBLIC_STATUS_SUCCEED;
}


