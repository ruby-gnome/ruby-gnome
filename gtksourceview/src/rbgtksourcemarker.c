/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcemarker.c -

  $Author $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceMarker
 * A source marker.
 */

#define _SELF(self) (GTK_SOURCE_MARKER(RVAL2GOBJ(self)))

/*
 * Method: set_marker_type(type)
 * type: the type of the marker (as a string).
 *
 * Sets the type of the marker.
 *
 * Returns: self.
 */
static VALUE
sourcemarker_set_marker_type (self, type)
	VALUE self, type;
{
	gtk_source_marker_set_marker_type (_SELF (self), RVAL2CSTR (type));
	return self;
}

/* Method: marker_type
 * Returns: the type of the marker.
 */
static VALUE
sourcemarker_get_marker_type (self)
	VALUE self;
{
	gchar *name = gtk_source_marker_get_marker_type (_SELF (self));
	VALUE ret = CSTR2RVAL (name);

	g_free (name);
	return ret;
}

/* Method: line
 * Returns: the line number of this marker.
 */
static VALUE
sourcemarker_get_line (self)
	VALUE self;
{
	return INT2NUM (gtk_source_marker_get_line (_SELF (self)));
}

/* Method: name
 * Returns: the name of this marker.
 */
static VALUE
sourcemarker_get_name (self)
	VALUE self;
{
	return CSTR2RVAL (gtk_source_marker_get_name (_SELF (self)));
}

/* Method: buffer
 * Returns: the buffer associated with this marker, as a Gtk::SourceBuffer
 * object.
 */
static VALUE
sourcemarker_get_buffer (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_marker_get_buffer (_SELF (self)));
}

/* Method: next
 * Returns: the next Gtk::SourceMarker after the marker.
 */
static VALUE
sourcemarker_next (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_marker_next (_SELF (self)));
}

/* Method: prev 
 * Returns: the previous Gtk::SourceMarker before the marker.
 */
static VALUE
sourcemarker_prev (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_marker_prev (_SELF (self)));
}

void
Init_gtk_sourcemarker ()
{
	VALUE csm = G_DEF_CLASS (GTK_TYPE_SOURCE_MARKER, "SourceMarker", mGtk);

	rb_define_method (csm, "next", sourcemarker_next, 0);
	rb_define_method (csm, "name", sourcemarker_get_name, 0);
	rb_define_method (csm, "set_marker_type", sourcemarker_set_marker_type,
			  1);
	rb_define_method (csm, "marker_type", sourcemarker_get_marker_type, 0);
	rb_define_method (csm, "prev", sourcemarker_prev, 0);
	rb_define_method (csm, "line", sourcemarker_get_line, 0);
	rb_define_method (csm, "buffer", sourcemarker_get_buffer, 0);

	G_DEF_SETTERS (csm);
}
