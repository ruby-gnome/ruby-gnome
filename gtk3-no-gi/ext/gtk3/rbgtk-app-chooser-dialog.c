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

#define RG_TARGET_NAMESPACE cAppChooserDialog
#define _SELF(self) (RVAL2GTKAPPCHOOSERDIALOG(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_parent, rb_file, rb_flags, rb_content_type;
    GtkWindow *parent;
    GtkDialogFlags flags;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "parent", &rb_parent,
                     "flags", &rb_flags,
                     "file", &rb_file,
                     "content_type", &rb_content_type,
                     NULL);
    parent = NIL_P(rb_parent) ? NULL : RVAL2GTKWINDOW(rb_parent);
    flags = NIL_P(rb_flags) ? 0 : RVAL2GTKDIALOGFLAGS(rb_flags);

    if (!NIL_P(rb_file))
        widget = gtk_app_chooser_dialog_new(parent, flags, RVAL2GFILE(rb_file));
    else
        widget = gtk_app_chooser_dialog_new_for_content_type(parent,
                                                             flags,
                                                             RVAL2CSTR(rb_content_type));
    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rg_widget(VALUE self)
{
    return GOBJ2RVAL(gtk_app_chooser_dialog_get_widget(_SELF(self)));
}

void
Init_gtk_appchooserdialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_APP_CHOOSER_DIALOG, "AppChooserDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(widget, 0);
}
