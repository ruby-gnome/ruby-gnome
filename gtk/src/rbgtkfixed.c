/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfixed.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_FIXED(RVAL2GOBJ(self)))

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
    gtk_fixed_put(_SELF(self), GTK_WIDGET(RVAL2GOBJ(win)), 
                  NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
fixed_move(self, win, x, y)
    VALUE self, win, x, y;
{
    gtk_fixed_move(_SELF(self), GTK_WIDGET(RVAL2GOBJ(win)), 
                   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
fixed_get_has_window(self)
    VALUE self;
{
    return gtk_fixed_get_has_window(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
fixed_set_has_window(self, has_window)
    VALUE self;
{
    gtk_fixed_set_has_window(_SELF(self), RTEST(has_window));
    return self;
}

void 
Init_gtk_fixed()
{
    VALUE gFixed = G_DEF_CLASS(GTK_TYPE_FIXED, "Fixed", mGtk);

    rb_define_method(gFixed, "initialize", fixed_initialize, 0);
    rb_define_method(gFixed, "put", fixed_put, 3);
    rb_define_method(gFixed, "move", fixed_move, 3);
    rb_define_method(gFixed, "has_window?", fixed_get_has_window, 0);
    rb_define_method(gFixed, "set_has_window", fixed_set_has_window, 1);

    G_DEF_SETTERS(gFixed);
}
