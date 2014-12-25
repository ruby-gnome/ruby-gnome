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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cStatusIcon
#define _SELF(w) (RVAL2GTKSTATUSICON(w))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg;
    GtkStatusIcon *icon = NULL;

    rb_scan_args(argc, argv, "01", &arg);
    if (NIL_P(arg)) {
        icon = gtk_status_icon_new();
    } else if (TYPE(arg) == T_HASH) {
        VALUE stock, icon_name, gicon, file, pixbuf, buffer;
        rbg_scan_options(arg,
                         "stock", &stock,
                         "icon_name", &icon_name,
                         "gicon", &gicon,
                         "file", &file,
                         "pixbuf", &pixbuf,
                         NULL);

        if (!NIL_P(stock))
            icon = gtk_status_icon_new_from_stock(RVAL2GLIBID(stock, buffer));
        else if (!NIL_P(icon_name))
            icon = gtk_status_icon_new_from_icon_name(RVAL2CSTR(icon_name));
        else if (!NIL_P(gicon))
            icon = gtk_status_icon_new_from_gicon(RVAL2GICON(gicon));
        else if (!NIL_P(file))
            icon = gtk_status_icon_new_from_file(RVAL2CSTR(file));
        else if (!NIL_P(pixbuf))
            icon = gtk_status_icon_new_from_pixbuf(RVAL2GDKPIXBUF(pixbuf));
    } else {
        GType gtype = RVAL2GTYPE(arg);

        if (gtype == GDK_TYPE_PIXBUF)
            icon = gtk_status_icon_new_from_pixbuf(RVAL2GDKPIXBUF(arg));
        else if (g_type_is_a(gtype, G_TYPE_ICON))
            icon = gtk_status_icon_new_from_gicon(RVAL2GICON(arg));
    }
    if (!icon)
        rb_raise(rb_eArgError, "Invalid arguments.");

    G_INITIALIZE(self, icon);

    return Qnil;
}

/* deprecated
static VALUE
rg_set_tooltip(VALUE self, VALUE tooltip_text)
{
    gtk_status_icon_set_tooltip(_SELF(self), RVAL2CSTR(tooltip_text));
    return self;
}
*/

static VALUE
rg_position_menu(VALUE self, VALUE menu)
{
    gint x, y;
    gboolean push_in;

    gtk_status_icon_position_menu(RVAL2GTKMENU(menu), &x, &y, &push_in,
                                  (gpointer)(_SELF(self)));
    return rb_ary_new3(3, INT2NUM(x), INT2NUM(y), CBOOL2RVAL(push_in));
}

static VALUE
rg_geometry(VALUE self)
{
    GdkScreen* screen;
    GdkRectangle area;
    GtkOrientation orientation;
    gboolean ret = gtk_status_icon_get_geometry(_SELF(self), &screen, 
                                                &area, &orientation);
    if (ret) {
        return rb_ary_new3(3, GOBJ2RVAL(screen), 
                           GDKRECTANGLE2RVAL(&area),
                           GTKORIENTATION2RVAL(orientation));
    } else {
        return rb_ary_new3(3, Qnil, Qnil, Qnil);
    }
}

void
Init_gtk_status_icon(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STATUS_ICON, "StatusIcon", mGtk);

    RG_DEF_METHOD(initialize, -1);
/* deprecated
    RG_DEF_METHOD(set_tooltip, 1);
*/
    RG_DEF_METHOD(position_menu, 1);
    RG_DEF_METHOD(geometry, 0);

    RG_REG_GLIBID_SETTER("stock");
}
