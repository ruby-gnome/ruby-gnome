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
misc_set_align(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_misc_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
misc_set_padding(VALUE self, VALUE xpad, VALUE ypad)
{
    gtk_misc_set_padding(_SELF(self), NUM2DBL(xpad), NUM2DBL(ypad));
    return self;
}

static VALUE
misc_get_align(VALUE self)
{
    gfloat xalign, yalign;
    gtk_misc_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_ary_new3(2, rb_float_new(xalign), rb_float_new(yalign));
}

static VALUE
misc_get_padding(VALUE self)
{
    gint xpad, ypad;
    gtk_misc_get_padding(_SELF(self), &xpad, &ypad);

    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}
void 
Init_gtk_misc(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MISC, "Misc", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "set_alignment", misc_set_align, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_padding", misc_set_padding, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "alignment", misc_get_align, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "padding", misc_get_padding, 0);
}
