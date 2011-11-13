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

#include "global.h"

#if GTK_CHECK_VERSION(2,12,0)

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
    gtk_tooltip_set_icon(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(icon)));
    return self;
}

static VALUE
rg_set_icon_from_stock(VALUE self, VALUE stock_id, VALUE size)
{
    gtk_tooltip_set_icon_from_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)), RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));
    return self;
}

static VALUE
rg_set_custom(VALUE self, VALUE custom_widget)
{
    gtk_tooltip_set_custom(_SELF(self), GTK_WIDGET(RVAL2GOBJ(custom_widget)));
    return self;
}

#endif

void 
Init_gtk_tooltip(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,12,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOLTIP, "Tooltip", mGtk);
    RG_DEF_METHOD(set_markup, 1);
    RG_DEF_METHOD(set_text, 1);
    RG_DEF_METHOD(set_icon, 1);
    RG_DEF_METHOD(set_icon_from_stock, 2);
    RG_DEF_METHOD(set_custom, 1);
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
