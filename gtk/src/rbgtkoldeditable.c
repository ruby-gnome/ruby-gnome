/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkoldeditable.c -

  $Author: sakai $
  $Date: 2002/08/05 17:52:00 $

  Copyright (C) 2002 Masahiro Sakai

************************************************/

#include "global.h"

void Init_gtk_oldeditable()
{
#ifndef GTK_DISABLE_DEPRECATED
    VALUE gOldEditable = G_DEF_CLASS(GTK_TYPE_OLD_EDITABLE, "OldEditable", mGtk);
#endif
}
