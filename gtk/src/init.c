/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  init.c -

  $Author: sakai $
  $Date: 2003/03/14 03:12:20 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "gmodule.h"

extern void Init_gtk_gdk();
extern void Init_gtk_gtk();
extern void Init_gtk_inits();

VALUE treeiter_set_value_table;
GType (*rbgtk_clipboard_get_type)() = NULL;
GtkClipboard* (*rbgtk_get_clipboard)(VALUE obj) = NULL;
GType (*rbgtk_tree_row_reference_get_type)() = NULL;
GtkTreeRowReference *(*rbgtk_get_tree_row_reference)(VALUE obj) = NULL;

static VALUE
require_minor()
{
#if 0
    char feature[32];
    sprintf(feature, "gtk2%d", gtk_minor_version);
    rb_require(feature);
#else
    if (gtk_minor_version == 0) {
        rb_require("gtk20");
    } else {
        rb_require("gtk22");
    }
#endif
    return Qnil;
}

void
Init_gtk2()
{
#ifdef G_THREADS_ENABLED
    g_thread_init (NULL);
    gdk_threads_init();
#endif

    rb_protect(&require_minor, Qnil, NULL);

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
