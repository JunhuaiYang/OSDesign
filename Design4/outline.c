/***********************************************************************
 * @file outline.c
     OUTLINE
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "outline.h"

void createCPUPage(GtkWidget* notebook)
{
    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), NULL, gtk_label_new(
			"CPU信息"));
}

