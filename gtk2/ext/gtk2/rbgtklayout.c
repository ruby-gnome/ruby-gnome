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

#define RG_TARGET_NAMESPACE cLayout
#define _SELF(self) (GTK_LAYOUT(RVAL2GOBJ(self)))
#define RVAL2ADJ(a) (GTK_ADJUSTMENT(RVAL2GOBJ(a)))

static VALUE
layout_initialize(int argc, VALUE *argv, VALUE self)
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
layout_put(VALUE self, VALUE widget, VALUE x, VALUE y)
{
    gtk_layout_put(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
		   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_move(VALUE self, VALUE widget, VALUE x, VALUE y)
{
    gtk_layout_move(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
		    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
layout_set_size(VALUE self, VALUE width, VALUE height)
{
    gtk_layout_set_size(_SELF(self), NUM2UINT(width), NUM2UINT(height));
    return self;
}

static VALUE
layout_get_size(VALUE self)
{
    guint width, height;
    gtk_layout_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
layout_get_bin_window(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->bin_window);
}

void
Init_gtk_layout(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_LAYOUT, "Layout", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", layout_initialize, -1);
    rb_define_method(RG_TARGET_NAMESPACE, "put", layout_put, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "move", layout_move, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "set_size", layout_set_size, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "size", layout_get_size, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "bin_window", layout_get_bin_window, 0);
}
