/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cComboBoxEntry

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE model_or_false, text_column;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "02", &model_or_false, &text_column);

    if (rb_obj_is_kind_of(model_or_false, GTYPE2CLASS(GTK_TYPE_TREE_MODEL))){
        widget = gtk_combo_box_entry_new_with_model(
            GTK_TREE_MODEL(RVAL2GOBJ(model_or_false)),
            NUM2INT(text_column));
    } else if (NIL_P(model_or_false) || TYPE(model_or_false) == T_TRUE){
        widget = gtk_combo_box_entry_new_text();
    } else if (TYPE(model_or_false) == T_FALSE){
        widget = gtk_combo_box_entry_new();
    } else {
        rb_raise(rb_eArgError, "invalid 2nd argument %s (except true/false or Gtk::TreeModel)", 
                 rb_class2name(CLASS_OF(model_or_false)));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

#endif

void 
Init_gtk_comboboxentry(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COMBO_BOX_ENTRY, "ComboBoxEntry", mGtk);
    RG_DEF_METHOD(initialize, -1);
#endif
}
