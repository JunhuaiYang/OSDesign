/***********************************************************************
 * @file outline.h
     OUTLINE
 * @brief   header file
***********************************************************************/
#ifndef __OUTLINE_h__
#define __OUTLINE_h__

#include "MainWindow.h"

typedef struct outline_info
{
    char hostname[30];
    char os[20];
    char version[30];
    char kernel[50];
    char cpu[50];
    char memory[20];

} OUTLINE_INFO, *p_outline_info;

void CreateOutline(GtkWidget* notebook);
void GetOutlineInfo(p_outline_info oinfo);



#endif // __OUTLINE_h__

