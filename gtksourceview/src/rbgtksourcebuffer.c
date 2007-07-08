/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcebuffer.c -

  $Author: sakai $
  $Date: 2007/07/08 03:02:28 $

  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceBuffer
 * Text buffer object for Gtk::SourceView.
 */

#define _SELF(self) (GTK_SOURCE_BUFFER(RVAL2GOBJ(self)))
#define RVAL2ITR(i) ((GtkTextIter*)RVAL2BOXED(i, GTK_TYPE_TEXT_ITER))
#define RVAL2MARKER(m) (GTK_SOURCE_MARKER(RVAL2GOBJ(marker)))

/*
 * Class method: new(obj=nil)
 * obj: either a Gtk::SourceTagTable, a Gtk::SourceLanguage, or nil. 
 *
 * Creates a new source buffer.  If a Gtk::SourceTagTable is provided, the
 * buffer will use it, otherwise it will create a new one.
 *
 * If a Gtk::SourceLanguage object is given, the buffer will be created
 * using highlightings patterns in this language. This is equivalent to
 * creating a new source buffer with the default tag table and then setting
 * the 'language' property.
 * 
 * Returns: a newly created Gtk::SourceBuffer object.
 */
static VALUE
sourcebuffer_new (argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE val;

	rb_scan_args (argc, argv, "01", &val);
	if (NIL_P (val)) {
		G_INITIALIZE (self, gtk_source_buffer_new (NULL));
	} else
	    if (rb_obj_is_kind_of
		(val, GTYPE2CLASS (GTK_TYPE_SOURCE_TAG_TABLE))) {
		G_INITIALIZE (self,
			      gtk_source_buffer_new (GTK_SOURCE_TAG_TABLE
						     (RVAL2GOBJ (val))));
	} else
	    if (rb_obj_is_kind_of
		(val, GTYPE2CLASS (GTK_TYPE_SOURCE_LANGUAGE))) {
		G_INITIALIZE (self,
			      gtk_source_buffer_new_with_language
			      (GTK_SOURCE_LANGUAGE (RVAL2GOBJ (val))));
	} else {
		rb_raise (rb_eArgError,
			  "invalid argument %s (expect nil, Gtk::SourceTagTable or Gtk::SourceLanguage)",
			  rb_class2name (CLASS_OF (val)));
	}
	return Qnil;
}

/* Defined as a property.
gboolean    gtk_source_buffer_get_check_brackets
                                            (GtkSourceBuffer *buffer);
void        gtk_source_buffer_set_check_brackets
                                            (GtkSourceBuffer *buffer,
                                             gboolean check_brackets);
*/


/*
 * Method: set_bracket_match_style(style)
 * style: the Gtk::SourceTagStyle specifying colors and text attributes.
 *
 * Sets the style used for highlighting matching brackets.
 *
 * Returns: self.
 */
static VALUE
sourcebuffer_set_bracket_match_style (self, style)
	VALUE self, style;
{
	gtk_source_buffer_set_bracket_match_style
	    (_SELF (self),
	     (GtkSourceTagStyle *) RVAL2BOXED (style,
					       GTK_TYPE_SOURCE_TAG_STYLE));

	return self;
}

/* Defined as properties.
gboolean    gtk_source_buffer_get_highlight (GtkSourceBuffer *buffer);
void        gtk_source_buffer_set_highlight (GtkSourceBuffer *buffer,
                                             gboolean highlight);
gint        gtk_source_buffer_get_max_undo_levels
                                            (GtkSourceBuffer *buffer);
void        gtk_source_buffer_set_max_undo_levels
                                            (GtkSourceBuffer *buffer,
                                             gint max_undo_levels);
GtkSourceLanguage* gtk_source_buffer_get_language
                                            (GtkSourceBuffer *buffer);
void        gtk_source_buffer_set_language  (GtkSourceBuffer *buffer,
                                             GtkSourceLanguage *language);
gunichar    gtk_source_buffer_get_escape_char
                                            (GtkSourceBuffer *buffer);
void        gtk_source_buffer_set_escape_char
                                            (GtkSourceBuffer *buffer,
                                             gunichar escape_char);
*/

/*
 * Method: can_undo?
 *
 * Determines whether a source buffer can undo the last action.
 *
 * Returns: whether it's possible to undo the last action. 
 */
