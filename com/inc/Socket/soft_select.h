#ifndef __SOFT_SELECT_H__
#define __SOFT_SELECT_H__

#include "soft_types.h"
#include <sys/select.h>

typedef fd_set                          SOFT_SELECT_st;

PUBLIC_RETURN_STATUS_en soft_socket_select_init(INOUT SOFT_SELECT_st *pstSelect);
PUBLIC_RETURN_STATUS_en soft_socket_select(IN SOFT_SELECT_st *pstWrite, 
                                                  IN SOFT_SELECT_st * pstRead, 
                                                  IN INT iMaxfd, IN struct timeval *timeout);
PUBLIC_RETURN_STATUS_en soft_socket_select_add_fds(IN SOFT_SELECT_st *pstSelect, 
                                                             IN INT fd);
PUBLIC_RETURN_STATUS_en soft_socket_select_check_fd(IN SOFT_SELECT_st *pstSelect, 
                                                               IN INT fd, OUT BOOL_T *pbStatus);
PUBLIC_RETURN_STATUS_en soft_socket_select_clear_fd(IN SOFT_SELECT_st *pstSelect, 
                                                               IN INT fd);

#endif

