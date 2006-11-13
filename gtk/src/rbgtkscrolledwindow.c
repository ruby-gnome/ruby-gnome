/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscrolledwindow.c -

  $Author: ktou $
  $Date: 2006/11/13 12:53:21 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)))

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
    gtk_scrolled_window_set_policy(_SELF(self),
                                   RVAL2GENUM(hpolicy, GTK_TYPE_POLICY_TYPE),
                                   RVAL2GENUM(vpolicy, GTK_TYPE_POLICY_TYPE));
    return self;
}

static VALUE
scwin_get_policy(self)
    VALUE self;
{
    GtkPolicyType hpolicy, vpolicy;

    gtk_scrolled_window_get_policy(_SELF(self), &hpolicy, &vpolicy);
    return rb_ary_new3(2, 
                       GENUM2RVAL(hpolicy, GTK_TYPE_POLICY_TYPE), 
                       GENUM2RVAL(vpolicy, GTK_TYPE_POLICY_TYPE)); 
}

static VALUE
scwin_add_with_viewport(self, other)
    VALUE self, other;
{
    gtk_scrolled_window_add_with_viewport(_SELF(self),
                                          GTK_WIDGET(RVAL2GOBJ(other)));
    G_CHILD_ADD(self, other);
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
scwin_get_hscrollbar(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_scrolled_window_get_hscrollbar(_SELF(self)));
}

static VALUE
scwin_get_vscrollbar(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_scrolled_window_get_vscrollbar(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
scwin_set_placement(self, corner_type)
    VALUE self, corner_type;
{
    gtk_scrolled_window_set_placement(_SELF(self), 
                                      RVAL2GENUM(corner_type, GTK_TYPE_CORNER_TYPE));
    return self;
}

static VALUE
scwin_unset_placement(self)
    VALUE self;
{
    gtk_scrolled_window_unset_placement(_SELF(self));
    return self;
}

static VALUE
scwin_get_placement(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_scrolled_window_get_placement(_SELF(self)), 
                      GTK_TYPE_CORNER_TYPE);
}
#endif

/* Defined as properties
GtkAdjustment* gtk_scrolled_window_get_hadjustment
                                            (GtkScrolledWindow *scrolled_window);
GtkAdjustment* gtk_scrolled_window_get_vadjustment
                                            (GtkScrolledWindow *scrolled_window);
GtkWidget*  gtk_scrolled_window_get_hscrollbar
                                            (GtkScrolledWindow *scrolled_window);
GtkWidget*  gtk_scrolled_window_get_vscrollbar
                                            (GtkScrolledWindow *scrolled_window);
void        gtk_scrolled_window_add_with_viewport
                                            (GtkScrolledWindow *scrolled_window,
                                             GtkWidget *child);
void        gtk_scrolled_window_set_shadow_type
                                            (GtkScrolledWindow *scrolled_window,
                                             GtkShadowType type);
void        gtk_scrolled_window_set_hadjustment
                                            (GtkScrolledWindow *scrolled_window,
                                             GtkAdjustment *hadjustment);
void        gtk_scrolled_window_set_vadjustment
                                            (GtkScrolledWindow *scrolled_window,
                                             GtkAdjustment *vadjustment);
GtkShadowType gtk_scrolled_window_get_shadow_type
                                            (GtkScrolledWindow *scrolled_window);

*/


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

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gScrolledWin, "unset_placement", scwin_unset_placement, 0);
    rb_define_method(gScrolledWin, "placement", scwin_get_placement, 0);
    rb_define_method(gScrolledWin, "set_placement", scwin_set_placement, 1);
#endif
    G_DEF_SETTERS(gScrolledWin);
}
