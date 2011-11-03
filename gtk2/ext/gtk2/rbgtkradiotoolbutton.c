/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cRadioToolButton
#define _SELF(self) (GTK_RADIO_TOOL_BUTTON(RVAL2GOBJ(self)))

static VALUE RG_TARGET_NAMESPACE;

struct rbgtk_rval2gtkradiotoolbuttongslist_args {
    VALUE ary;
    long n;
    GSList *result;
};

static VALUE
rbgtk_rval2gtkradiotoolbuttongslist_body(VALUE value)
{
    long i;
    struct rbgtk_rval2gtkradiotoolbuttongslist_args *args = (struct rbgtk_rval2gtkradiotoolbuttongslist_args *)value;

    for (i = 0; i < args->n; i++)
        args->result = g_slist_append(args->result, GTK_RADIO_MENU_ITEM(RVAL2GOBJ(RARRAY_PTR(args->ary)[i])));

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgtk_rval2gtkradiotoolbuttongslist_rescue(VALUE value)
{
    g_slist_free(((struct rbgtk_rval2gtkradiotoolbuttongslist_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GSList *
rbgtk_rval2gtkradiotoolbuttongslist(VALUE value)
{
    struct rbgtk_rval2gtkradiotoolbuttongslist_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(rbgtk_rval2gtkradiotoolbuttongslist_body, (VALUE)&args,
              rbgtk_rval2gtkradiotoolbuttongslist_rescue, (VALUE)&args);

    return args.result;
}

#define RVAL2GTKRADIOTOOLBUTTONGSLIST(value) rbgtk_rval2gtkradiotoolbuttongslist(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE group_or_stock_id, rbstock_id;
    GtkToolItem *widget;

    if (rb_scan_args(argc, argv, "02", &group_or_stock_id, &rbstock_id) > 0) {
        GSList *group = NULL;
        const gchar *stock_id = TYPE(rbstock_id) == T_SYMBOL ?
            rb_id2name(SYM2ID(rbstock_id)) :
            RVAL2CSTR_ACCEPT_NIL(rbstock_id);

        if (TYPE(group_or_stock_id) == T_ARRAY)
            /* TODO: This has a potential for leaking. */
            group = RVAL2GTKRADIOTOOLBUTTONGSLIST(group_or_stock_id);
        else if (rb_obj_is_kind_of(group_or_stock_id, RG_TARGET_NAMESPACE))
            group = gtk_radio_tool_button_get_group(_SELF(group_or_stock_id));

        if (stock_id == NULL)
            widget = gtk_radio_tool_button_new(group);
        else
            widget = gtk_radio_tool_button_new_from_stock(group, stock_id);
    } else {
        widget = gtk_radio_tool_button_new(NULL);
    }

    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rbtn_get_group(VALUE self)
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_radio_tool_button_get_group(_SELF(self)));
}

static VALUE
rbtn_set_group(VALUE self, VALUE rbgroup)
{
    GtkRadioToolButton *button = _SELF(self);
    GSList *group = TYPE(rbgroup) == T_ARRAY ?
        /* TODO: This might leak. */
        RVAL2GTKRADIOTOOLBUTTONGSLIST(rbgroup) :
        gtk_radio_tool_button_get_group(_SELF(rbgroup));

    gtk_radio_tool_button_set_group(button, group);

    return self;
}

#endif

void 
Init_gtk_radiotoolbutton(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RADIO_TOOL_BUTTON, "RadioToolButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
    G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "group", rbtn_get_group, 0);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "group", rbtn_set_group, 1);
#endif
}
