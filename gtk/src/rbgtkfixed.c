/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfixed.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

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

void 
Init_gtk_fixed()
{
    VALUE gFixed = G_DEF_CLASS(GTK_TYPE_FIXED, "Fixed", mGtk);

    rb_define_method(gFixed, "initialize", fixed_initialize, 0);
    rb_define_method(gFixed, "put", fixed_put, 3);
    rb_define_method(gFixed, "move", fixed_move, 3);
}
