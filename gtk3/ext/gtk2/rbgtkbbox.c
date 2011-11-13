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

#define RG_TARGET_NAMESPACE cButtonBox

static VALUE
rg_set_child_secondary(VALUE self, VALUE child, VALUE is_secondary)
{
    gtk_button_box_set_child_secondary(GTK_BUTTON_BOX(RVAL2GOBJ(self)), 
                                       GTK_WIDGET(RVAL2GOBJ(child)),
                                       RVAL2CBOOL(is_secondary));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_get_child_secondary(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_button_box_get_child_secondary(GTK_BUTTON_BOX(RVAL2GOBJ(self)), 
                                                         GTK_WIDGET(RVAL2GOBJ(child))));
}
#endif

void 
Init_gtk_button_box(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BUTTON_BOX, "ButtonBox", mGtk);
    RG_DEF_METHOD(set_child_secondary, 2);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(get_child_secondary, 1);
#endif
    /* GtkButtonBoxStyle(General constants) */
    G_DEF_CLASS(GTK_TYPE_BUTTON_BOX_STYLE, "Style", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_BUTTON_BOX_STYLE, "GTK_BUTTONBOX_");
}
