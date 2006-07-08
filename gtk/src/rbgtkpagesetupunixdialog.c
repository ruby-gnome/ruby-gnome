/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkpagesetupunixdialog.c -

  $Author: ktou $
  $Date: 2006/07/08 08:50:07 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#ifdef HAVE_GTK_GTKPAGESETUPUNIXDIALOG_H
#include <gtk/gtkpagesetupunixdialog.h>

#define _SELF(s) (GTK_PAGE_SETUP_UNIX_DIALOG(RVAL2GOBJ(s)))

static VALUE
psud_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkWidget *dialog;
    VALUE rb_title, parent;
    gchar *title;

    rb_scan_args(argc, argv, "02", &rb_title, &parent);
    title = NIL_P(rb_title) ? NULL : RVAL2CSTR(rb_title);
    dialog = gtk_page_setup_unix_dialog_new(title, RVAL2GOBJ(parent));

    RBGTK_INITIALIZE(self, dialog);
    return Qnil;
}

static VALUE
psud_set_page_setup(VALUE self, VALUE page_setup)
{
    gtk_page_setup_unix_dialog_set_page_setup(_SELF(self),
                                              RVAL2GOBJ(page_setup));
    return Qnil;
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
Init_gtk_page_setup_unix_dialog()
{
#ifdef HAVE_GTK_GTKPAGESETUPUNIXDIALOG_H
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
