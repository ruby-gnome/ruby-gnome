/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkfixed.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
fixed_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_fixed_new());
    return Qnil;
}

static VALUE
fixed_put(self, win, x, y)
    VALUE self, win, x, y;
{
    gtk_fixed_put(GTK_FIXED(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(win)), 
				  NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
fixed_move(self, win, x, y)
    VALUE self, win, x, y;
{
    gtk_fixed_move(GTK_FIXED(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(win)), 
				   NUM2INT(x), NUM2INT(y));
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
