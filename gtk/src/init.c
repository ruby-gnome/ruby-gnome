/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  init.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE treeiter_set_value_table;
GType (*rbgtk_clipboard_get_type)() = NULL;
GtkClipboard* (*rbgtk_get_clipboard)(VALUE obj) = NULL;
GType (*rbgtk_tree_row_reference_get_type)() = NULL;
GtkTreeRowReference *(*rbgtk_get_tree_row_reference)(VALUE obj) = NULL;

void
Init_gtk2()
{
    if (gtk_minor_version == 0) {
        rb_require("gtk20");
    } else {
        rb_require("gtk22");
    }

    gtk_set_locale();

    /*
     * For Gtk::TreeModel, Gtk::TreeIter. 
     * They should be initialized on this timing.
     */
    treeiter_set_value_table = rb_hash_new();
    rb_global_variable(&treeiter_set_value_table);

    Init_gtk_gdk();
    Init_gtk_gtk();

    Init_gtk_inits();
}
