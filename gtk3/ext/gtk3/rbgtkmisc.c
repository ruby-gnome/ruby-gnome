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

#define RG_TARGET_NAMESPACE cMisc
#define _SELF(s) (GTK_MISC(RVAL2GOBJ(s)))

static VALUE
rg_set_alignment(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_misc_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
rg_set_padding(VALUE self, VALUE xpad, VALUE ypad)
{
    gtk_misc_set_padding(_SELF(self), NUM2DBL(xpad), NUM2DBL(ypad));
    return self;
}

static VALUE
rg_alignment(VALUE self)
{
    gfloat xalign, yalign;
    gtk_misc_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_ary_new3(2, rb_float_new(xalign), rb_float_new(yalign));
}

static VALUE
rg_padding(VALUE self)
{
    gint xpad, ypad;
    gtk_misc_get_padding(_SELF(self), &xpad, &ypad);

    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}
void 
Init_gtk_misc(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MISC, "Misc", mGtk);

    RG_DEF_METHOD(set_alignment, 2);
    RG_DEF_METHOD(set_padding, 2);
    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD(padding, 0);
}
