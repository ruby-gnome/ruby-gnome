/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAlignment

static VALUE
rg_initialize(VALUE self, VALUE xalign, VALUE yalign, VALUE xscale, VALUE yscale)
{
    RBGTK_INITIALIZE(self, gtk_alignment_new(NUM2DBL(xalign),
                                             NUM2DBL(yalign),
                                             NUM2DBL(xscale),
                                             NUM2DBL(yscale)));
    return Qnil;
}

static VALUE
rg_set(VALUE self, VALUE xalign, VALUE yalign, VALUE xscale, VALUE yscale)
{
    gtk_alignment_set(GTK_ALIGNMENT(RVAL2GOBJ(self)),
                      NUM2DBL(xalign), NUM2DBL(yalign),
                      NUM2DBL(xscale), NUM2DBL(yscale));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_padding(VALUE self)
{
    guint top, bottom, left, right;
    gtk_alignment_get_padding(GTK_ALIGNMENT(RVAL2GOBJ(self)),
                              &top, &bottom, &left, &right);
    return rb_ary_new3(4, UINT2NUM(top), UINT2NUM(bottom), 
                       UINT2NUM(left), UINT2NUM(right));
}
static VALUE
rg_set_padding(VALUE self, VALUE top, VALUE bottom, VALUE left, VALUE right)
{
    gtk_alignment_set_padding(GTK_ALIGNMENT(RVAL2GOBJ(self)),
                              NUM2UINT(top), NUM2UINT(bottom),
                              NUM2UINT(left), NUM2UINT(right));
    return self;
}
#endif

void 
Init_gtk_alignment(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ALIGNMENT, "Alignment", mGtk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(set, 4);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(padding, 0);
    RG_DEF_METHOD(set_padding, 4);
#endif

}
