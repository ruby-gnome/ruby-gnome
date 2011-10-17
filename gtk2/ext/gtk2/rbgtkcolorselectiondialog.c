/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcolorselectiondialog.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is devided from rbgtkcolorsel.c.
  rbgtkcolorsel.c:
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

#define _SELF(s) (GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(s)))

static VALUE
cdialog_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_color_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

void 
Init_gtk_color_selection_dialog(void)
{
    VALUE gColorSelDialog = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION_DIALOG, 
                                        "ColorSelectionDialog", mGtk);

    rb_define_method(gColorSelDialog, "initialize", cdialog_initialize, -1);
    /* NOTE: Backward compatibility */
    rb_define_alias(gColorSelDialog, "colorsel", "color_selection");
}
