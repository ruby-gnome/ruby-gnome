/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbin.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bin_child(self)
    VALUE self;
{
    GtkWidget *child = GTK_BIN(RVAL2GOBJ(self))->child;
    return child ? GOBJ2RVAL(child) : Qnil;
}

void Init_gtk_bin()
{
    static rbgtk_class_info cinfo;

    gBin = rb_define_class_under(mGtk, "Bin", gContainer);
    cinfo.klass = gBin;
    cinfo.gtype = GTK_TYPE_BIN;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* rb_define_const(   gBin, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gBin, "child", bin_child, 0);

    /* Child Initialization */
    Init_gtk_alignment();
    Init_gtk_button();
    Init_gtk_eventbox();
    Init_gtk_frame();
    Init_gtk_handle_box();
    Init_gtk_item();
    Init_gtk_scrolled_window();
    Init_gtk_viewport();
    Init_gtk_window();
}
