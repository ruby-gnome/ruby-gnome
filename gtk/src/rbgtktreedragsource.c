/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreedragsource.c -

  $Author: mutoh $

  $Date: 2003/07/11 19:39:08 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

void
Init_gtk_treedragsource()
{
    G_DEF_INTERFACE(GTK_TYPE_TREE_DRAG_SOURCE, "TreeDragSource", mGtk);
}
