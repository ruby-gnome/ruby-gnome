/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
/* Class: Gtk::SourceMark
 * A source mark.
 */

#define _SELF(self) (GTK_SOURCE_MARK(RVAL2GOBJ(self)))

/* Class method: new(name, category)
 * name: mark name (string)
 * category: marker category (string)
 *
 * Returns: a newly created Gtk::SourceMark object.
 */
static VALUE
sourcemark_new(VALUE self, VALUE name, VALUE category)
{
	G_INITIALIZE (self,
		      gtk_source_mark_new (RVAL2CSTR(name), RVAL2CSTR(category)));
	return Qnil;
}

/* Defined as properties.
const gchar*        gtk_source_mark_get_category        (GtkSourceMark *mark);
*/

/* Method: next(category=nil)
 * category: the category (string), or nil.
 *
 * Returns: the next Gtk::SourceMark after the mark.
 */
static VALUE
sourcemark_next(int argc, VALUE *argv, VALUE self)
{
    VALUE category;

    rb_scan_args (argc, argv, "01", &category);

    return GOBJ2RVAL (gtk_source_mark_next (_SELF (self),
                                NIL_P (category) ? NULL : RVAL2CSTR(category)));
}

/* Method: prev(category=nil)
 * category: the category (string), or nil.
 *
 * Returns: the previous Gtk::SourceMark before the mark.
 */
static VALUE
sourcemark_prev(int argc, VALUE *argv, VALUE self)
{
    VALUE category;

    rb_scan_args (argc, argv, "01", &category);

    return GOBJ2RVAL (gtk_source_mark_prev (_SELF (self),
                                NIL_P (category) ? NULL : RVAL2CSTR(category)));
}
#endif /* HAVE_GTK_SOURCE_MARK_GET_TYPE */

void
Init_gtk_sourcemark ()
{
#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
	VALUE csm = G_DEF_CLASS (GTK_TYPE_SOURCE_MARK, "SourceMark", mGtk);

	rb_define_method (csm, "initialize", sourcemark_new, 2);
	rb_define_method (csm, "next", sourcemark_next, -1);
	rb_define_method (csm, "prev", sourcemark_prev, -1);

	G_DEF_SETTERS (csm);
#endif /* HAVE_GTK_SOURCE_MARK_GET_TYPE */
}
