/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbin.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bin_child(self)
    VALUE self;
{
    GtkWidget *child = GTK_BIN(get_widget(self))->child;
    return child ? get_value_from_gobject(GTK_OBJECT(child)) : Qnil;
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
