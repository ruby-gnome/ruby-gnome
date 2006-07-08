/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintunixdialog.c -

  $Author: ktou $
  $Date: 2006/07/08 08:36:07 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#ifdef HAVE_GTK_GTKPRINTUNIXDIALOG_H
#include <gtk/gtkprintunixdialog.h>

#define _SELF(s) (GTK_PRINT_UNIX_DIALOG(RVAL2GOBJ(s)))
#ifndef GTK_TYPE_PRINT_CAPABILITIES
#  define GTK_TYPE_PRINT_CAPABILITIES (gtk_print_capabilities_get_type())
#endif

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
    return Qnil;
}

static VALUE
pud_set_manual_capabilities(VALUE self, VALUE rb_capabilities)
{
    GtkPrintCapabilities capabilities;
    capabilities = RVAL2GFLAGS(rb_capabilities, GTK_TYPE_PRINT_CAPABILITIES);
    gtk_print_unix_dialog_set_manual_capabilities(_SELF(self), capabilities);
    return Qnil;
}
#endif

void
Init_gtk_printunixdialog()
{
#ifdef HAVE_GTK_GTKPRINTUNIXDIALOG_H
    VALUE gPrintUnixDialog = G_DEF_CLASS(GTK_TYPE_PRINT_UNIX_DIALOG,
                                         "PrintUnixDialog", mGtk);

    rb_define_method(gPrintUnixDialog, "initialize", pud_initialize, -1);
    rb_define_method(gPrintUnixDialog, "add_custom_tab", pud_add_custom_tab, 2);
    rb_define_method(gPrintUnixDialog, "set_manual_capability",
                     pud_set_manual_capabilities, 1);
    rb_define_alias(gPrintUnixDialog, "settings", "print_settings");
    rb_define_alias(gPrintUnixDialog, "set_settings", "set_print_settings");

    G_DEF_SETTERS(gPrintUnixDialog);
#endif
}
