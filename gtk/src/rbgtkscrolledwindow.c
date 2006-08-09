/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscrolledwindow.c -

  $Author: ssimons $
  $Date: 2006/08/09 21:45:09 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
                                   RVAL2GENUM(hpolicy, GTK_TYPE_POLICY_TYPE),
                                   RVAL2GENUM(vpolicy, GTK_TYPE_POLICY_TYPE));
    return self;
}

static VALUE
scwin_get_policy(self)
    VALUE self;
{
    GtkPolicyType hpolicy, vpolicy;

    gtk_scrolled_window_get_policy(GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)),
                                   &hpolicy, &vpolicy);
    return rb_ary_new3(2, 
                       GENUM2RVAL(hpolicy, GTK_TYPE_POLICY_TYPE), 
                       GENUM2RVAL(vpolicy, GTK_TYPE_POLICY_TYPE)); 
}

static VALUE
scwin_add_with_viewport(self, other)
    VALUE self, other;
{
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)),
                                          GTK_WIDGET(RVAL2GOBJ(other)));
    G_CHILD_ADD(self, other);
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
scwin_get_hscrollbar(self)
    VALUE self;
{
    GtkWidget* hscrollbar = gtk_scrolled_window_get_hscrollbar(GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)));
    return GOBJ2RVAL(hscrollbar);
}

static VALUE
scwin_get_vscrollbar(self)
    VALUE self;
{
    GtkWidget* vscrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)));
    return GOBJ2RVAL(vscrollbar);
}
#endif


void 
Init_gtk_scrolled_window()
{
    VALUE gScrolledWin = G_DEF_CLASS(GTK_TYPE_SCROLLED_WINDOW, "ScrolledWindow", mGtk);

    rb_define_method(gScrolledWin, "initialize", scwin_initialize, -1);
    rb_define_method(gScrolledWin, "set_policy", scwin_set_policy, 2);
    rb_define_method(gScrolledWin, "policy", scwin_get_policy, 0);
    rb_define_method(gScrolledWin, "add_with_viewport", scwin_add_with_viewport, 1);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gScrolledWin, "hscrollbar", scwin_get_hscrollbar, 0);
    rb_define_method(gScrolledWin, "vscrollbar", scwin_get_vscrollbar, 0);
#endif
}
