/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdialog.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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

void Init_gtk_dialog()
{
    static rbgtk_class_info cinfo;

    gDialog = rb_define_class_under(mGtk, "Dialog", gWindow);
    cinfo.klass = gDialog;
    cinfo.gtype = GTK_TYPE_DIALOG;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* rb_define_const(gDialog, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gDialog, "initialize", dialog_initialize, 0);
    rb_define_method(gDialog, "vbox", dialog_vbox, 0);
    rb_define_method(gDialog, "action_area", dialog_action_area, 0);

    /* child initialization */
    Init_gtk_input_dialog();
}
