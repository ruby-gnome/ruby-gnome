/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgtksourcemain.h"

/* Module: Gtk::SourceStyle
 */

#define RG_TARGET_NAMESPACE cSourceStyle
#define _SELF(self) (GTK_SOURCE_STYLE(RVAL2GOBJ(self)))

static VALUE
rg_copy(VALUE self)
{
    return GOBJ2RVAL (gtk_source_style_copy (_SELF (self)));
}

void
Init_gtk_sourcestyle (VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GTK_SOURCE_TYPE_STYLE, "SourceStyle", mGtk);

    RG_DEF_METHOD(copy, 0);
}
