/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cStatusIcon
#define _SELF(w) (GTK_STATUS_ICON(RVAL2GOBJ(w)))

static VALUE
si_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_status_icon_new());
    return Qnil;
}

/* Don't implement this. Use Gtk::StatusIcon.new.set_foo(v) instead.
GtkStatusIcon* gtk_status_icon_new_from_pixbuf
                                            (GdkPixbuf *pixbuf);
GtkStatusIcon* gtk_status_icon_new_from_file
                                            (const gchar *filename);
GtkStatusIcon* gtk_status_icon_new_from_stock
                                            (const gchar *stock_id);
GtkStatusIcon* gtk_status_icon_new_from_icon_name
                                            (const gchar *icon_name);
*/

static VALUE
si_set_tooltip(VALUE self, VALUE tooltip_text)
{
    gtk_status_icon_set_tooltip(_SELF(self), RVAL2CSTR(tooltip_text));
    return self;
}

static VALUE
si_position_menu(VALUE self, VALUE menu)
{
    gint x, y;
    gboolean push_in;

    gtk_status_icon_position_menu(GTK_MENU(RVAL2GOBJ(menu)), &x, &y, &push_in,
                                  (gpointer)(_SELF(self)));
    return rb_ary_new3(3, INT2NUM(x), INT2NUM(y), CBOOL2RVAL(push_in));
}

static VALUE
si_get_geometry(VALUE self)
{
    GdkScreen* screen;
    GdkRectangle area;
    GtkOrientation orientation;
    gboolean ret = gtk_status_icon_get_geometry(_SELF(self), &screen, 
                                                &area, &orientation);
    if (ret) {
        return rb_ary_new3(3, GOBJ2RVAL(screen), 
                           BOXED2RVAL(&area, GDK_TYPE_RECTANGLE),
                           GENUM2RVAL(orientation, GTK_TYPE_ORIENTATION));
    } else {
        return rb_ary_new3(3, Qnil, Qnil, Qnil);
    }
}
#endif

void 
Init_gtk_status_icon(void)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STATUS_ICON, "StatusIcon", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", si_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_tooltip", si_set_tooltip, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "tooltip");
    rb_define_method(RG_TARGET_NAMESPACE, "position_menu", si_position_menu, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "geometry", si_get_geometry, 0);
#endif
}
