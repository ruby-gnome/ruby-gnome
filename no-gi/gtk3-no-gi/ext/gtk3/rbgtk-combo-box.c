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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cComboBox
#define _SELF(self) (RVAL2GTKCOMBOBOX(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_entry, rb_model, rb_area;
    gboolean entry;
    GtkTreeModel *model;
    GtkCellArea *area;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "entry", &rb_entry,
                     "model", &rb_model,
                     "area", &rb_area,
                     NULL);
    entry = RVAL2CBOOL(rb_entry);
    model = NIL_P(rb_model) ? NULL : RVAL2GTKTREEMODEL(rb_model);
    area = NIL_P(rb_area) ? NULL : RVAL2GTKCELLAREA(rb_area);

    if (entry) {
        if (model)
            widget = gtk_combo_box_new_with_model_and_entry(model);
        else if (area)
            widget = gtk_combo_box_new_with_area_and_entry(area);
        else
            widget = gtk_combo_box_new_with_entry();
    } else {
        if (model)
            widget = gtk_combo_box_new_with_model(model);
        else if (area)
            widget = gtk_combo_box_new_with_area(area);
        else
            widget = gtk_combo_box_new();
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
        val = GTKTREEITER2RVAL(&iter);
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
rg_popup_accessible(VALUE self)
{
    return GOBJ2RVAL(gtk_combo_box_get_popup_accessible(_SELF(self)));
}

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

static VALUE
rg_title(VALUE self)
{
    return CSTR2RVAL(gtk_combo_box_get_title(_SELF(self)));
}

/* TODO: merge to popup? */
static VALUE
rg_popup_for_device(VALUE self, VALUE device)
{
    gtk_combo_box_popup_for_device(_SELF(self), RVAL2GDKDEVICE(device));

    return self;
}

static VALUE
rg_set_title(VALUE self, VALUE title)
{
    gtk_combo_box_set_title(_SELF(self), RVAL2CSTR(title));

    return self;
}

void
Init_gtk_combobox(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COMBO_BOX, "ComboBox", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(active_iter, 0);
    RG_DEF_METHOD(set_active_iter, 1);
    RG_DEF_METHOD(popup_accessible, 0);
    RG_DEF_METHOD(set_row_separator_func, 0);
    RG_DEF_METHOD(title, 0);
    RG_DEF_METHOD(popup_for_device, 1);
    RG_DEF_METHOD(set_title, 1);
}
