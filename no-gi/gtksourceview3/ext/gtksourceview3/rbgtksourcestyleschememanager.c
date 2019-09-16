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

/* Class: Gtk::SourceStyleSchemeManager
 * A class to manage source style scheme.
 */

#define RG_TARGET_NAMESPACE cStyleSchemeManager
#define _SELF(self) (RVAL2GTKSOURCESTYLESCHEMEMANAGER(self))

static VALUE rb_mGtkSource;

/* Class method: new
 * Returns: a newly created Gtk::SourceStyleSchemeManager object.
 */
static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE (self, gtk_source_style_scheme_manager_new ());
    return Qnil;
}

/* Class method: default
 *
 * Gets the default style scheme manager.
 *
 * Returns: a Gtk::SourceStyleSchemeManager
 */
static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    GtkSourceStyleSchemeManager* sssm = gtk_source_style_scheme_manager_get_default();
    GType gtype = G_TYPE_FROM_INSTANCE(sssm);

    gchar *gtypename = (gchar *) g_type_name (gtype);
    if (strncmp (gtypename, "GtkSource", 9) == 0)
        gtypename += 9;
    if (!rb_const_defined_at (rb_mGtkSource, rb_intern (gtypename)))
        G_DEF_CLASS (gtype, gtypename, rb_mGtkSource);

    return GOBJ2RVAL(sssm);
}

/* Method: append_search_path(path)
 * path: additional style scheme file directory path (string)
 *
 * Appends the style scheme files directory for the given style scheme manager.
 *
 * Returns: self.
 */
static VALUE
rg_append_search_path(VALUE self, VALUE path)
{
    gtk_source_style_scheme_manager_append_search_path (_SELF (self), RVAL2CSTR(path));
    return self;
}

/* Method: prepend_search_path(path)
 * path: additional style scheme file directory path (string)
 *
 * Prepend the style scheme files directory for the given style scheme manager.
 *
 * Returns: self.
 */
static VALUE
rg_prepend_search_path(VALUE self, VALUE path)
{
    gtk_source_style_scheme_manager_prepend_search_path (_SELF (self), RVAL2CSTR(path));
    return self;
}

/*
 * Method: scheme(scheme_id)
 * scheme_id: a style scheme id (as a string).
 *
 * Gets the Gtk::SourceStyleScheme which is associated with the given id
 * in the style scheme manager.
 *
 * Returns: a Gtk::SourceStyleScheme, or nil if there is no style scheme
 * associated with the given id.
 */
static VALUE
rg_get_scheme(VALUE self, VALUE scheme_id)
{
    return
        GOBJ2RVAL (gtk_source_style_scheme_manager_get_scheme
               (_SELF (self), RVAL2CSTR (scheme_id)));
}

/*
 * Method: force_rescan
 *
 * Forces all style schemes to be reloaded the next time the
 * Gtk::SourceStyleSchemeManager is accessed.
 *
 * Returns: self.
 */
static VALUE
rg_force_rescan(VALUE self)
{
    gtk_source_style_scheme_manager_force_rescan(_SELF (self));
    return self;
}

void
Init_gtksource_styleschememanager (VALUE mGtkSource)
{
    rb_mGtkSource = mGtkSource;
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS (GTK_SOURCE_TYPE_STYLE_SCHEME_MANAGER,
             "StyleSchemeManager", mGtkSource);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(append_search_path, 1);
    RG_DEF_METHOD(prepend_search_path, 1);
    RG_DEF_METHOD(get_scheme, 1);
    RG_DEF_METHOD(force_rescan, 0);
    RG_DEF_SMETHOD(default, 0);
}