static VALUE
sourcebuffer_can_undo (self)
	VALUE self;
{
	return CBOOL2RVAL (gtk_source_buffer_can_undo (_SELF (self)));
}

/*
 * Method: can_redo?
 *
 * Determines whether a source buffer can redo the last action (i.e. if the
 * last operation was an undo).
 *
 * Returns: whether a redo is possible.
 */
static VALUE
sourcebuffer_can_redo (self)
	VALUE self;
{
	return CBOOL2RVAL (gtk_source_buffer_can_redo (_SELF (self)));
}

/*
 * Method: undo!
 *
 * Undoes the last user action which modified the buffer.
 * Use Gtk::SourceBuffer#can_undo? to check whether a call to this function
 * will have any effect.
 *
 * Actions are defined as groups of operations between a call to 
 * Gtk::TextBuffer#begin_user_action and Gtk::TextBuffer#end_user_action,
 * or sequences of similar edits (inserts or deletes) on the same line.
 *
 * Returns: self.
 */
static VALUE
sourcebuffer_undo (self)
	VALUE self;
{
	gtk_source_buffer_undo (_SELF (self));
	return self;
}

/*
 * Method: redo!
 *
 * Redoes the last undo operation. Use Gtk::SourceBuffer#can_redo? to check
 * whether a call to this function will have any effect.
 *
 * Returns: self.
 */
static VALUE
sourcebuffer_redo (self)
	VALUE self;
{
	gtk_source_buffer_redo (_SELF (self));
	return self;
}

/*
 * Method: begin_not_undoable_action
 * Method: begin_not_undoable_action { ... }
 * 
 * Marks the beginning of a not undoable action on the buffer, disabling the
 * undo manager.
 * 
 * If a block is given, the block is called after marking the beginning
 * of a not undoable action on the buffer.
 * At the end of the block, marks the end of a not undoable action on the
 * buffer. When the last not undoable block is finished, the list of undo
 * actions is cleared and the undo manager is re-enabled.
 *
 * Returns: self
 */
static VALUE
sourcebuffer_begin_not_undoable_action(self)
    VALUE self;
{
    gtk_source_buffer_begin_not_undoable_action (_SELF (self));

    if (rb_block_given_p()) {
	VALUE block = rb_block_proc ();
	rb_funcall (block, rb_intern ("call"), 0);
	gtk_source_buffer_end_not_undoable_action (_SELF (self));
    }
    return self;
}

/*
 * Method: end_not_undoable_action
 * 
 * Marks the end of a not undoable action on the buffer.
 * When the last not undoable block is finished, the list of undo
 * actions is cleared and the undo manager is re-enabled.
 *
 * Returns: self
 */
static VALUE
sourcebuffer_end_not_undoable_action(self)
    VALUE self;
{
    gtk_source_buffer_end_not_undoable_action (_SELF (self));
    return self;
}


/*
 * Method: not_undoable_action { ... }
 * 
 * Marks the beginning of a not undoable action on the buffer, disabling the
 * undo manager, then calls the provided block of code.
 * 
 * At the end of the block, marks the end of a not undoable action on the
 * buffer. When the last not undoable block is finished, the list of undo
 * actions is cleared and the undo manager is re-enabled.
 *
 * ((*Deprecated*)). Use Gtk::SourceView#begin_not_undoable_action{ ... } instead.
 *
 * Returns: the return value of the provided block.
 */
static VALUE
sourcebuffer_not_undoable_action (self)
	VALUE self;
{
	VALUE block, ret;

	block = rb_block_proc ();
	gtk_source_buffer_begin_not_undoable_action (_SELF (self));
	ret = rb_funcall (block, rb_intern ("call"), 0);
	gtk_source_buffer_end_not_undoable_action (_SELF (self));
	return ret;
}

/*
 * Method: create_marker(name=nil, type=nil, where)
 * name: the name of the marker.
 * type: a string defining the marker type. 
 * where: a location to place the marker, as a Gtk::TreeIter object. 
 *
 * Creates a marker in the buffer of the given type. A marker is semantically
 * very similar to a Gtk::TextMark, except it has a type which is used by the
 * Gtk::SourceView object displaying the buffer to show a pixmap on the left
 * margin, at the line the marker is in. Because of this, a marker is generally
 * associated to a line and not a character position. Markers are also
 * accessible through a position or range in the buffer.
 *
 * Markers are implemented using Gtk::TextMark, so all characteristics and
 * restrictions to marks apply to markers too. These includes life cycle issues
 * and "mark-set" and "mark-deleted" signal emissions.
 *
 * Like a Gtk::TextMark, a Gtk::SourceMarker can be anonymous if the passed
 * name is nil. 
 *
 * Markers always have left gravity and are moved to the beginning of the line
 * when the user deletes the line they were in. Also, if the user deletes a
 * region of text which contained lines with markers, those are deleted.
 *
 * Typical uses for a marker are bookmarks, breakpoints, current executing
 * instruction indication in a source file, etc..
 *
 * Returns: a new Gtk::SourceMarker object, owned by the buffer.
 */
