/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define _SELF(s) (GTK_COLOR_SELECTION_DIALOG(RVAL2GOBJ(s)))

static VALUE
cdialog_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_color_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

void 
Init_gtk_color_selection_dialog(void)
{
    VALUE gColorSelDialog = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION_DIALOG, 
                                        "ColorSelectionDialog", mGtk);

    rb_define_method(gColorSelDialog, "initialize", cdialog_initialize, -1);
    /* NOTE: Backward compatibility */
    rb_define_alias(gColorSelDialog, "colorsel", "color_selection");
}
