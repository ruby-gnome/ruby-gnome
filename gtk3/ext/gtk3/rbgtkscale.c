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

#define RG_TARGET_NAMESPACE cScale
#define _SELF(self) (GTK_SCALE(RVAL2GOBJ(self)))

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_layout(VALUE self)
{
    return GOBJ2RVAL(gtk_scale_get_layout(_SELF(self)));
}

static VALUE
rg_layout_offsets(VALUE self)
{
    gint x, y;
    gtk_scale_get_layout_offsets(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}
#endif

void 
Init_gtk_scale(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);
    RG_DEF_METHOD(layout, 0);
    RG_DEF_METHOD(layout_offsets, 0);
#else
    G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);
#endif
}
