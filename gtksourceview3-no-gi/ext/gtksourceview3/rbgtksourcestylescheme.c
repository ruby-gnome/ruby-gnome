/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

/* Module: Gtk::SourceStyleScheme
 */

#define RG_TARGET_NAMESPACE cStyleScheme
#define _SELF(self) (RVAL2GTKSOURCESTYLESCHEME(self))

/* Method: authors
 *
 * Returns: a list of authors for the given style scheme.
 */
static VALUE
rg_authors(VALUE self)
{
    VALUE ary;
    const gchar * const * authors =
            gtk_source_style_scheme_get_authors (_SELF (self));
    if (!authors)
        return Qnil;

    ary = rb_ary_new();
    while (*authors){
        rb_ary_push(ary, CSTR2RVAL(*authors));
        authors++;
    }
    return ary;
}

/* Method: get_style(style_id)
 * style_name: the name of a style.
 *
 * Gets the tag associated with the given style_name in the style scheme.
 *
 * Returns: Gtk::SourceStyle
 */
static VALUE
rg_get_style(VALUE self, VALUE style_name)
{
    return GOBJ2RVAL(gtk_source_style_scheme_get_style(_SELF(self),
                                                       RVAL2CSTR(style_name)));
}

void
Init_gtksource_stylescheme (VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GTK_SOURCE_TYPE_STYLE_SCHEME, "StyleScheme", mGtkSource);

    RG_DEF_METHOD(authors, 0);
    RG_DEF_METHOD(get_style, 1);
}
