/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcheckbutton.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

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

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_check_button()
{
    VALUE gCButton = G_DEF_CLASS(GTK_TYPE_CHECK_BUTTON, "CheckButton", mGtk);

    rb_define_method(gCButton, "initialize", cbtn_initialize, -1);
}
