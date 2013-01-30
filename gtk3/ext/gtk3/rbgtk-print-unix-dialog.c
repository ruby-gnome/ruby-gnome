/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#ifdef HAVE_GTK_UNIX_PRINT
#include <gtk/gtkunixprint.h>

#define RG_TARGET_NAMESPACE cPrintUnixDialog
#define _SELF(s) (RVAL2GTKPRINTUNIXDIALOG(s))
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_title, rb_parent;
    const gchar *title;
    GtkWindow *parent;
    GtkWidget *dialog;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "title", &rb_title,
                     "parent", &rb_parent,
                     NULL);
    title = RVAL2CSTR_ACCEPT_NIL(rb_title);
    parent = NIL_P(rb_parent) ? NULL : RVAL2GTKWINDOW(rb_parent);

    dialog = gtk_print_unix_dialog_new(title, parent);
    RBGTK_INITIALIZE(self, dialog);

    return Qnil;
}

static VALUE
rg_add_custom_tab(VALUE self, VALUE child, VALUE tab_label)
{
    gtk_print_unix_dialog_add_custom_tab(_SELF(self),
                                         RVAL2GTKWIDGET(child),
                                         RVAL2GTKWIDGET(tab_label));
    return self;
}

static VALUE
rg_set_manual_capability(VALUE self, VALUE rb_capabilities)
{
    GtkPrintCapabilities capabilities;

    capabilities = RVAL2GTKPRINTCAPABILITIES(rb_capabilities);
    gtk_print_unix_dialog_set_manual_capabilities(_SELF(self), capabilities);
    return self;
}
#endif

void
Init_gtk_print_unix_dialog(VALUE mGtk)
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_UNIX_DIALOG,
                                         "PrintUnixDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(add_custom_tab, 2);
    RG_DEF_METHOD(set_manual_capability, 1);
    RG_DEF_ALIAS("settings", "print_settings");
    RG_DEF_ALIAS("set_settings", "set_print_settings");

    G_DEF_CLASS(GTK_TYPE_PRINT_CAPABILITIES, "Capabilities", RG_TARGET_NAMESPACE);
#endif
}