static VALUE
sourcebuffer_create_marker (argc, argv, self)
	int argc;
	VALUE *argv, self;
{
	VALUE name, type, where;

	if (argc == 1)
		rb_scan_args (argc, argv, "12", &where, &name, &type);
	else
		rb_scan_args (argc, argv, "30", &name, &type, &where);

	return GOBJ2RVAL (gtk_source_buffer_create_marker (_SELF (self),
							   RVAL2CSTR (name),
							   RVAL2CSTR (type),
							   RVAL2ITR (where)));
}

/*
 * Method: move_marker(marker, where)
 * marker: a Gtk::SourceMarker object in the buffer.
 * where: the new location of the marker, as a Gtk::TreeIter object.
 *
 * Moves a marker to a new location in the buffer.
 * 
 * Returns: self.
 */
static VALUE
sourcebuffer_move_marker (self, marker, where)
	VALUE self, marker, where;
{
	gtk_source_buffer_move_marker (_SELF (self), RVAL2MARKER (marker),
				       RVAL2ITR (where));
	return self;
}

/*
 * Method: delete_marker(marker)
 * marker: a Gtk::SourceMarker object in the buffer.
 *
 * Deletes a marker from the source buffer. The same conditions as for
 * Gtk::TextMark apply here.
 *
 * Returns: self.
 */
static VALUE
sourcebuffer_delete_marker (self, marker)
	VALUE self, marker;
{
	gtk_source_buffer_delete_marker (_SELF (self), RVAL2MARKER (marker));
	return self;
}

/*
 * Method: get_marker(name)
 * name: the name of the marker to retrieve.
 *
 * Looks up the Gtk::SourceMarker named name in buffer, returning nil if it
 * doesn't exists.
 *
 * Returns: a reference to a Gtk::SourceMarker object, or nil if it's not found.
 */
static VALUE
sourcebuffer_get_marker (self, rname)
	VALUE self, rname;
{
	return
	    GOBJ2RVAL (gtk_source_buffer_get_marker
		       (_SELF (self), RVAL2CSTR (rname)));
}

/*
 * Method: first_marker
 *
 * Gets the first (nearest to the top of the buffer) marker in the buffer.
 *
 * Returns: a reference to the first Gtk::SourceMarker, or nil if there are
 * no markers in the buffer.
 */
static VALUE
sourcebuffer_get_first_marker (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_buffer_get_first_marker (_SELF (self)));
}

/*
 * Method: last_marker
 *
 * Returns the last (nearest to the bottom of the buffer) marker in buffer.
 *
 * Returns: a reference to the first Gtk::SourceMarker, or nil if there are
 * no markers in the buffer.
 */
static VALUE
sourcebuffer_get_last_marker (self)
	VALUE self;
{
	return GOBJ2RVAL (gtk_source_buffer_get_last_marker (_SELF (self)));
}

/*
 * Method: get_iter_at_marker(iter, marker)
 * iter: a Gtk::TextIter to initialize.
 * marker: a Gtk::SourceMarker of the buffer.
 *
 * Initializes iter at the location of marker.
 *
 * Returns: self.
 */
static VALUE
sourcebuffer_get_iter_at_marker (self, iter, marker)
	VALUE self, iter, marker;
{
	gtk_source_buffer_get_iter_at_marker (_SELF (self), RVAL2ITR (iter),
					      RVAL2MARKER (marker));
	return self;
}

/*
 * Method: get_next_marker(iter)
 * iter: the location to search from, as a Gtk::TextIter object.
 *
 * Returns the nearest marker to the right of iter. If there are multiple
 * markers at the same position, this method will always return the first one
 * (from the internal linked list), even if starting the search exactly at its
 * location. You can get the others using Gtk::SourceMarker#next.
 *
 * Returns: the Gtk::SourceMarker nearest to the right of iter, or nil if there
 * are no more markers after iter.
 */
