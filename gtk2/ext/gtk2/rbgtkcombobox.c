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

#define RG_TARGET_NAMESPACE cComboBox
#define _SELF(self) (GTK_COMBO_BOX(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE model_or_false;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "01", &model_or_false);

    if (rb_obj_is_kind_of(model_or_false, GTYPE2CLASS(GTK_TYPE_TREE_MODEL))){
        widget = gtk_combo_box_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model_or_false)));
    } else if (NIL_P(model_or_false) || TYPE(model_or_false) == T_TRUE){
        widget = gtk_combo_box_new_text();
    } else if (TYPE(model_or_false) == T_FALSE){
        widget = gtk_combo_box_new();
    } else {
        rb_raise(rb_eArgError, "invalid 2nd argument %s (except true/false or Gtk::TreeModel)", 
                 rb_class2name(CLASS_OF(model_or_false)));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
rg_active_iter(VALUE self)
{
    GtkTreeIter iter;
    VALUE val = Qnil;
    gboolean ret = gtk_combo_box_get_active_iter(_SELF(self), &iter);
    if (ret){
        iter.user_data3 = (gpointer)gtk_combo_box_get_model(_SELF(self));
        val = BOXED2RVAL(&iter, GTK_TYPE_TREE_ITER);
    }
    return val;
}

static VALUE
rg_set_active_iter(VALUE self, VALUE iter)
{
    gtk_combo_box_set_active_iter(_SELF(self), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_append_text(VALUE self, VALUE text)
{
    gtk_combo_box_append_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_insert_text(VALUE self, VALUE position, VALUE text)
{
    gtk_combo_box_insert_text(_SELF(self), NUM2INT(position), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_prepend_text(VALUE self, VALUE text)
{
    gtk_combo_box_prepend_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_remove_text(VALUE self, VALUE position)
{
    gtk_combo_box_remove_text(_SELF(self), NUM2INT(position));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)

static VALUE
rg_active_text(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_combo_box_get_active_text(_SELF(self)));
}

static VALUE
rg_popup_accessible(VALUE self)
{
    return GOBJ2RVAL(gtk_combo_box_get_popup_accessible(_SELF(self)));
}

/* How can I implement this?
GtkTreeViewRowSeparatorFunc gtk_combo_box_get_row_separator_func
                                            (GtkComboBox *combo_box);
*/

static gboolean
row_separator_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer *func)
{  
    iter->user_data3 = model;
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(model),
                      GTKTREEITER2RVAL(iter)));
}

static VALUE
rg_set_row_separator_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_combo_box_set_row_separator_func(_SELF(self), 
                                         (GtkTreeViewRowSeparatorFunc)row_separator_func,
                                         (gpointer)func, NULL);
    return self;
}

#endif
#endif

void 
Init_gtk_combobox(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COMBO_BOX, "ComboBox", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(active_iter, 0);
    RG_DEF_METHOD(set_active_iter, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "active_iter");
    RG_DEF_METHOD(append_text, 1);
    RG_DEF_METHOD(insert_text, 2);
    RG_DEF_METHOD(prepend_text, 1);
    RG_DEF_METHOD(remove_text, 1);

#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(active_text, 0);
    RG_DEF_METHOD(popup_accessible, 0);
    RG_DEF_METHOD(set_row_separator_func, 0);
#endif
#endif
}
