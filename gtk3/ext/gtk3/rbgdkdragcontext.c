/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cDragContext
#define _SELF(self) (RVAL2GDKDRAGCONTEXT(self))

static VALUE
rg_finish(VALUE self, VALUE success, VALUE del, VALUE time)
{
    gtk_drag_finish(_SELF(self),
                    RVAL2CBOOL(success),
                    RVAL2CBOOL(del),
                    NUM2UINT(time));
    return self;
}

static VALUE
rg_source_widget(VALUE self)
{
    return GOBJ2RVAL(gtk_drag_get_source_widget(_SELF(self)));
}

static VALUE
rg_set_icon(VALUE self, VALUE icon)
{
    if (TYPE(icon) == T_HASH) {
        VALUE stock_id, icon_name, gicon, pixbuf, surface, widget, hot_x, hot_y, buffer;
        rbg_scan_options(icon,
                         "stock_id", &stock_id,
                         "icon_name", &icon_name,
                         "gicon", &gicon,
                         "pixbuf", &pixbuf,
                         "surface", &surface,
                         "widget", &widget,
                         "hot_x", &hot_x,
                         "hot_y", &hot_y,
                         NULL);

        if (!NIL_P(stock_id))
            gtk_drag_set_icon_stock(_SELF(self),
                                    RVAL2GLIBID(stock_id, buffer),
                                    NUM2INT(hot_x), NUM2INT(hot_y));
        else if (!NIL_P(icon_name))
            gtk_drag_set_icon_name(_SELF(self),
                                   RVAL2CSTR(icon_name),
                                   NUM2INT(hot_x), NUM2INT(hot_y));
#if GTK_CHECK_VERSION(3, 2, 0)
        else if (!NIL_P(gicon))
            gtk_drag_set_icon_gicon(_SELF(self),
                                    RVAL2GICON(gicon),
                                    NUM2INT(hot_x), NUM2INT(hot_y));
#endif
        else if (!NIL_P(pixbuf))
            gtk_drag_set_icon_pixbuf(_SELF(self),
                                     RVAL2GDKPIXBUF(pixbuf),
                                     NUM2INT(hot_x), NUM2INT(hot_y));
        else if (!NIL_P(surface))
            gtk_drag_set_icon_surface(_SELF(self),
                                      RVAL2CRSURFACE(surface));
        else if (!NIL_P(widget))
            gtk_drag_set_icon_widget(_SELF(self),
                                     RVAL2GTKWIDGET(widget),
                                     NUM2INT(hot_x), NUM2INT(hot_y));
        else
            rb_raise(rb_eArgError, "Invalid arguments.");
    } else {
        gtk_drag_set_icon_surface(_SELF(self), RVAL2CRSURFACE(icon));
    }

    return self;
}

static VALUE
rg_set_icon_default(VALUE self)
{
    gtk_drag_set_icon_default(_SELF(self));
    return self;
}

void
Init_gdk_dragcontext(void)
{
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS(GDK_TYPE_DRAG_CONTEXT);

    RG_DEF_METHOD(finish, 3);
    RG_DEF_METHOD(source_widget, 0);
    RG_DEF_METHOD(set_icon, 1);
    RG_DEF_METHOD(set_icon_default, 0);
}
