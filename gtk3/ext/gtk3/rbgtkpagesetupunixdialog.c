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

#define RG_TARGET_NAMESPACE cPageSetupUnixDialog
#define _SELF(s) (GTK_PAGE_SETUP_UNIX_DIALOG(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkWidget *dialog;
    VALUE title, parent;

    rb_scan_args(argc, argv, "02", &title, &parent);
    dialog = gtk_page_setup_unix_dialog_new(RVAL2CSTR_ACCEPT_NIL(title), RVAL2GOBJ(parent));

    RBGTK_INITIALIZE(self, dialog);
    return Qnil;
}

static VALUE
rg_set_page_setup(VALUE self, VALUE page_setup)
{
    gtk_page_setup_unix_dialog_set_page_setup(_SELF(self),
                                              RVAL2GOBJ(page_setup));
    return self;
}

static VALUE
rg_page_setup(VALUE self)
{
    return GOBJ2RVAL(gtk_page_setup_unix_dialog_get_page_setup(_SELF(self)));
}

static VALUE
rg_set_print_settings(VALUE self, VALUE print_settings)
{
    gtk_page_setup_unix_dialog_set_print_settings(_SELF(self),
                                                  RVAL2GOBJ(print_settings));
    return Qnil;
}

static VALUE
rg_print_settings(VALUE self)
{
    return GOBJ2RVAL(gtk_page_setup_unix_dialog_get_print_settings(_SELF(self)));
}
#endif

void
Init_gtk_page_setup_unix_dialog(VALUE mGtk)
{
#ifdef HAVE_GTK_UNIX_PRINT
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PAGE_SETUP_UNIX_DIALOG,
                                             "PageSetupUnixDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_page_setup, 1);
    RG_DEF_METHOD(page_setup, 0);
    RG_DEF_METHOD(set_print_settings, 1);
    RG_DEF_METHOD(print_settings, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
