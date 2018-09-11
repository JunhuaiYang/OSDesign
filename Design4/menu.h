/***********************************************************************
 * @file menu.h
     MENU
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __MENU_h__
#define __MENU_h__

#include "MainWindow.h"
#include "process.h"
#include "cpu.h"
#include "memory.h"
#include "module.h"

void CreateMenuBar(GtkWidget *vbox);
void MenuShutDown(GtkWidget *widget,gpointer data);
void MenuReboot(GtkWidget *widget,gpointer data);
void ReFreshAll(GtkWidget *widget,gpointer data);
void StopAll(GtkWidget *widget,gpointer data);
void ContinueAll(GtkWidget *widget,gpointer data);
void Help(GtkWidget *widget,gpointer data);
void About(GtkWidget *widget,gpointer data);

#endif // __MENU_h__
