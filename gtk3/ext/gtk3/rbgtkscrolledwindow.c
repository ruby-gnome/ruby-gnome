/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cScrolledWindow
#define _SELF(self) (GTK_SCROLLED_WINDOW(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_set_policy(VALUE self, VALUE hpolicy, VALUE vpolicy)
{
    gtk_scrolled_window_set_policy(_SELF(self),
                                   RVAL2GENUM(hpolicy, GTK_TYPE_POLICY_TYPE),
                                   RVAL2GENUM(vpolicy, GTK_TYPE_POLICY_TYPE));
    return self;
}

static VALUE
rg_policy(VALUE self)
{
    GtkPolicyType hpolicy, vpolicy;

    gtk_scrolled_window_get_policy(_SELF(self), &hpolicy, &vpolicy);
    return rb_ary_new3(2, 
                       GENUM2RVAL(hpolicy, GTK_TYPE_POLICY_TYPE), 
                       GENUM2RVAL(vpolicy, GTK_TYPE_POLICY_TYPE)); 
}

static VALUE
rg_add_with_viewport(VALUE self, VALUE other)
{
    gtk_scrolled_window_add_with_viewport(_SELF(self),
                                          GTK_WIDGET(RVAL2GOBJ(other)));
    G_CHILD_ADD(self, other);
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_hscrollbar(VALUE self)
{
    return GOBJ2RVAL(gtk_scrolled_window_get_hscrollbar(_SELF(self)));
}

static VALUE
rg_vscrollbar(VALUE self)
{
    return GOBJ2RVAL(gtk_scrolled_window_get_vscrollbar(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_set_placement(VALUE self, VALUE corner_type)
{
    gtk_scrolled_window_set_placement(_SELF(self), 
                                      RVAL2GENUM(corner_type, GTK_TYPE_CORNER_TYPE));
    return self;
}

static VALUE
rg_unset_placement(VALUE self)
{
    gtk_scrolled_window_unset_placement(_SELF(self));
    return self;
}

static VALUE
rg_placement(VALUE self)
{
    return GENUM2RVAL(gtk_scrolled_window_get_placement(_SELF(self)), 
                      GTK_TYPE_CORNER_TYPE);
}
#endif

void 
Init_gtk_scrolled_window(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SCROLLED_WINDOW, "ScrolledWindow", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_policy, 2);
    RG_DEF_METHOD(policy, 0);
    RG_DEF_METHOD(add_with_viewport, 1);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(hscrollbar, 0);
    RG_DEF_METHOD(vscrollbar, 0);
#endif

#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(unset_placement, 0);
    RG_DEF_METHOD(placement, 0);
    RG_DEF_METHOD(set_placement, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "placement");
#endif
}
