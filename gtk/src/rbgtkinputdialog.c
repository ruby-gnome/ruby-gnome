/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkinputdialog.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
idiag_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_input_dialog_new());
    return Qnil;
}

void Init_gtk_input_dialog()
{
    static rbgtk_class_info cinfo;

    gInputDialog = rb_define_class_under(mGtk, "InputDialog", gDialog);
    cinfo.klass = gInputDialog;
    cinfo.gtype = GTK_TYPE_INPUT_DIALOG;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gInputDialog, "SIGNAL_ENABLE_DEVICE", rb_str_new2("enable_device"));
    rb_define_const(gInputDialog, "SIGNAL_DISABLE_DEVICE", rb_str_new2("disable_device"));

    rb_define_method(gInputDialog, "initialize", idiag_initialize, 0);
}
