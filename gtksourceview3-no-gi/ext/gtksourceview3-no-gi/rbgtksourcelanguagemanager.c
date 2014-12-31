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

/* Class: Gtk::SourceLanguageManager
 * A class to manage source language.
 */

#define RG_TARGET_NAMESPACE cLanguageManager
#define _SELF(self) (RVAL2GTKSOURCELANGUAGEMANAGER(self))

static VALUE rb_mGtkSource;

/* Class method: new
 * Returns: a newly created Gtk::SourceLanguageManager object.
 */
static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE (self, gtk_source_language_manager_new ());
    return Qnil;
}

/* Class method: default
 *
 * Gets the default language manager.
 *
 * Returns: a Gtk::SourceLanguageManager
 */
static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    GtkSourceLanguageManager* slm = gtk_source_language_manager_get_default();
    GType gtype = G_TYPE_FROM_INSTANCE(slm);

    gchar *gtypename = (gchar *) g_type_name (gtype);
    if (strncmp (gtypename, "GtkSource", 9) == 0)
        gtypename += 9;
    if (!rb_const_defined_at (rb_mGtkSource, rb_intern (gtypename)))
        G_DEF_CLASS (gtype, gtypename, rb_mGtkSource);

    return GOBJ2RVAL(slm);
}

/*
 * Method: get_language(id)
 * id: a language id (as a string).
 *
 * Gets the Gtk::SourceLanguage which is associated with the given id
 * in the language manager.
 *
 * Returns: a Gtk::SourceLanguage, or nil if there is no language associated
 * with the given id.
 */
static VALUE
rg_get_language(VALUE self, VALUE id)
{
    return
        GOBJ2RVAL (gtk_source_language_manager_get_language
               (_SELF (self), RVAL2CSTR (id)));
}

/*
 * Method: guess_language(filename, content_type)
 * filename: a file name (as a string), or nil.
 * content_type: content type (as a string), or nil.
 *
 * Guesses the Gtk::SourceLanguage for the given file name and content type.
 *
 * Returns: a Gtk::SourceLanguage, or nil if there is no language associated
 * with the given filename or content_type.
 */
static VALUE
rg_guess_language(VALUE self, VALUE filename, VALUE content_type)
{
    return GOBJ2RVAL (gtk_source_language_manager_guess_language
                         (_SELF (self),
                          RVAL2CSTR_ACCEPT_NIL (filename),
                          RVAL2CSTR_ACCEPT_NIL (content_type)));
}

void
Init_gtksource_languagemanager (VALUE mGtkSource)
{
    rb_mGtkSource = mGtkSource;
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS (GTK_SOURCE_TYPE_LANGUAGE_MANAGER,
             "LanguageManager", mGtkSource);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(get_language, 1);
    RG_DEF_METHOD(guess_language, 2);
    RG_DEF_SMETHOD(default, 0);
}
