/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbin.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

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

void 
Init_gtk_bin()
{
    VALUE gBin = G_DEF_CLASS(GTK_TYPE_BIN, "Bin", mGtk);

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
