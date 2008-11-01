/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcelanguage.c -

  $Author: mutoh $
  $Date: 2005/10/02 18:40:34 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team  
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceLanguage
 * Source language.
 */

#define _SELF(self) (GTK_SOURCE_LANGUAGE(RVAL2GOBJ(self)))

/* Method: name
 * Returns: the localized name of the language.
 */
static VALUE
sourcelanguage_get_name (self)
	VALUE self;
{
	gchar *name = gtk_source_language_get_name (_SELF (self));
	VALUE ret = CSTR2RVAL (name);

	g_free (name);
	return ret;
}

/* Method: section
 * Returns: the localized section of the language, each language belong to a
 * section (ex. HTML belogs to the Markup section).
 */
static VALUE
sourcelanguage_get_section (self)
	VALUE self;
{
	gchar *section = gtk_source_language_get_section (_SELF (self));
	VALUE ret = CSTR2RVAL (section);

	g_free (section);
	return ret;
}

/* Method: escape_char
 * Returns: the value of the ESC character in the given language.
 */
static VALUE
sourcelanguage_get_escape_char (self)
	VALUE self;
{
	gchar buf[10];
	gint len =
	    g_unichar_to_utf8 (gtk_source_language_get_escape_char
			       (_SELF (self)), buf);
	buf[len] = '\0';
	return rb_str_new2 (buf);
}

/* Method: style_scheme
 * Returns: the style scheme associated with the given language, as a
 * reference to a Gtk::SourceStyleScheme object.
 */
static VALUE
sourcelanguage_get_style_scheme (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_language_get_style_scheme (_SELF (self)));
}

/*
 * Method: set_style_scheme(scheme)
 * scheme: a Gtk::SourceStyleScheme object.
 *
 * Sets the style scheme of the given language.
 *
 * Returns: self.
 */
static VALUE
sourcelanguage_set_style_scheme (self, scheme)
	VALUE self, scheme;
{
	gtk_source_language_set_style_scheme (_SELF (self),
					      GTK_SOURCE_STYLE_SCHEME
					      (RVAL2GOBJ (scheme)));
	return self;
}

/*
 * Method: get_tag_style(tag_id)
 * tag_id: the ID of a Gtk::SourceTag, as a string. 
 *
 * Gets the style of the tag whose ID is tag_id. If the style is not defined
 * then returns the default style.
 * 
 * Returns: a Gtk::SourceTagStyle object.
 */
static VALUE
sourcelanguage_get_tag_style (self, tag_name)
	VALUE self, tag_name;
{
	return
	    GOBJ2RVAL (gtk_source_language_get_tag_style
		       (_SELF (self), RVAL2CSTR (tag_name)));
}

/*
 * Method: set_tag_style(tag_id, style=nil)
 * tag_id: the ID of a Gtk::SourceTag, as a string.
 * style: a Gtk::SourceTagStyle.
 *
 * Sets the style of the tag whose ID is tag_id. If style is nil restore
 * the default style.
 *
 * Returns: self.
 */
static VALUE
sourcelanguage_set_tag_style (argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE name, style;
	GtkSourceTagStyle *gstyle;

	rb_scan_args (argc, argv, "11", &name, &style);

	gstyle = NIL_P (style) ? (GtkSourceTagStyle *) NULL :
	    (GtkSourceTagStyle *) RVAL2BOXED (style,
					      GTK_TYPE_SOURCE_TAG_STYLE);

	gtk_source_language_set_tag_style (_SELF (self), RVAL2CSTR (name),
					   gstyle);
	return self;
}

/*
 * Method: get_tag_default_style(tag_id)
 * tag_id: the ID of a Gtk::SourceTag, as a string.
 *
 * Gets the default style of the tag whose ID is tag_id.
 *
 * Returns: a Gtk::SourceTagStyle object.
 */
static VALUE
sourcelanguage_get_tag_default_style (self, tag_id)
	VALUE self, tag_id;
{
	return
	    GOBJ2RVAL (gtk_source_language_get_tag_default_style
		       (_SELF (self), RVAL2CSTR (tag_id)));
}

