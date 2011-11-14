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

#define RG_TARGET_NAMESPACE cAspectFrame

static VALUE
rg_initialize(VALUE self, VALUE label, VALUE xalign, VALUE yalign, VALUE ratio, VALUE obey_child)
{
    RBGTK_INITIALIZE(self, gtk_aspect_frame_new(NIL_P(label)?NULL:RVAL2CSTR(label),
                                                NUM2DBL(xalign),
                                                NUM2DBL(yalign),
                                                NUM2DBL(ratio),
                                                RVAL2CBOOL(obey_child)));
    return Qnil;
}

static VALUE
rg_set(VALUE self, VALUE xalign, VALUE yalign, VALUE ratio, VALUE obey_child)
{
    gtk_aspect_frame_set(GTK_ASPECT_FRAME(RVAL2GOBJ(self)),
                         NUM2DBL(xalign), NUM2DBL(yalign),
                         NUM2DBL(ratio), RVAL2CBOOL(obey_child));
    return self;
}

void 
Init_gtk_aspect_frame(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ASPECT_FRAME, "AspectFrame", mGtk);

    RG_DEF_METHOD(initialize, 5);
    RG_DEF_METHOD(set, 4);
}
