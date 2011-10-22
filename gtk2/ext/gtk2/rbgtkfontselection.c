/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 2001  Neil Conway <neilconway@rogers.com>
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

static VALUE
fs_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_font_selection_new());
    return Qnil;
}

void 
Init_gtk_font_selection(void)
{
    VALUE gFontSelection = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION, "FontSelection", mGtk);

    rb_define_method(gFontSelection, "initialize", fs_initialize, 0);
    rb_undef_method(gFontSelection, "font");
}
