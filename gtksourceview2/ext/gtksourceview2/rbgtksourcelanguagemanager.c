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

#include "rbgtksourcemain.h"

/* Class: Gtk::SourceLanguageManager
 * A class to manage source language.
 */

#define RG_TARGET_NAMESPACE cSourceLanguageManager
#define _SELF(self) (GTK_SOURCE_LANGUAGE_MANAGER(RVAL2GOBJ(self)))

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
rg_s_default(VALUE self)
{
    GtkSourceLanguageManager* slm = gtk_source_language_manager_get_default();
    GType gtype = G_TYPE_FROM_INSTANCE(slm);

    gchar *gtypename = (gchar *) g_type_name (gtype);
    if (strncmp (gtypename, "Gtk", 3) == 0)
        gtypename += 3;
    if (!rb_const_defined_at (mGtk, rb_intern (gtypename)))
        G_DEF_CLASS (gtype, gtypename, mGtk);

    return GOBJ2RVAL(slm);
}

/* Method: set_search_path(dirs)
 * dirs: language file directory path
 *
 * Sets the language files directories for the given language manager.
 *
 * Returns: self.
 */
static VALUE
rg_set_search_path(VALUE self, VALUE dirs)
{
    gchar** gdirs = (gchar**)NULL;
    gint i;

    if (! NIL_P(dirs)){
        Check_Type(dirs, T_ARRAY);
        i = RARRAY_LEN(dirs);
        gdirs = ALLOCA_N(gchar*, i + 1);
        for (i = 0; i < i; i++) {
            if (TYPE(RARRAY_PTR(dirs)[i]) == T_STRING) {
                gdirs[i] = RVAL2CSTR(RARRAY_PTR(dirs)[i]);
            }
            else {
                gdirs[i] = "";
            }
        }
        gdirs[i] = (gchar*)NULL;
    }

    gtk_source_language_manager_set_search_path (_SELF (self), gdirs);

    return self;
}

/* Method: search_path
 * Returns: a list of language files directories (strings) for the given
 * language manager.
 */
static VALUE
rg_search_path(VALUE self)
{
    VALUE ary;
    const gchar * const * dirs =
            gtk_source_language_manager_get_search_path (_SELF (self));
    if (!dirs)
        return Qnil;

    ary = rb_ary_new();
    while (*dirs){
        rb_ary_push(ary, CSTR2RVAL(*dirs));
        dirs++;
    }
    return ary;
}

/* Method: language_ids
 * Returns: a list of languages ids for the given language manager
 */
static VALUE
rg_language_ids(VALUE self)
{
    VALUE ary;
    const gchar * const * ids =
            gtk_source_language_manager_get_language_ids (_SELF (self));
    if (!ids)
        return Qnil;

    ary = rb_ary_new();
    while (*ids){
        rb_ary_push(ary, CSTR2RVAL(*ids));
        ids++;
    }
    return ary;
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

#ifdef HAVE_GTK_SOURCE_LANGUAGE_MANAGER_GUESS_LANGUAGE
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
                          NIL_P(filename) ? NULL : RVAL2CSTR (filename),
                          NIL_P(content_type) ? NULL : RVAL2CSTR (content_type)));
}
#endif /* HAVE_GTK_SOURCE_LANGUAGE_MANAGER_GUESS_LANGUAGE */

void
Init_gtk_sourcelanguagemanager ()
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS (GTK_TYPE_SOURCE_LANGUAGE_MANAGER,
             "SourceLanguageManager", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_search_path, 1);
    RG_DEF_METHOD(search_path, 0);
    RG_DEF_METHOD(language_ids, 0);
    RG_DEF_METHOD(get_language, 1);
#ifdef HAVE_GTK_SOURCE_LANGUAGE_MANAGER_GUESS_LANGUAGE
    RG_DEF_METHOD(guess_language, 2);
#endif
    RG_DEF_SMETHOD(default, 0);
    G_DEF_SETTERS (RG_TARGET_NAMESPACE);
}