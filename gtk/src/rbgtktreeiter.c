/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeiter.c -

  $Author: mutoh $
  $Date: 2002/10/02 15:39:06 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) ((GtkTreeIter*)RVAL2BOXED(s, GTK_TYPE_TREE_ITER))

void 
Init_gtk_treeiter()
{
    VALUE gTreeiter = G_DEF_CLASS(GTK_TYPE_TREE_ITER, "TreeIter", mGtk);
}
