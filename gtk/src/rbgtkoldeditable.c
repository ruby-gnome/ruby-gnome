/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkoldeditable.c -

  $Author: mutoh $
  $Date: 2002/09/10 17:43:25 $

  Copyright (C) 2002 Masahiro Sakai

************************************************/

#include "global.h"

void 
Init_gtk_oldeditable()  /* GTK_DISABLE_DEPRECATED */
{
#ifndef GTK_DISABLE_DEPRECATED
    VALUE gOldEditable = G_DEF_CLASS(GTK_TYPE_OLD_EDITABLE, "OldEditable", mGtk);
#endif
}
