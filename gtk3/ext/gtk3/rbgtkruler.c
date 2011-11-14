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

#define RG_TARGET_NAMESPACE cRuler
#define _SELF(self) (GTK_RULER(RVAL2GOBJ(self)))

static VALUE
rg_set_range(VALUE self, VALUE lower, VALUE upper, VALUE position, VALUE max_size)
{
    gtk_ruler_set_range(_SELF(self), NUM2DBL(lower), NUM2DBL(upper),
                        NUM2DBL(position), NUM2DBL(max_size));
    return self;
}

static VALUE
rg_range(VALUE self)
{
    gdouble lower, upper, position, max_size;
    gtk_ruler_get_range(_SELF(self), &lower, &upper, &position, &max_size);
    return rb_ary_new3(4, rb_float_new(lower), rb_float_new(upper), 
                       rb_float_new(position), rb_float_new(max_size));
}

static VALUE
rg_draw_ticks(VALUE self)
{
    gtk_ruler_draw_ticks(_SELF(self));
    return self;
}

static VALUE
rg_draw_pos(VALUE self)
{
    gtk_ruler_draw_pos(_SELF(self));
    return self;
}

void 
Init_gtk_ruler(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RULER, "Ruler", mGtk);

    RG_DEF_METHOD(set_range, 4);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "range");
    RG_DEF_METHOD(range, 0);
    RG_DEF_METHOD(draw_ticks, 0);
    RG_DEF_METHOD(draw_pos, 0);
}
