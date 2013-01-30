/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cTooltip
/* GTK_TOOLTIP type-cast is private because it should be useless */
#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
rg_set_markup(VALUE self, VALUE markup)
{
    gtk_tooltip_set_markup(_SELF(self), RVAL2CSTR(markup));
    return self;
}

static VALUE
rg_set_text(VALUE self, VALUE text)
{
    gtk_tooltip_set_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_set_icon(VALUE self, VALUE icon)
{
    if (NIL_P(icon)) {
        gtk_tooltip_set_icon(_SELF(self), NULL);
    } else if (TYPE(icon) == T_HASH) {
        VALUE stock_id, icon_name, gicon, size, buffer;
        rbg_scan_options(icon,
                         "stock_id", &stock_id,
                         "icon_name", &icon_name,
                         "gicon", &gicon,
                         "size", &size,
                         NULL);

        if (!NIL_P(stock_id))
            gtk_tooltip_set_icon_from_stock(_SELF(self),
                                            RVAL2GLIBID(stock_id, buffer),
                                            RVAL2GTKICONSIZE(size));
        else if (!NIL_P(icon_name))
            gtk_tooltip_set_icon_from_icon_name(_SELF(self),
                                                RVAL2CSTR(icon_name),
                                                RVAL2GTKICONSIZE(size));
        else if (!NIL_P(gicon))
            gtk_tooltip_set_icon_from_gicon(_SELF(self),
                                            RVAL2GICON(gicon),
                                            RVAL2GTKICONSIZE(size));
        else
            rb_raise(rb_eArgError, "Invalid arguments.");
    } else {
        GType gtype = RVAL2GTYPE(icon);

        if (gtype == GDK_TYPE_PIXBUF)
            gtk_tooltip_set_icon(_SELF(self), RVAL2GDKPIXBUF(icon));
        else
            rb_raise(rb_eArgError, "Invalid arguments.");
    }

    return self;
}

static VALUE
rg_set_custom(VALUE self, VALUE custom_widget)
{
    gtk_tooltip_set_custom(_SELF(self), RVAL2GTKWIDGET(custom_widget));
    return self;
}

void
Init_gtk_tooltip(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOLTIP, "Tooltip", mGtk);
    RG_DEF_METHOD(set_markup, 1);
    RG_DEF_METHOD(set_text, 1);
    RG_DEF_METHOD(set_icon, 1);
    RG_DEF_METHOD(set_custom, 1);
}
