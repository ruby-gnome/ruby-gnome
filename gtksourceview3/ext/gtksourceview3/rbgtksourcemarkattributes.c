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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE cMarkAttributes
#define _SELF(self) (RVAL2GTKSOURCEMARKATTRIBUTES(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_source_mark_attributes_new());

    return Qnil;
}

static VALUE
rg_get_tooltip_markup(VALUE self, VALUE mark)
{
    gchar *markup;

    markup = gtk_source_mark_attributes_get_tooltip_markup(_SELF(self),
                                                           RVAL2GTKSOURCEMARK(mark));

    return CSTR2RVAL_FREE(markup);
}

static VALUE
rg_get_tooltip_text(VALUE self, VALUE mark)
{
    gchar *text;

    text = gtk_source_mark_attributes_get_tooltip_text(_SELF(self),
                                                       RVAL2GTKSOURCEMARK(mark));

    return CSTR2RVAL_FREE(text);
}

static VALUE
rg_render_icon(VALUE self, VALUE widget, VALUE size)
{
    GdkPixbuf *icon;

    icon = gtk_source_mark_attributes_render_icon(_SELF(self),
                                                  RVAL2GTKWIDGET(widget),
                                                  NUM2INT(size));

    return GOBJ2RVAL(icon);
}

void
Init_gtksource_markattributes(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_SOURCE_TYPE_MARK_ATTRIBUTES, "MarkAttributes", mGtkSource);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(get_tooltip_markup, 1);
    RG_DEF_METHOD(get_tooltip_text, 1);
    RG_DEF_METHOD(render_icon, 2);
}
