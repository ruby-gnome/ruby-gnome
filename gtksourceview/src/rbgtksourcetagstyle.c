/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcetagstyle.c -

  $Author: lrz $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceTagStyle
 * The style of a source tag.
 */
#define _SELF(self) (GTK_SOURCE_TAG_STYLE(RVAL2GOBJ(self)))

/* Class method: new
 * Returns: a newly created Gtk::SourceTagStyle object.
 */
static VALUE
sourcetagstyle_new (self)
	VALUE self;
{
	G_INITIALIZE (self, gtk_source_tag_style_new ());
	return Qnil;
}

void
Init_gtk_sourcetagstyle ()
{
	VALUE cSourceTagStyle =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_TAG_STYLE, "SourceTagStyle", mGtk);

	rb_define_method (cSourceTagStyle, "initialize", sourcetagstyle_new,
			  0);

	G_DEF_SETTERS (cSourceTagStyle);
}
