/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscrolledwindow.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
scwin_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkAdjustment *h_adj = NULL;
    GtkAdjustment *v_adj = NULL;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);

    if (!NIL_P(arg1)) h_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
    if (!NIL_P(arg2)) v_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg2));

    RBGTK_INITIALIZE(self, gtk_scrolled_window_new(h_adj, v_adj));
    return Qnil;
}

static VALUE
scwin_set_policy(self, hpolicy, vpolicy)
    VALUE self, hpolicy, vpolicy;
{
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)),
								   (GtkPolicyType)NUM2INT(hpolicy),
								   (GtkPolicyType)NUM2INT(vpolicy));
    return self;
}

static VALUE
scwin_add_with_viewport(self, other)
    VALUE self, other;
{
    gtk_scrolled_window_add_with_viewport(
		GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)),
		GTK_WIDGET(RVAL2GOBJ(other)));
    return self;
}

static VALUE
scwin_get_hadjustment(self)
    VALUE self;
{
    GtkAdjustment *adj =
		gtk_scrolled_window_get_hadjustment(
			GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)));
    return GOBJ2RVAL(adj);
}

static VALUE
scwin_get_vadjustment(self)
    VALUE self;
{
    GtkAdjustment *adj =
		gtk_scrolled_window_get_vadjustment(
			GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)));
    return GOBJ2RVAL(adj);
}

void Init_gtk_scrolled_window()
{
    static RGObjClassInfo cinfo;

    gScrolledWin = rb_define_class_under(mGtk, "ScrolledWindow", gBin);
    cinfo.klass = gScrolledWin;
    cinfo.gtype = GTK_TYPE_SCROLLED_WINDOW;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gScrolledWin, "initialize", scwin_initialize, -1);
    rb_define_method(gScrolledWin, "set_policy", scwin_set_policy, 2);
    rb_define_method(gScrolledWin, "add_with_viewport", scwin_add_with_viewport, 1);
    rb_define_method(gScrolledWin, "hadjustment", scwin_get_hadjustment, 0);
    rb_define_method(gScrolledWin, "get_hadjustment", scwin_get_hadjustment, 0);
    rb_define_method(gScrolledWin, "vadjustment", scwin_get_vadjustment, 0);
    rb_define_method(gScrolledWin, "get_vadjustment", scwin_get_vadjustment, 0);
}
