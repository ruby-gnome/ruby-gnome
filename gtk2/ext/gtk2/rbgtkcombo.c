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

static VALUE
combo_set_popdown_strings(VALUE self, VALUE ary)
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
combo_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE ary;
    GtkWidget* widget = NULL; 

    rb_scan_args(argc, argv, "01", &ary);
    widget = gtk_combo_new();

    RBGTK_INITIALIZE(self, widget);

    if (! NIL_P(ary)){
        combo_set_popdown_strings(self, ary);
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
combo_set_use_arrows(VALUE self, VALUE val)
{
    gtk_combo_set_use_arrows(GTK_COMBO(RVAL2GOBJ(self)),
			     RVAL2CBOOL(val));
    return self;
}

static VALUE
combo_set_use_arrows_always(VALUE self, VALUE val)
{
    gtk_combo_set_use_arrows_always(GTK_COMBO(RVAL2GOBJ(self)),
                                    RVAL2CBOOL(val));
    return self;
}

static VALUE
combo_disable_activate(VALUE self)
{
    gtk_combo_disable_activate(GTK_COMBO(RVAL2GOBJ(self)));
    return self;
}

static VALUE
combo_entry(VALUE self)
{
    return GOBJ2RVAL(GTK_COMBO(RVAL2GOBJ(self))->entry);
}
#endif

void 
Init_gtk_combo(void)
{
#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */
    VALUE gCombo = G_DEF_CLASS(GTK_TYPE_COMBO, "Combo", mGtk);

    rb_define_method(gCombo, "initialize", combo_initialize, -1);
    G_REPLACE_SET_PROPERTY(gCombo, "value_in_list", combo_set_val_in_list, 2);
    rb_define_method(gCombo, "set_use_arrows", combo_set_use_arrows, 1);
    rb_define_method(gCombo, "set_use_arrows_always", combo_set_use_arrows_always, 1);
    rb_define_method(gCombo, "set_popdown_strings", combo_set_popdown_strings, 1);
    rb_define_method(gCombo, "disable_activate", combo_disable_activate, 0);

    rb_define_method(gCombo, "entry", combo_entry, 0);

    G_DEF_SETTERS(gCombo);
#endif
}

