/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdialog.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
dialog_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_dialog_new());
    return Qnil;
}

static VALUE
dialog_vbox(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_DIALOG(RVAL2GOBJ(self))->vbox);
}

static VALUE
dialog_action_area(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_DIALOG(RVAL2GOBJ(self))->action_area);
}

void 
Init_gtk_dialog()
{
    VALUE gDialog = G_DEF_CLASS(GTK_TYPE_DIALOG, "Dialog", mGtk);
    rb_define_method(gDialog, "initialize", dialog_initialize, 0);
    rb_define_method(gDialog, "vbox", dialog_vbox, 0);
    rb_define_method(gDialog, "action_area", dialog_action_area, 0);
}
