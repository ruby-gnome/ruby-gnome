/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkoldeditable.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 2002 Masahiro Sakai

************************************************/

#include "global.h"

void Init_gtk_oldeditable()
{
#ifndef GTK_DISABLE_DEPRECATED
    VALUE gOldEditable = G_DEF_CLASS(GTK_TYPE_OLD_EDITABLE, "OldEditable", mGtk);

    rb_include_module(gOldEditable, mEditable);
#endif
}
