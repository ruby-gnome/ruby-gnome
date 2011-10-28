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

#include "global.h"

#ifdef HAVE_GTK_UNIX_PRINT
#include <gtk/gtkprintunixdialog.h>

#define RG_TARGET_NAMESPACE cPrintUnixDialog
#define _SELF(s) (GTK_PRINT_UNIX_DIALOG(RVAL2GOBJ(s)))
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

/* Defined as Properties
void        gtk_print_unix_dialog_set_page_setup
                                            (GtkPrintUnixDialog *dialog,
                                             GtkPageSetup *page_setup);
GtkPageSetup* gtk_print_unix_dialog_get_page_setup
                                            (GtkPrintUnixDialog *dialog);
void        gtk_print_unix_dialog_set_current_page
                                            (GtkPrintUnixDialog *dialog,
                                             gint current_page);
gint        gtk_print_unix_dialog_get_current_page
                                            (GtkPrintUnixDialog *dialog);

void        gtk_print_unix_dialog_set_settings
                                            (GtkPrintUnixDialog *dialog,
                                             GtkPrintSettings *settings);
GtkPrintSettings* gtk_print_unix_dialog_get_settings
                                            (GtkPrintUnixDialog *dialog);

GtkPrinter* gtk_print_unix_dialog_get_selected_printer
                                            (GtkPrintUnixDialog *dialog);
*/

static VALUE
pud_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkWidget *dialog;
    VALUE title, parent;

    rb_scan_args(argc, argv, "02", &title, &parent);
    dialog = gtk_print_unix_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title),
                                       RVAL2GOBJ(parent));

    RBGTK_INITIALIZE(self, dialog);
    return Qnil;
}

static VALUE
pud_add_custom_tab(VALUE self, VALUE child, VALUE tab_label)
{
    gtk_print_unix_dialog_add_custom_tab(_SELF(self),
                                         RVAL2GOBJ(child),
                                         RVAL2GOBJ(tab_label));
    return self;
}

static VALUE
pud_set_manual_capabilities(VALUE self, VALUE rb_capabilities)
{
    GtkPrintCapabilities capabilities;

    capabilities = RVAL2GFLAGS(rb_capabilities, GTK_TYPE_PRINT_CAPABILITIES);
    gtk_print_unix_dialog_set_manual_capabilities(_SELF(self), capabilities);
    return self;
}
#endif

void
Init_gtk_print_unix_dialog(void)
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_UNIX_DIALOG,
                                         "PrintUnixDialog", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", pud_initialize, -1);
    rb_define_method(RG_TARGET_NAMESPACE, "add_custom_tab", pud_add_custom_tab, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_manual_capability",
                     pud_set_manual_capabilities, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "settings", "print_settings");
    rb_define_alias(RG_TARGET_NAMESPACE, "set_settings", "set_print_settings");

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkPrintCapabilities */
    G_DEF_CLASS(GTK_TYPE_PRINT_CAPABILITIES, "Capabilities", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PRINT_CAPABILITIES, "GTK_PRINT_");
#endif
}
