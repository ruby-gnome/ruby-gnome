/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkoldeditable.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2002 Masahiro Sakai

************************************************/

#include "global.h"

void Init_gtk_oldeditable()
{
#ifndef GTK_DISABLE_DEPRECATED
    VALUE gOldEditable = G_DEF_CLASS(GTK_TYPE_OLD_EDITABLE, "OldEditable", mGtk);

    rb_include_module(gOldEditable, mEditable);

#ifdef GTK_ENABLE_BROKEN
    Init_gtk_text();
#endif
#endif
}
