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
#include <gtk/gtkpagesetupunixdialog.h>

#define _SELF(s) (GTK_PAGE_SETUP_UNIX_DIALOG(RVAL2GOBJ(s)))

static VALUE
psud_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkWidget *dialog;
    VALUE title, parent;

    rb_scan_args(argc, argv, "02", &title, &parent);
    dialog = gtk_page_setup_unix_dialog_new(RVAL2CSTR_ACCEPT_NIL(title), RVAL2GOBJ(parent));

    RBGTK_INITIALIZE(self, dialog);
    return Qnil;
}

static VALUE
psud_set_page_setup(VALUE self, VALUE page_setup)
{
    gtk_page_setup_unix_dialog_set_page_setup(_SELF(self),
                                              RVAL2GOBJ(page_setup));
    return self;
}

static VALUE
psud_get_page_setup(VALUE self)
{
    return GOBJ2RVAL(gtk_page_setup_unix_dialog_get_page_setup(_SELF(self)));
}

static VALUE
psud_set_print_settings(VALUE self, VALUE print_settings)
{
    gtk_page_setup_unix_dialog_set_print_settings(_SELF(self),
                                                  RVAL2GOBJ(print_settings));
    return Qnil;
}

static VALUE
psud_get_print_settings(VALUE self)
{
    return GOBJ2RVAL(gtk_page_setup_unix_dialog_get_print_settings(_SELF(self)));
}
#endif

void
Init_gtk_page_setup_unix_dialog(void)
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE gPageSetupUnixDialog = G_DEF_CLASS(GTK_TYPE_PAGE_SETUP_UNIX_DIALOG,
                                             "PageSetupUnixDialog", mGtk);

    rb_define_method(gPageSetupUnixDialog, "initialize", psud_initialize, -1);
    rb_define_method(gPageSetupUnixDialog, "set_page_setup",
                     psud_set_page_setup, 1);
    rb_define_method(gPageSetupUnixDialog, "page_setup",
                     psud_get_page_setup, 0);
    rb_define_method(gPageSetupUnixDialog, "set_print_settings",
                     psud_set_print_settings, 1);
    rb_define_method(gPageSetupUnixDialog, "print_settings",
                     psud_get_print_settings, 0);

    G_DEF_SETTERS(gPageSetupUnixDialog);
#endif
}
