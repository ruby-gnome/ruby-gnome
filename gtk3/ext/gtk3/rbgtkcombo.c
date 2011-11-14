/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

#ifndef GTK_DISABLE_DEPRECATED /* Since 2.4 */

#define RG_TARGET_NAMESPACE cCombo

static VALUE
rg_set_popdown_strings(VALUE self, VALUE ary)
{
    int i;
    GList *glist = NULL;

    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY_LEN(ary); i++) {
        glist = g_list_append(glist, (gpointer)RVAL2CSTR(RARRAY_PTR(ary)[i]));
    }
    gtk_combo_set_popdown_strings(GTK_COMBO(RVAL2GOBJ(self)), glist);
    g_list_free(glist);
    return self;
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE ary;
    GtkWidget* widget = NULL; 

    rb_scan_args(argc, argv, "01", &ary);
    widget = gtk_combo_new();

    RBGTK_INITIALIZE(self, widget);

    if (! NIL_P(ary)){
        rg_set_popdown_strings(self, ary);
    }

    return Qnil;
}

static VALUE
combo_set_val_in_list(VALUE self, VALUE val, VALUE ok)
{
    gtk_combo_set_value_in_list(GTK_COMBO(RVAL2GOBJ(self)),
                                RVAL2CBOOL(val), RVAL2CBOOL(ok));
    return self;
}

static VALUE
rg_set_use_arrows(VALUE self, VALUE val)
{
    gtk_combo_set_use_arrows(GTK_COMBO(RVAL2GOBJ(self)),
                             RVAL2CBOOL(val));
    return self;
}

static VALUE
rg_set_use_arrows_always(VALUE self, VALUE val)
{
    gtk_combo_set_use_arrows_always(GTK_COMBO(RVAL2GOBJ(self)),
                                    RVAL2CBOOL(val));
    return self;
}

static VALUE
rg_disable_activate(VALUE self)
{
    gtk_combo_disable_activate(GTK_COMBO(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_entry(VALUE self)
{
    return GOBJ2RVAL(GTK_COMBO(RVAL2GOBJ(self))->entry);
}
#endif

void 
Init_gtk_combo(VALUE mGtk)
{
#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COMBO, "Combo", mGtk);

    RG_DEF_METHOD(initialize, -1);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "value_in_list", combo_set_val_in_list, 2);
    RG_DEF_METHOD(set_use_arrows, 1);
    RG_DEF_METHOD(set_use_arrows_always, 1);
    RG_DEF_METHOD(set_popdown_strings, 1);
    RG_DEF_METHOD(disable_activate, 0);

    RG_DEF_METHOD(entry, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
