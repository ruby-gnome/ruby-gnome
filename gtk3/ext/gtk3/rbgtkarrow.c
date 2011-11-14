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

#define RG_TARGET_NAMESPACE cArrow

static VALUE
rg_initialize(VALUE self, VALUE arrow_t, VALUE shadow_t)
{
    RBGTK_INITIALIZE(self, gtk_arrow_new(RVAL2GENUM(arrow_t, GTK_TYPE_ARROW_TYPE),
                                         RVAL2GENUM(shadow_t, GTK_TYPE_SHADOW_TYPE)));
    return Qnil;
}

static VALUE
rg_set(VALUE self, VALUE arrow_t, VALUE shadow_t)
{
    gtk_arrow_set(GTK_ARROW(RVAL2GOBJ(self)),
                  RVAL2GENUM(arrow_t, GTK_TYPE_ARROW_TYPE),
                  RVAL2GENUM(shadow_t, GTK_TYPE_SHADOW_TYPE));
    return self;
}

void 
Init_gtk_arrow(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ARROW, "Arrow", mGtk);

    RG_DEF_METHOD(initialize, 2);
    RG_DEF_METHOD(set, 2);

    /* GtkArrowType (from General constants) */
    G_DEF_CLASS(GTK_TYPE_ARROW_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_ARROW_TYPE, "GTK_ARROW_");
}