/*
 * Method: set_mime_types(arr)
 * arr: an array of mime types (strings).
 *
 * Sets a list of mime types for the given language. If mime types is nil this
 * method will use the default mime types from the language file.
 *
 * Returns: self.
 */
static VALUE
sourcelanguage_set_mime_types (self, list)
	VALUE self, list;
{
	GSList *types = NULL, *p;
	int i;

	if (list != Qnil) {
		Check_Type (list, T_ARRAY);

		for (i = 0; i < RARRAY_LEN(list); i++)
			types = g_slist_append (types,
						RVAL2CSTR(RARRAY_PTR(list)[i]));
	}
	
	gtk_source_language_set_mime_types (_SELF (self), types);

	if (types != NULL) {
		p = types;
		while (p != NULL) {
			g_free (p->data);
			p = g_slist_next (p);
		}
		g_slist_free (types);
	}

	return self;
}

/* Method: mime_types
 * Returns: a list of mime types for the given language, as an array of strings.
 */
static VALUE
sourcelanguage_get_mime_types (self)
	VALUE self;
{
	GSList *p;
	GSList *types = gtk_source_language_get_mime_types (_SELF (self));
	VALUE ary = rb_ary_new ();

	p = types;
	while (p) {
		rb_ary_push (ary, CSTR2RVAL (p->data));
		g_free (p->data);
		p = g_slist_next (p);
	}
	g_slist_free (types);
	return ary;
}

/* Method: tags
 * Returns: a list of tags for the given language, as an array of
 * Gtk::SourceTag objects.
 */
static VALUE
sourcelanguage_get_tags (self)
	VALUE self;
{
	GSList *p;
	VALUE ary = rb_ary_new ();
	GSList *types = gtk_source_language_get_tags (_SELF (self));

	p = types;
	while (p) {
		rb_ary_push (ary, GOBJ2RVAL (GTK_SOURCE_TAG (p->data)));
		g_object_unref (p->data);
		p = g_slist_next (p);
	}
	g_slist_free (types);
	return ary;
}

/*
 * Method: each_tag { |tag| ... }
 *
 * Calls the block for each tag for the given language, passing a 
 * reference to a Gtk::SourceTag object as parameter.
 *
 * Returns: self.
 */
static VALUE
sourcelanguage_each_tag (self)
	VALUE self;
{
	rb_ary_each (sourcelanguage_get_tags (self));
	return self;
}

/*
 * Method: each_mime_type { |mime_type| ... }
 *
 * Calls the block for each mime type for the given language, passing a 
 * string as parameter.
 *
 * Returns: self.
 */
static VALUE
sourcelanguage_each_mime_type (self)
	VALUE self;
{
	rb_ary_each (sourcelanguage_get_mime_types (self));
	return self;
}

void
Init_gtk_sourcelanguage ()
{
	VALUE clang =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_LANGUAGE, "SourceLanguage", mGtk);

	rb_define_method (clang, "name", sourcelanguage_get_name, 0);
	rb_define_method (clang, "section", sourcelanguage_get_section, 0);
	rb_define_method (clang, "get_tag_style", sourcelanguage_get_tag_style,
			  1);
	rb_define_method (clang, "style_scheme",
			  sourcelanguage_get_style_scheme, 0);
	rb_define_method (clang, "set_mime_types",
			  sourcelanguage_set_mime_types, 1);
	rb_define_method (clang, "escape_char", sourcelanguage_get_escape_char,
			  0);
	rb_define_method (clang, "get_tag_default_style",
			  sourcelanguage_get_tag_default_style, 1);
	rb_define_method (clang, "tags", sourcelanguage_get_tags, 0);
	rb_define_method (clang, "each_tag", sourcelanguage_each_tag, 0);
	rb_define_method (clang, "set_style_scheme",
			  sourcelanguage_set_style_scheme, 1);
	rb_define_method (clang, "set_tag_style", sourcelanguage_set_tag_style,
			  -1);
	rb_define_method (clang, "mime_types", sourcelanguage_get_mime_types,
			  0);
	rb_define_method (clang, "each_mime_type",
			  sourcelanguage_each_mime_type, 0);

	G_DEF_SETTERS (clang);
}
