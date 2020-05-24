#include <sys/epoll.h>
#include "Socket/soft_epoll.h"

/*
    epoll 特点：
    优点： 将用户关系的文教描述符的事件存放在内核的事件表中，这样只需要用户空间和内核空间
           拷贝一次。支持LT（电平触发）和ET（边沿触发）
    特点：：
          a. 边缘触发，他只告诉进程哪些fd刚刚变成就绪态，并且只会通知一次
          b. epoll使用"事件"的就绪通知方式，通过epoll_ctl注册，一旦fd就绪，内核会采用callback
             方式回调激活该fd, epoll_wait便可收到通知
note：
    epoll可以理解为event poll，不同于忙轮询和无差别轮询，epoll之会把哪个流发生了怎样的I/O事件
    通知我们

相关使用方法：
    epoll_create 创建一个epoll对象，一般epollfd = epoll_create()
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socket, EPOLLIN);//注册缓冲区非空事件，即有数据流入
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socket, EPOLLOUT);//注册缓冲区非满事件，即流可以被写入
    epoll_wait(epollfd,...)等待直到注册的事件发生
*/

/*************************************************************************************
 * Function Name:   soft_socket_epoll_option
 * Description:     epoll操作
 * Parameters[in]:  INOUT fd_set *pstSelect
 * Parameters[ou]:  INOUT fd_set *pstSelect
 * ReturnValue:     PUBLIC_RETURN_STATUS
 * Author:          liuke
 * Date:            2019.2.20
 * Note:
        state contains ->
                EPOLL_CTL_ADD
                EPOLL_CTL_DEL
                EPOLL_CTL_MOD
*************************************************************************************/
INT soft_socket_epoll_option(INT epollfd, INT fd, INT state)
{
    struct epoll_event ev = {0};
    
    ev.events = state;
    ev.data.fd = fd;
    
    return epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
