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

#define RG_TARGET_NAMESPACE cFixed
#define _SELF(self) (GTK_FIXED(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_fixed_new());
    return Qnil;
}

static VALUE
rg_put(VALUE self, VALUE win, VALUE x, VALUE y)
{
    gtk_fixed_put(_SELF(self), GTK_WIDGET(RVAL2GOBJ(win)), 
                  NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_move(VALUE self, VALUE win, VALUE x, VALUE y)
{
    gtk_fixed_move(_SELF(self), GTK_WIDGET(RVAL2GOBJ(win)), 
                   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_has_window_p(VALUE self)
{
    return CBOOL2RVAL(gtk_fixed_get_has_window(_SELF(self)));
}

static VALUE
rg_set_has_window(VALUE self, VALUE has_window)
{
    gtk_fixed_set_has_window(_SELF(self), RVAL2CBOOL(has_window));
    return self;
}

void 
Init_gtk_fixed(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FIXED, "Fixed", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(put, 3);
    RG_DEF_METHOD(move, 3);
    RG_DEF_METHOD_P(has_window, 0);
    RG_DEF_METHOD(set_has_window, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
