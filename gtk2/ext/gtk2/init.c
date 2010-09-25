/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  init.c -

  $Author: mutoh $
  $Date: 2006/10/21 16:58:00 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

void
Init_gtk2()
{
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
