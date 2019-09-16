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

#if GTK_CHECK_VERSION(3, 2, 0)

#define RG_TARGET_NAMESPACE cFontChooserDialog
#define _SELF(self) (RVAL2GTKFONTCHOOSERDIALOG(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_title, rb_parent;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "title", &rb_title,
                     "parent", &rb_parent,
                     NULL);

    /* TODO: accept NULL? */
    widget = gtk_font_chooser_dialog_new(RVAL2CSTR(rb_title), RVAL2GTKWINDOW(rb_parent));
    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}
#endif

void
Init_gtk_fontchooserdialog(VALUE mGtk)
{
#if GTK_CHECK_VERSION(3, 2, 0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FONT_CHOOSER_DIALOG, "FontChooserDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
#endif
}
