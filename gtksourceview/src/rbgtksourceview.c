/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourceview.c -

  $Author: lrz $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceView
 * A view on a source.
 */
#define _SELF(self) (GTK_SOURCE_VIEW(RVAL2GOBJ(self)))

/*
 * Class method: new(buffer=nil)
 * buffer: a Gtk::SourceBuffer object.
 *
 * Creates a new Gtk::SourceView.  If buffer is not provided or nil, an empty
 * buffer will be created for you.  Note that one buffer can be shared among
 * many widgets.
 * 
 * Returns: a newly created Gtk::SourceView object.
 */
static VALUE
sourceview_initialize (argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE buffer;
	GtkWidget *widget;

	rb_scan_args (argc, argv, "01", &buffer);

	if (NIL_P (buffer))
		widget = gtk_source_view_new ();
	else
		widget = gtk_source_view_new_with_buffer (RVAL2GOBJ (buffer));

	RBGTK_INITIALIZE (self, widget);
	return self;
}

/*
 * Method: set_marker_pixbuf(marker_type, pixbuf)
 * marker_type: a marker type (as a string).
 * pixbuf: a Gdk::Pixbuf object.
 *
 * Associates a given pixbuf with a given marker_type.
 *
 * Returns: self.
 */
static VALUE
sourceview_set_marker_pixbuf (self, marker_type, pixbuf)
	VALUE self, marker_type, pixbuf;
{
	gtk_source_view_set_marker_pixbuf (_SELF (self),
					   RVAL2CSTR (marker_type),
					   GDK_PIXBUF (RVAL2GOBJ (pixbuf)));
	return self;
}

/*
 * Method: get_marker_type(marker_type)
 * marker_type: a marker type (as a string).
 *
 * Gets the pixbuf which is associated with the given marker_type. 
 *
 * Returns: a Gdk::Pixbuf object if found, or nil if not found.
 */
static VALUE
sourceview_get_marker_pixbuf (self, marker_type)
	VALUE self, marker_type;
{
	return
	    GOBJ2RVAL (gtk_source_view_get_marker_pixbuf
		       (_SELF (self), RVAL2CSTR (marker_type)));
}

void
Init_gtk_sourceview ()
{
	VALUE cSourceView =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_VIEW, "SourceView", mGtk);

	rb_define_method (cSourceView, "get_marker_pixbuf",
			  sourceview_get_marker_pixbuf, 1);
	rb_define_method (cSourceView, "set_marker_pixbuf",
			  sourceview_set_marker_pixbuf, 2);
	rb_define_method (cSourceView, "initialize", sourceview_initialize,
			  -1);

	G_DEF_SETTERS (cSourceView);
}
