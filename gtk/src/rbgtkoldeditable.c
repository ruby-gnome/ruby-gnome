/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkoldeditable.c -

  $Author: sakai $
  $Date: 2002/07/30 05:37:54 $

  Copyright (C) 2002 Masahiro Sakai

************************************************/

#include "global.h"

void Init_gtk_oldeditable()
{
#ifndef GTK_DISABLE_DEPRECATED
    static RGObjClassInfo cinfo;

    gOldEditable = rb_define_class_under(mGtk, "OldEditable", gWidget);
    cinfo.klass = gOldEditable;
    cinfo.gtype = GTK_TYPE_OLD_EDITABLE;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_include_module(gOldEditable, gEditable);

#ifdef GTK_ENABLE_BROKEN
    Init_gtk_text();
#endif
#endif
}
