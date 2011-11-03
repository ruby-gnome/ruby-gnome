/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cFontButton

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE fontname;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "01", &fontname);

    if (NIL_P(fontname)){
        widget = gtk_font_button_new();
    } else {
        widget = gtk_font_button_new_with_font(RVAL2CSTR(fontname));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

#endif

void 
Init_gtk_fontbutton(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FONT_BUTTON, "FontButton", mGtk);
    RG_DEF_METHOD(initialize, -1);
#endif
}
