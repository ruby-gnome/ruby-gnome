/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtklayout.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE gLayout;

static VALUE
layout_initialize(self, hadjustment, vadjustment)
     VALUE self, hadjustment, vadjustment;
{
    GtkWidget* layout;
    layout = gtk_layout_new(
		NIL_P(hadjustment)?0:GTK_ADJUSTMENT(RVAL2GOBJ(hadjustment)),
		NIL_P(vadjustment)?0:GTK_ADJUSTMENT(RVAL2GOBJ(hadjustment)));
    RBGTK_INITIALIZE(self, layout);
    return Qnil;
}

static VALUE
layout_put(self, widget, x, y)
    VALUE self, widget, x, y;
{
    gtk_layout_put(GTK_LAYOUT(RVAL2GOBJ(self)),
		   GTK_WIDGET(RVAL2GOBJ(widget)),
		   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_move(self, widget, x,  y)
    VALUE self, widget, x, y;
{
    gtk_layout_move(GTK_LAYOUT(RVAL2GOBJ(self)),
		    GTK_WIDGET(RVAL2GOBJ(widget)),
		    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_set_size(self, width, height)
    VALUE self, width, height;
{
    gtk_layout_set_size(GTK_LAYOUT(RVAL2GOBJ(self)),
			NUM2UINT(width),
			NUM2UINT(height));
    return self;
}

static VALUE
layout_get_hadjustment(self)
    VALUE self;
{
    GtkAdjustment* hadjustment;
    hadjustment = gtk_layout_get_hadjustment(GTK_LAYOUT(RVAL2GOBJ(self)));
    return GOBJ2RVAL(hadjustment);
}

static VALUE
layout_get_vadjustment(self)
    VALUE self;
{
    GtkAdjustment* vadjustment;
    vadjustment = gtk_layout_get_vadjustment(GTK_LAYOUT(RVAL2GOBJ(self)));
    return GOBJ2RVAL(vadjustment);
}

static VALUE
layout_set_hadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_layout_set_hadjustment(GTK_LAYOUT(RVAL2GOBJ(self)),
			       GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
layout_set_vadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_layout_set_vadjustment(GTK_LAYOUT(RVAL2GOBJ(self)),
			       GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
layout_freeze(self)
    VALUE self;
{
    gtk_layout_freeze(GTK_LAYOUT(RVAL2GOBJ(self)));
    return self;
}

static VALUE
layout_thaw(self)
    VALUE self;
{
    gtk_layout_thaw(GTK_LAYOUT(RVAL2GOBJ(self)));
    return self;
}

static VALUE
layout_get_bin_window(self)
    VALUE self;
{
    return make_gdkwindow(GTK_LAYOUT(RVAL2GOBJ(self))->bin_window);
}

void
Init_gtk_layout()
{
    static rbgtk_class_info cinfo;

    gLayout = rb_define_class_under(mGtk, "Layout", gContainer);
    cinfo.klass = gLayout;
    cinfo.gtype = GTK_TYPE_LAYOUT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* Instance methods */
    rb_define_method(gLayout, "initialize", layout_initialize, 2);
    rb_define_method(gLayout, "put", layout_put, 3);
    rb_define_method(gLayout, "move", layout_move, 3);
    rb_define_method(gLayout, "set_size", layout_set_size, 2);
    rb_define_method(gLayout, "get_hadjustment", layout_get_hadjustment, 0);
    rb_define_method(gLayout, "hadjustment", layout_get_hadjustment, 0);
    rb_define_method(gLayout, "get_vadjustment", layout_get_vadjustment, 0);
    rb_define_method(gLayout, "vadjustment", layout_get_vadjustment, 0);
    rb_define_method(gLayout, "set_hadjustment", layout_set_hadjustment, 1);
    rb_define_method(gLayout, "hadjustment=", layout_set_hadjustment, 1);
    rb_define_method(gLayout, "set_vadjustment", layout_set_vadjustment, 1);
    rb_define_method(gLayout, "vadjustment=", layout_set_vadjustment, 1);
    rb_define_method(gLayout, "freeze", layout_freeze, 0);
    rb_define_method(gLayout, "thaw", layout_thaw, 0);
    rb_define_method(gLayout, "bin_window", layout_get_bin_window, 0);

    /* Signals */
    rb_define_const(gLayout, "SIGNAL_SET_SCROLL_ADJUSTMENTS",
		    rb_str_new2("set_scroll_adjustments"));
}