static VALUE
sourcebuffer_get_next_marker (self, iter)
	VALUE self, iter;
{
	return
	    GOBJ2RVAL (gtk_source_buffer_get_next_marker
		       (_SELF (self), RVAL2ITR (iter)));
}

/*
 * Method: get_prev_marker(iter)
 * iter: the location to search from, as a Gtk::TextIter object.
 *
 * Returns the nearest marker to the left of iter. If there are multiple
 * markers at the same position, this method will always return the last one
 * (from the internal linked list), even if starting the search exactly at its
 * location. You can get the others using Gtk::SourceMarker#next.
 *
 * Returns: the Gtk::SourceMarker nearest to the left of iter, or nil if there
 * are no more markers after iter.
 */
static VALUE
sourcebuffer_get_prev_marker (self, iter)
	VALUE self, iter;
{
	return
	    GOBJ2RVAL (gtk_source_buffer_get_prev_marker
		       (_SELF (self), RVAL2ITR (iter)));
}

/*
 * Method: get_markers(begin, end)
 * begin: beginning of the range, as a Gtk::TextIter object.
 * end: end of the range, as a Gtk::TextIter object.
 *
 * Returns an ordered (by position) list of Gtk::SourceMarker objects inside
 * the region delimited by the Gtk::TextIter begin and end. The iters may be in
 * any order.
 *
 * Returns: an array of Gtk::SourceMarker objects inside the range.
 */
static VALUE
sourcebuffer_get_markers_in_region (self, begin, end)
	VALUE self, begin, end;
{
	GSList *list, *p;
	VALUE ary;

	list =
	    gtk_source_buffer_get_markers_in_region (_SELF (self),
						     RVAL2ITR (begin),
						     RVAL2ITR (end));
	ary = rb_ary_new ();

	p = (GSList *) list;
	while (p) {
		rb_ary_push (ary, GOBJ2RVAL (p->data));
		p = g_slist_next (p);
	}

	return ary;
}

void
Init_gtk_sourcebuffer ()
{
	VALUE cbuffer =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_BUFFER, "SourceBuffer", mGtk);

	rb_define_method (cbuffer, "initialize", sourcebuffer_new, -1);
	rb_define_method (cbuffer, "get_iter_at_marker",
			  sourcebuffer_get_iter_at_marker, 2);
	rb_define_method (cbuffer, "set_bracket_match_style",
			  sourcebuffer_set_bracket_match_style, 1);

	rb_define_method (cbuffer, "can_undo?", sourcebuffer_can_undo, 0);
	rb_define_method (cbuffer, "can_redo?", sourcebuffer_can_redo, 0);

	rb_define_method (cbuffer, "undo!", sourcebuffer_undo, 0);
	rb_define_method (cbuffer, "redo!", sourcebuffer_redo, 0);
	rb_define_method (cbuffer, "begin_not_undoable_action",
			  sourcebuffer_begin_not_undoable_action, 0);
	rb_define_method (cbuffer, "end_not_undoable_action",
			  sourcebuffer_end_not_undoable_action, 0);
	rb_define_method (cbuffer, "not_undoable_action",
			  sourcebuffer_not_undoable_action, 0);
	rb_define_alias (cbuffer, "non_undoable_action",
			 "not_undoable_action");
	rb_define_method (cbuffer, "create_marker", sourcebuffer_create_marker,
			  -1);
	rb_define_method (cbuffer, "move_marker", sourcebuffer_move_marker, 2);
	rb_define_method (cbuffer, "delete_marker", sourcebuffer_delete_marker,
			  1);
	rb_define_method (cbuffer, "get_marker", sourcebuffer_get_marker, 1);
	rb_define_method (cbuffer, "get_markers",
			  sourcebuffer_get_markers_in_region, 2);
	rb_define_method (cbuffer, "first_marker",
			  sourcebuffer_get_first_marker, 0);
	rb_define_method (cbuffer, "last_marker", sourcebuffer_get_last_marker,
			  0);

	rb_define_method (cbuffer, "get_prev_marker",
			  sourcebuffer_get_prev_marker, 1);

	rb_define_method (cbuffer, "get_next_marker",
			  sourcebuffer_get_next_marker, 1);

	G_DEF_SETTERS (cbuffer);
}
