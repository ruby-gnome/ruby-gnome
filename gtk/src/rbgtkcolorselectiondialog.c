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
cdialog_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_color_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
cdialog_get_colorsel(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->colorsel);
}

static VALUE
cdialog_get_ok_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
cdialog_get_cancel_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
cdialog_get_help_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->help_button);
}

void 
Init_gtk_color_selection_dialog()
{
    VALUE gColorSelDialog = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION_DIALOG, 
                                        "ColorSelectionDialog", mGtk);

    rb_define_method(gColorSelDialog, "initialize", cdialog_initialize, -1);
    rb_define_method(gColorSelDialog, "colorsel", cdialog_get_colorsel, 0);
    rb_define_method(gColorSelDialog, "ok_button", cdialog_get_ok_button, 0);
    rb_define_method(gColorSelDialog, "cancel_button", cdialog_get_cancel_button, 0);
    rb_define_method(gColorSelDialog, "help_button", cdialog_get_help_button, 0);
}
