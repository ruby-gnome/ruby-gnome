/* $Id: rbgtk-pixmap-menuitem.c,v 1.1 2002/05/19 12:29:55 mutoh Exp $ */

/* Gtk::PixmapMenuItem widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gPixmapMenuItem;

static VALUE
pmenuitem_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_pixmap_menu_item_new());
    return Qnil;
}

static VALUE
pmenuitem_set_pixmap(self, pixmap)
    VALUE self, pixmap;
{
    GtkPixmapMenuItem* pMenuItem;
    GtkWidget* c_pixmap;

    pMenuItem = GTK_PIXMAP_MENU_ITEM(get_widget(self));
    c_pixmap = get_widget(pixmap);
    gtk_pixmap_menu_item_set_pixmap(pMenuItem, c_pixmap);
    return Qnil;
}

void
Init_gtk_pixmap_menu_item()
{
    gPixmapMenuItem = rb_define_class_under(mGtk, "PixmapMenuItem", gMenuItem);
    
    rb_define_method(gPixmapMenuItem, "initialize", pmenuitem_initialize, 0);
    rb_define_method(gPixmapMenuItem, "pixmap=", pmenuitem_set_pixmap, 1);
    rb_define_alias(gPixmapMenuItem, "set_pixmap", "pixmap=");
}
