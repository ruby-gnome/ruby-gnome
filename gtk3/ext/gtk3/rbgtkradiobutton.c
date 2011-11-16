/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cRadioButton

static VALUE RG_TARGET_NAMESPACE;

#define _GROUP(s) (NIL_P(s) ? NULL : GTK_RADIO_BUTTON(RVAL2GOBJ(s)))

static GtkWidget*
create_button(VALUE group, VALUE label, VALUE use_underline)
{
    GtkWidget* widget = NULL;

    if (TYPE(label) == T_STRING){
        if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)){
            widget = gtk_radio_button_new_with_mnemonic_from_widget(_GROUP(group), RVAL2CSTR(label));
        } else {
            widget = gtk_radio_button_new_with_label_from_widget(_GROUP(group), RVAL2CSTR(label));
        }
    } else if (TYPE(label) == T_SYMBOL){
        widget = gtk_radio_button_new_with_label_from_widget(_GROUP(group), rb_id2name(SYM2ID(label)));
        gtk_button_set_use_stock(GTK_BUTTON(widget), TRUE);
    } else if (NIL_P(label)){
        widget = gtk_radio_button_new_from_widget(_GROUP(group));
    } else {
        rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                         rb_class2name(CLASS_OF(label)));
    }
    return widget;
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE group_or_label, label_or_use_underline, use_underline;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "03", &group_or_label, &label_or_use_underline, &use_underline) > 0) {
        if (rb_obj_is_kind_of(group_or_label, RG_TARGET_NAMESPACE)){
            widget = create_button(group_or_label, label_or_use_underline, use_underline);
        } else {
            widget = create_button(Qnil, group_or_label, label_or_use_underline);
        }
    } else {
        widget = gtk_radio_button_new(NULL);
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
rg_group(VALUE self)
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_radio_button_get_group(GTK_RADIO_BUTTON(RVAL2GOBJ(self))));
}

void 
Init_gtk_radio_button(VALUE mGtk)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RADIO_BUTTON, "RadioButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(group, 0);
}
