/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkinputdialog.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:37 $

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
    gInputDialog = rb_define_class_under(mGtk, "InputDialog", gDialog);

    rb_define_const(gInputDialog, "SIGNAL_ENABLE_DEVICE", rb_str_new2("enable_device"));
    rb_define_const(gInputDialog, "SIGNAL_DISABLE_DEVICE", rb_str_new2("disable_device"));

    rb_define_method(gInputDialog, "initialize", idiag_initialize, 0);
}
