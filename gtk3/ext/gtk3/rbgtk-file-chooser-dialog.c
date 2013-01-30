/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Seiya Nishizawa, Masao Mutoh
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

#define RG_TARGET_NAMESPACE cFileChooserDialog
#define _SELF(self) RVAL2GTKFILECHOOSERDIALOG(self)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_title, rb_parent, rb_action, rb_button_ary;
    const gchar *title;
    GtkWindow *parent;
    GtkFileChooserAction action;
    GtkWidget *dialog;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "title", &rb_title,
                     "parent", &rb_parent,
                     "action", &rb_action,
                     "buttons", &rb_button_ary,
                     NULL);
    title = RVAL2CSTR_ACCEPT_NIL(rb_title);
    parent = NIL_P(rb_parent) ? NULL : RVAL2GTKWINDOW(rb_parent);
    action = NIL_P(rb_action) ? GTK_FILE_CHOOSER_ACTION_OPEN : RVAL2GTKFILECHOOSERACTION(rb_action);

    dialog = gtk_file_chooser_dialog_new(title, parent, action, NULL, NULL);
    RBGTK_INITIALIZE(self, dialog);
    if (!NIL_P(rb_button_ary))
        rb_funcall2(self, rb_intern("add_buttons"),
                    RARRAY_LEN(rb_button_ary), RARRAY_PTR(rb_button_ary));

    return Qnil;
}

void 
Init_gtk_file_chooser_dialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_DIALOG, "FileChooserDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
}
