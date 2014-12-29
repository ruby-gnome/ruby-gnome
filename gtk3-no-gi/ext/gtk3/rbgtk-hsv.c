/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cHSV
#define _SELF(self) (RVAL2GTKHSV(self))

static VALUE
rg_s_to_rgb(G_GNUC_UNUSED VALUE self, VALUE h, VALUE s, VALUE v)
{
    gdouble r, g, b;

    gtk_hsv_to_rgb(NUM2DBL(h), NUM2DBL(s), NUM2DBL(v), &r, &g, &b);

    return rb_ary_new3(3, DBL2NUM(r), DBL2NUM(g), DBL2NUM(b));
}

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_hsv_new());

    return Qnil;
}

static VALUE
rg_color(VALUE self)
{
    gdouble h, s, v;

    gtk_hsv_get_color(_SELF(self), &h, &s, &v);

    return rb_ary_new3(3, DBL2NUM(h), DBL2NUM(s), DBL2NUM(v));
}

static VALUE
rg_metrics(VALUE self)
{
    gint size, ring_width;

    gtk_hsv_get_metrics(_SELF(self), &size, &ring_width);

    return rb_ary_new3(2, INT2NUM(size), INT2NUM(ring_width));
}

static VALUE
rg_adjusting_p(VALUE self)
{
    gboolean result;

    result = gtk_hsv_is_adjusting(_SELF(self));

    return CBOOL2RVAL(result);
}

static VALUE
rg_set_color(VALUE self, VALUE h, VALUE s, VALUE v)
{
    gtk_hsv_set_color(_SELF(self), NUM2DBL(h), NUM2DBL(s), NUM2DBL(v));

    return self;
}

static VALUE
rg_set_metrics(VALUE self, VALUE size, VALUE ring_width)
{
    gtk_hsv_set_metrics(_SELF(self), NUM2INT(size), NUM2INT(ring_width));

    return self;
}

void
Init_gtk_hsv(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_HSV, "HSV", mGtk);

    RG_DEF_SMETHOD(to_rgb, 3);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(color, 0);
    RG_DEF_METHOD(metrics, 0);
    RG_DEF_METHOD_P(adjusting, 0);
    RG_DEF_METHOD(set_color, 3);
    RG_DEF_METHOD(set_metrics, 2);
}
