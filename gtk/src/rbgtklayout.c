/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtklayout.c -

  $Author: mutoh $
  $Date: 2002/11/04 16:19:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_LAYOUT(RVAL2GOBJ(self)))
#define RVAL2ADJ(a) (GTK_ADJUSTMENT(RVAL2GOBJ(a)))

static VALUE
layout_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE hadjustment, vadjustment;
    GtkWidget* layout;
    rb_scan_args(argc, argv, "02", &hadjustment, &vadjustment);

    layout = gtk_layout_new(
        NIL_P(hadjustment) ? 0 : RVAL2ADJ(hadjustment),
        NIL_P(vadjustment) ? 0 : RVAL2ADJ(vadjustment));
    
    RBGTK_INITIALIZE(self, layout);
    return Qnil;
}

static VALUE
layout_put(self, widget, x, y)
    VALUE self, widget, x, y;
{
    gtk_layout_put(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
		   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_move(self, widget, x,  y)
    VALUE self, widget, x, y;
{
    gtk_layout_move(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
		    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_set_size(self, width, height)
    VALUE self, width, height;
{
    gtk_layout_set_size(_SELF(self), NUM2UINT(width), NUM2UINT(height));
    return self;
}

static VALUE
layout_get_size(self)
    VALUE self;
{
    guint width, height;
    gtk_layout_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
layout_get_bin_window(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->bin_window);
}

void
Init_gtk_layout()
{
    VALUE gLayout = G_DEF_CLASS(GTK_TYPE_LAYOUT, "Layout", mGtk);

    rb_define_method(gLayout, "initialize", layout_initialize, -1);
    rb_define_method(gLayout, "put", layout_put, 3);
    rb_define_method(gLayout, "move", layout_move, 3);
    rb_define_method(gLayout, "set_size", layout_set_size, 2);
    rb_define_method(gLayout, "size", layout_get_size, 0);
    rb_define_method(gLayout, "bin_window", layout_get_bin_window, 0);
}
