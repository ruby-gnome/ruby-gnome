/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cMessageDialog
#define _SELF(s) (RVAL2GTKMESSAGEDIALOG(s))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_parent, rb_flags, rb_type, rb_buttons_type, rb_message;
    GtkWindow *parent;
    GtkDialogFlags flags;
    GtkMessageType type;
    GtkButtonsType buttons_type;
    const gchar *message;
    GtkWidget *dialog;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "parent", &rb_parent,
                     "flags", &rb_flags,
                     "type", &rb_type,
                     "buttons_type", &rb_buttons_type,
                     "message", &rb_message,
                     NULL);
    parent = NIL_P(rb_parent) ? NULL : RVAL2GTKWINDOW(rb_parent);
    flags = NIL_P(rb_flags) ? 0 : RVAL2GTKDIALOGFLAGS(rb_flags);
    type = NIL_P(rb_type) ? GTK_MESSAGE_INFO : RVAL2GTKMESSAGETYPE(rb_type);
    buttons_type = NIL_P(rb_buttons_type) ? GTK_BUTTONS_OK : RVAL2GTKBUTTONSTYPE(rb_buttons_type);
    message = NIL_P(rb_message) ? "" : RVAL2CSTR(rb_message);

    dialog = gtk_message_dialog_new(parent, flags, type, buttons_type, "%s", message);
    RBGTK_INITIALIZE(self, dialog);

    return Qnil;
}

static VALUE
rg_set_markup(VALUE self, VALUE str)
{
    gtk_message_dialog_set_markup(_SELF(self), RVAL2CSTR(str));
    return self;
}

void 
Init_gtk_message_dialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MESSAGE_DIALOG, "MessageDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_markup, 1);

    G_DEF_CLASS(GTK_TYPE_BUTTONS_TYPE, "ButtonsType", RG_TARGET_NAMESPACE);
}
