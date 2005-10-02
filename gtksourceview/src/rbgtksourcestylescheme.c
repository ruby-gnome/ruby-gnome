/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcestylescheme.c -

  $Author: mutoh $
  $Date: 2005/10/02 18:40:34 $

  Copyright (C) 2005  Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Module: Gtk::SourceStyleScheme
 */

#define _SELF(self) (GTK_SOURCE_STYLE_SCHEME(RVAL2GOBJ(self)))

/* Method: get_tag_style(style_name)
 * style_name: the name of a style.
 *
 * Gets the tag associated with the given style_name in the style scheme.
 *
 * Returns: Gtk::SourceTagStyle
 */
static VALUE
scheme_get_tag_style(self, style_name)
    VALUE self, style_name;
{
    return GOBJ2RVAL(gtk_source_style_scheme_get_tag_style(_SELF(self), 
                                                           (const gchar*)RVAL2CSTR(style_name)));
}

/* Method: name
 *
 * Gets the name of the given style scheme.
 *
 * Returns: the name of the sytle scheme.
 */
static VALUE
scheme_get_name(self)
    VALUE self;
{
    return CSTR2RVAL((gchar*)gtk_source_style_scheme_get_name(_SELF(self)));
}

/* Class method: default
 *
 * Gets the default style scheme.
 *
 * Returns: a Gtk::SourceStyleScheme
 */
static VALUE
scheme_s_get_default(self)
    VALUE self;
{
    GtkSourceStyleScheme* scheme = gtk_source_style_scheme_get_default();
    GType gtype = G_TYPE_FROM_INSTANCE(scheme);

    gchar *gtypename = (gchar *) g_type_name (gtype);
    if (strncmp (gtypename, "Gtk", 3) == 0)
        gtypename += 3;
    if (!rb_const_defined_at (mGtk, rb_intern (gtypename)))
        G_DEF_CLASS (gtype, gtypename, mGtk);

    return GOBJ2RVAL(scheme);
}

void
Init_gtk_sourcestylescheme ()
{
    VALUE scheme = G_DEF_CLASS (GTK_TYPE_SOURCE_STYLE_SCHEME, "SourceStyleScheme", mGtk);
    
    rb_define_method(scheme, "get_tag_style", scheme_get_tag_style, 1);
    rb_define_method(scheme, "name", scheme_get_name, 0);
    rb_define_singleton_method(scheme, "default", scheme_s_get_default, 0);
}
