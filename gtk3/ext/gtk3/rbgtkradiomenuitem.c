/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi,
 *                          dellin 
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

#define RG_TARGET_NAMESPACE cRadioMenuItem

struct rbgtk_rval2gtkradiomenuitemgslist_args {
    VALUE ary;
    long n;
    GSList *result;
};

static VALUE
rbgtk_rval2gtkradiomenuitemgslist_body(VALUE value)
{
    long i;
    struct rbgtk_rval2gtkradiomenuitemgslist_args *args = (struct rbgtk_rval2gtkradiomenuitemgslist_args *)value;

    for (i = 0; i < args->n; i++)
        args->result = g_slist_append(args->result, GTK_RADIO_MENU_ITEM(RVAL2GOBJ(RARRAY_PTR(args->ary)[i])));

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgtk_rval2gtkradiomenuitemgslist_rescue(VALUE value)
{
    g_slist_free(((struct rbgtk_rval2gtkradiomenuitemgslist_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GSList *
rbgtk_rval2gtkradiomenuitemgslist(VALUE value)
{
    struct rbgtk_rval2gtkradiomenuitemgslist_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(rbgtk_rval2gtkradiomenuitemgslist_body, (VALUE)&args,
              rbgtk_rval2gtkradiomenuitemgslist_rescue, (VALUE)&args);

    return args.result;
}

#define RVAL2GTKRADIOMENUITEMGSLIST(value) rbgtk_rval2gtkradiomenuitemgslist(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2, arg3;
    GtkWidget *widget;
    GSList *list = NULL;
    const gchar *label = NULL;
    const gchar *mnemonic = NULL;

    if (rb_scan_args(argc, argv, "03", &arg1, &arg2, &arg3) > 0 &&
        TYPE(arg1) == T_STRING) {
        if (NIL_P(arg2) || RVAL2CBOOL(arg2))
            mnemonic = RVAL2CSTR(arg1);
        else
            label = RVAL2CSTR(arg1);
    } else {
        if (!NIL_P(arg2)) {
            if (NIL_P(arg3) || RVAL2CBOOL(arg3))
                mnemonic = RVAL2CSTR(arg2);
            else
                label = RVAL2CSTR(arg2);
        }

        if (rb_obj_is_kind_of(arg1, GTYPE2CLASS(GTK_TYPE_RADIO_MENU_ITEM)))
            list = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(RVAL2GOBJ(arg1)));
        else if (TYPE(arg1) == T_ARRAY)
            /* TODO: This might leak. */
            list = RVAL2GTKRADIOMENUITEMGSLIST(arg1);
        else if (!NIL_P(arg1))
            rb_raise(rb_eArgError, "invalid argument %s (expect Array or Gtk::RadioMenuItem)", 
                     rb_class2name(CLASS_OF(arg1)));
    }

    if (label != NULL)
        widget = gtk_radio_menu_item_new_with_label(list, label);
    else if (mnemonic != NULL)
        widget = gtk_radio_menu_item_new_with_mnemonic(list, mnemonic);
    else
        widget = gtk_radio_menu_item_new(list);

    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rmitem_get_group(VALUE self)
{
    /* Owened by GTK+ */
    return GSLIST2ARY(gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(RVAL2GOBJ(self))));
}

static VALUE
rmitem_set_group(VALUE self, VALUE grp_ary)
{
    GtkRadioMenuItem *rmitem2add, *rmitem_orig;
    GSList *group;

    rmitem2add = GTK_RADIO_MENU_ITEM(RVAL2GOBJ(self));
    rmitem_orig = GTK_RADIO_MENU_ITEM(RVAL2GOBJ(rb_ary_entry(grp_ary, 0)));
    group = gtk_radio_menu_item_group(rmitem_orig);

    gtk_radio_menu_item_set_group(rmitem2add, group);

    return GSLIST2ARY(gtk_radio_menu_item_get_group(rmitem2add));
}

void 
Init_gtk_radio_menu_item(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RADIO_MENU_ITEM, "RadioMenuItem", mGtk);

    RG_DEF_METHOD(initialize, -1);
    G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "group", rmitem_get_group, 0);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "group", rmitem_set_group, 1);
}
