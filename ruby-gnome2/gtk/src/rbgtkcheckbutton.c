/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcheckbutton.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:36 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
cbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_check_button_new_with_label(STR2CSTR(label));
    }
    else {
	widget = gtk_check_button_new();
    }

    set_widget(self, widget);
    return Qnil;
}

void Init_gtk_check_button()
{
    gCButton = rb_define_class_under(mGtk, "CheckButton", gTButton);

    rb_define_method(gCButton, "initialize", cbtn_initialize, -1);

    /* child initialization */
    Init_gtk_radio_button();
}
