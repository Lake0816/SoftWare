#ifndef __SOFT_LIST_H__
#define __SOFT_LIST_H__

typedef enum _SOFT_LIST_TYPE_E_
{
    SOFT_LIST_TYPE_SINGLE_LIST,
    SOFT_LIST_TYPE_DUAL_LIST,
}SOFT_LIST_TYPE_e;

typedef enum _SOFT_LIST_TYPE_INSERT_E_
{
    SOFT_LIST_TYPE_INSERT_L_HEAD,			/* insert list head */
    SOFT_LIST_TYPE_INSERT_L_TAIL,			/* insert list tail */
    SOFT_LIST_TYPE_INSERT_S_PREV,			/* insert search node previous */
    SOFT_LIST_TYPE_INSERT_S_NEXT,			/* insert search node next */
}SOFT_LIST_TYPE_INSERT_e;

#endif
