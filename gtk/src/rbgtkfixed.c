/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkfixed.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
fixed_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_fixed_new());
    return Qnil;
}

static VALUE
fixed_put(self, win, x, y)
    VALUE self, win, x, y;
{
    gtk_fixed_put(GTK_FIXED(get_widget(self)), get_widget(win), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
fixed_move(self, win, x, y)
    VALUE self, win, x, y;
{
    gtk_fixed_move(GTK_FIXED(get_widget(self)), get_widget(win), NUM2INT(x), NUM2INT(y));
    return self;
}

void Init_gtk_fixed()
{
    static rbgtk_class_info cinfo;

    gFixed = rb_define_class_under(mGtk, "Fixed", gContainer);
    cinfo.klass = gFixed;
    cinfo.gtype = GTK_TYPE_FIXED;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gFixed, "initialize", fixed_initialize, 0);
    rb_define_method(gFixed, "put", fixed_put, 3);
    rb_define_method(gFixed, "move", fixed_move, 3);
}
