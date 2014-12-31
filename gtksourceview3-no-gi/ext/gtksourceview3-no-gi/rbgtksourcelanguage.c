/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs, based on gtktextview.c by Masao Mutoh
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

/* Class: Gtk::SourceLanguage
 * Source language.
 */

#define RG_TARGET_NAMESPACE cLanguage
#define _SELF(self) (RVAL2GTKSOURCELANGUAGE(self))

/* Method: get_metadata(name)
 * name: the metadata property name (string)
 * Returns: the localized metadata for the given name.
 */
static VALUE
rg_get_metadata(VALUE self, VALUE name)
{
    return
        CSTR2RVAL (gtk_source_language_get_metadata
            (_SELF (self), RVAL2CSTR(name)));
}

/* Method: mime_types
 * Returns: a list of mime types for the given language, as an array of strings.
 */
static VALUE
rg_mime_types(VALUE self)
{
    VALUE ary;
    char **types = gtk_source_language_get_mime_types (_SELF (self));
    if (!types)
        return Qnil;

    ary = rb_ary_new();
    while (*types){
        rb_ary_push(ary, CSTR2RVAL(*types));
        types++;
    }
    return ary;
}

/* Method: globs
 * Returns: a list of globs for the given language, as an array of strings.
 */
static VALUE
rg_globs(VALUE self)
{
    VALUE ary;
    char **globs = gtk_source_language_get_globs (_SELF (self));
    if (!globs)
        return Qnil;

    ary = rb_ary_new();
    while (*globs){
        rb_ary_push(ary, CSTR2RVAL(*globs));
        globs++;
    }
    return ary;
}

/* Method: get_style_name(style_id)
 * style_id: the style id (string)
 * Returns: the localized style name of the given id.
 */
static VALUE
rg_get_style_name(VALUE self, VALUE style_id)
{
    return
        CSTR2RVAL (gtk_source_language_get_style_name
            (_SELF (self), RVAL2CSTR(style_id)));
}

/* Method: style_id
 * Returns: the styles defined by the language.
 */
static VALUE
rg_style_ids(VALUE self)
{
    VALUE ary;
    gchar **ids = gtk_source_language_get_style_ids (_SELF (self));
    if (!ids)
        return Qnil;

    ary = rb_ary_new();
    while (*ids){
        rb_ary_push(ary, CSTR2RVAL(*ids));
        ids++;
    }
    return ary;
}

void
Init_gtksource_language (VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS (GTK_SOURCE_TYPE_LANGUAGE, "Language", mGtkSource);

    RG_DEF_METHOD(get_metadata, 1);
    RG_DEF_METHOD(mime_types, 0);
    RG_DEF_METHOD(globs, 0);
    RG_DEF_METHOD(get_style_name, 1);
    RG_DEF_METHOD(style_ids, 0);
}
