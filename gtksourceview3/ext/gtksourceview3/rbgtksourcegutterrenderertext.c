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

#define RG_TARGET_NAMESPACE cGutterRendererText
#define _SELF(self) (RVAL2GTKSOURCEGUTTERRENDERERTEXT(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_source_gutter_renderer_text_new());

    return Qnil;
}

static VALUE
rg_set_markup(VALUE self, VALUE markup)
{
    StringValue(markup);
    gtk_source_gutter_renderer_text_set_markup(_SELF(self),
                                               RSTRING_PTR(markup),
                                               RSTRING_LEN(markup));

    return self;
}

static VALUE
rg_set_text(VALUE self, VALUE text)
{
    StringValue(text);
    gtk_source_gutter_renderer_text_set_text(_SELF(self),
                                             RSTRING_PTR(text),
                                             RSTRING_LEN(text));

    return self;
}

void
Init_gtksource_gutterrenderertext(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_SOURCE_TYPE_GUTTER_RENDERER_TEXT, "GutterRendererText", mGtkSource);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_markup, 1);
    RG_DEF_METHOD(set_text, 1);
}
