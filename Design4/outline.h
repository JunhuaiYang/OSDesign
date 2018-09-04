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
    char hostname[50];
    char os[50];
    char version[50];
    char kernel[50];
    char cpu[50];
    char memory[50];

} OUTLINE_INFO, *p_outline_info;

void CreateOutline(GtkWidget* notebook);
void GetOutlineInfo(p_outline_info oinfo);
void ShowInfo(GtkWidget *vbox, OUTLINE_INFO oinfo);
void ShowLabel(GtkWidget *vbox, const char *text);



#endif // __OUTLINE_h__

