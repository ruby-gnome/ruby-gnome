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

#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */

#define RG_TARGET_NAMESPACE cOptionMenu

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_option_menu_new());
    return Qnil;
}

static VALUE
rg_remove_menu(VALUE self)
{
    gtk_option_menu_remove_menu(GTK_OPTION_MENU(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_set_history(VALUE self, VALUE index)
{
    gtk_option_menu_set_history(GTK_OPTION_MENU(RVAL2GOBJ(self)), NUM2INT(index));
    return self;
}

static VALUE
rg_history(VALUE self)
{
    return INT2NUM(gtk_option_menu_get_history(GTK_OPTION_MENU(RVAL2GOBJ(self))));
}

#endif

void 
Init_gtk_option_menu(VALUE mGtk)
{
#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_OPTION_MENU, "OptionMenu", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(remove_menu, 0);
    RG_DEF_METHOD(set_history, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "history");
    RG_DEF_METHOD(history, 0);
#endif
}
