/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourceview.c -

  $Author: mutoh $
  $Date: 2006/12/17 16:15:28 $

  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
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

/* Defined as properties.
void                gtk_source_view_set_auto_indent     (GtkSourceView *view,
                                                         gboolean enable)
gboolean            gtk_source_view_get_auto_indent     (GtkSourceView *view)
void                gtk_source_view_set_indent_on_tab   (GtkSourceView *view,
                                                         gboolean enable)
gboolean            gtk_source_view_get_indent_on_tab   (GtkSourceView *view)
void                gtk_source_view_set_indent_width    (GtkSourceView *view,
                                                         gint width)
gint                gtk_source_view_get_indent_width    (GtkSourceView *view)
void                gtk_source_view_set_insert_spaces_instead_of_tabs
                                                        (GtkSourceView *view,
                                                         gboolean enable)
gboolean            gtk_source_view_get_insert_spaces_instead_of_tabs
                                                        (GtkSourceView *view)
void                gtk_source_view_set_smart_home_end  (GtkSourceView *view,
                                                         GtkSourceSmartHomeEndType smart_he)
GtkSourceSmartHomeEndType gtk_source_view_get_smart_home_end
                                                        (GtkSourceView *view)
*/

#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
/*
 * Method: set_mark_category_pixbuf(category, pixbuf)
 * category: a category (as a string).
 * pixbuf: a Gdk::Pixbuf object.
 *
 * Associates a given pixbuf with a given category.
 *
 * Returns: self.
 */
static VALUE
sourceview_set_mark_category_pixbuf (self, category, pixbuf)
	VALUE self, category, pixbuf;
{
	gtk_source_view_set_mark_category_pixbuf (_SELF (self),
					   RVAL2CSTR (category),
					   GDK_PIXBUF (RVAL2GOBJ (pixbuf)));
	return self;
}

/*
 * Method: get_mark_category_pixbuf(category)
 * category: a category (as a string).
 *
 * Gets the pixbuf which is associated with the given category.
 *
 * Returns: a Gdk::Pixbuf object if found, or nil if not found.
 */
static VALUE
sourceview_get_mark_category_pixbuf (self, category)
	VALUE self, category;
{
	return
	    GOBJ2RVAL (gtk_source_view_get_mark_category_pixbuf
		       (_SELF (self), RVAL2CSTR (category)));
}

/*
 * Method: set_mark_category_priority(category, priority)
 * category: a category (as a string).
 * priority: a priority (int).
 *
 * Associates a given priority with a given category.
 *
 * Returns: self.
 */
static VALUE
sourceview_set_mark_category_priority (self, category, priority)
	VALUE self, category, priority;
{
	gtk_source_view_set_mark_category_priority (_SELF (self),
					   RVAL2CSTR (category),
					   NUM2INT (priority));
	return self;
}

/*
 * Method: get_mark_category_priority(category)
 * category: a category (as a string).
 *
 * Gets the priority which is associated with the given category.
 *
 * Returns: the priority if found, or 0 if not found.
 */
static VALUE
sourceview_get_mark_category_priority (self, category)
	VALUE self, category;
{
	return
	    INT2NUM (gtk_source_view_get_mark_category_priority
		       (_SELF (self), RVAL2CSTR (category)));
}
#endif /* HAVE_GTK_SOURCE_MARK_GET_TYPE */

/* Defined as properties.
void                gtk_source_view_set_highlight_current_line
                                                        (GtkSourceView *view,
                                                         gboolean show)
gboolean            gtk_source_view_get_highlight_current_line
                                                        (GtkSourceView *view)
void                gtk_source_view_set_show_line_marks (GtkSourceView *view,
                                                         gboolean show);
gboolean            gtk_source_view_get_show_line_marks (GtkSourceView *view)
void                gtk_source_view_set_show_line_numbers
                                                        (GtkSourceView *view,
                                                         gboolean show)
gboolean            gtk_source_view_get_show_line_numbers
                                                        (GtkSourceView *view)
void                gtk_source_view_set_show_right_margin
                                                        (GtkSourceView *view,
                                                         gboolean show)
gboolean            gtk_source_view_get_show_right_margin
                                                        (GtkSourceView *view)
void                gtk_source_view_set_right_margin_position
                                                        (GtkSourceView *view,
                                                         guint pos)
guint               gtk_source_view_get_right_margin_position
                                                        (GtkSourceView *view)
void                gtk_source_view_set_tab_width       (GtkSourceView *view,
                                                         guint width)
guint               gtk_source_view_get_tab_width       (GtkSourceView *view)
*/

void
Init_gtk_sourceview ()
{
    VALUE cSourceView = G_DEF_CLASS (GTK_TYPE_SOURCE_VIEW, "SourceView", mGtk);

    rb_define_const(cSourceView, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GTKSOURCEVIEW2_MAJOR_VERSION),
                                INT2FIX(GTKSOURCEVIEW2_MINOR_VERSION),
                                INT2FIX(GTKSOURCEVIEW2_MICRO_VERSION)));

    rb_define_method(cSourceView, "initialize", sourceview_initialize, -1);
#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
    rb_define_method(cSourceView, "get_mark_category_pixbuf", sourceview_get_mark_category_pixbuf, 1);
    rb_define_method(cSourceView, "set_mark_category_pixbuf", sourceview_set_mark_category_pixbuf, 2);
    rb_define_method(cSourceView, "get_mark_category_priority", sourceview_get_mark_category_priority, 1);
    rb_define_method(cSourceView, "set_mark_category_priority", sourceview_set_mark_category_priority, 2);
#endif /* HAVE_GTK_SOURCE_MARK_GET_TYPE */

    G_DEF_SETTERS (cSourceView);

    G_DEF_CLASS(GTK_TYPE_SOURCE_SMART_HOME_END_TYPE, "SmartHomeEndType", cSourceView);
    G_DEF_CONSTANTS(cSourceView, GTK_TYPE_SOURCE_SMART_HOME_END_TYPE, "GTK_SOURCE_");
}
