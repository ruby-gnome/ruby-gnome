/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcelanguagesmanager.c -

  $Author $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceLanguagesManager
 * A class to manage source languages.
 */

#define _SELF(self) (GTK_SOURCE_LANGUAGES_MANAGER(RVAL2GOBJ(self)))

/* Class method: new 
 * Returns: a newly created Gtk::SourceLanguagesManager object.
 */
static VALUE
slm_new (self)
	VALUE self;
{
	G_INITIALIZE (self, gtk_source_languages_manager_new ());
	return Qnil;
}

/*
 * Method: get_language(mime_type)
 * mime_type: a mime type (as a string).
 *
 * Gets the Gtk::SourceLanguage which is associated with the given mime_type
 * in the language manager.
 *
 * Returns: a Gtk::SourceLanguage, or nil if there is no language associated
 * with the given mime_type.
 */
static VALUE
slm_get_language_from_mime_type (self, mime_type)
	VALUE self, mime_type;
{
	return
	    GOBJ2RVAL (gtk_source_languages_manager_get_language_from_mime_type
		       (_SELF (self), RVAL2CSTR (mime_type)));
}

/* Method: lang_files_dirs
 * Returns: a list of language files directories (strings) for the given
 * language manager.
 */
static VALUE
slm_get_lang_files_dirs (self)
	VALUE self;
{
	/* FIXME: This is a property. But you can't default value because it uses gpointer 
        	  You may need also implement this setter method.
	*/
	VALUE ary = rb_ary_new ();
	const GSList *dirs =
	    gtk_source_languages_manager_get_lang_files_dirs (_SELF (self));

	while (dirs) {
		rb_ary_push (ary, CSTR2RVAL (dirs->data));
		dirs = g_slist_next (dirs);
	}

	return ary;
}

/* Method: available_languages 
 * Returns: a list of available languages for the given language manager,
 * as an array of Gtk::SourceLanguage objects.
 */
static VALUE
slm_get_available_languages (self)
	VALUE self;
{
	VALUE ary = rb_ary_new ();
	const GSList *langs =
	    gtk_source_languages_manager_get_available_languages (_SELF
								  (self));

	while (langs) {
		rb_ary_push (ary, GOBJ2RVAL (langs->data));
		langs = g_slist_next (langs);
	}
	return ary;
}

void
Init_gtk_sourcelanguagesmanager ()
{
	VALUE cslm =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_LANGUAGES_MANAGER,
			 "SourceLanguagesManager", mGtk);

	rb_define_method (cslm, "initialize", slm_new, 0);
	rb_define_method (cslm, "available_languages",
			  slm_get_available_languages, 0);
	rb_define_method (cslm, "get_language",
			  slm_get_language_from_mime_type, 1);
	rb_define_method (cslm, "lang_files_dirs", slm_get_lang_files_dirs, 0);
	G_DEF_SETTERS (cslm);
}
