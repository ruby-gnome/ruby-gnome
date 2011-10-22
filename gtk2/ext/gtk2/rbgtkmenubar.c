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

static VALUE
mbar_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_menu_bar_new());
    return Qnil;
}

/* Defined as property
void        gtk_menu_bar_set_pack_direction (GtkMenuBar *menubar,
                                             GtkPackDirection pack_dir);
GtkPackDirection gtk_menu_bar_get_pack_direction
                                            (GtkMenuBar *menubar);
void        gtk_menu_bar_set_child_pack_direction
                                            (GtkMenuBar *menubar,
                                             GtkPackDirection child_pack_dir);
GtkPackDirection gtk_menu_bar_get_child_pack_direction
                                            (GtkMenuBar *menubar);
*/

void 
Init_gtk_menu_bar(void)
{
    VALUE gMenuBar = G_DEF_CLASS(GTK_TYPE_MENU_BAR, "MenuBar", mGtk);
    rb_define_method(gMenuBar, "initialize", mbar_initialize, 0);

#if GTK_CHECK_VERSION(2,8,0)
    /* GtkPackDirection */
    G_DEF_CLASS(GTK_TYPE_PACK_DIRECTION, "PackDirection", gMenuBar);
    G_DEF_CONSTANTS(gMenuBar, GTK_TYPE_PACK_DIRECTION, "GTK_");
#endif
}
