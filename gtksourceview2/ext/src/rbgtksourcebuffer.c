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

/*
 * Class method: new(obj=nil)
 * obj: either a Gtk::TextTagTable, a Gtk::SourceLanguage, or nil.
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
		(val, GTYPE2CLASS (GTK_TYPE_TEXT_TAG_TABLE))) {
		G_INITIALIZE (self,
			      gtk_source_buffer_new (GTK_TEXT_TAG_TABLE
						     (RVAL2GOBJ (val))));
	} else
	    if (rb_obj_is_kind_of
		(val, GTYPE2CLASS (GTK_TYPE_SOURCE_LANGUAGE))) {
		G_INITIALIZE (self,
			      gtk_source_buffer_new_with_language
			      (GTK_SOURCE_LANGUAGE (RVAL2GOBJ (val))));
	} else {
		rb_raise (rb_eArgError,
			  "invalid argument %s (expect nil, Gtk::TextTagTable or Gtk::SourceLanguage)",
			  rb_class2name (CLASS_OF (val)));
	}
	return Qnil;
}

/* Defined as properties.
void                gtk_source_buffer_set_highlight_syntax
                                                        (GtkSourceBuffer *buffer,
                                                         gboolean highlight);
gboolean            gtk_source_buffer_get_highlight_syntax
                                                        (GtkSourceBuffer *buffer);
void                gtk_source_buffer_set_language      (GtkSourceBuffer *buffer,
                                                         GtkSourceLanguage *language);
GtkSourceLanguage*  gtk_source_buffer_get_language      (GtkSourceBuffer *buffer);
void                gtk_source_buffer_set_highlight_matching_brackets
                                                        (GtkSourceBuffer *buffer,
                                                         gboolean highlight);
gboolean            gtk_source_buffer_get_highlight_matching_brackets
                                                        (GtkSourceBuffer *buffer);
void                gtk_source_buffer_set_style_scheme  (GtkSourceBuffer *buffer,
                                                         GtkSourceStyleScheme *scheme);
GtkSourceStyleScheme* gtk_source_buffer_get_style_scheme
                                                        (GtkSourceBuffer *buffer);
gint                gtk_source_buffer_get_max_undo_levels
                                                        (GtkSourceBuffer *buffer);
void                gtk_source_buffer_set_max_undo_levels
                                                        (GtkSourceBuffer *buffer,
                                                         gint max_undo_levels);
gboolean            gtk_source_buffer_can_redo          (GtkSourceBuffer *buffer);
gboolean            gtk_source_buffer_can_undo          (GtkSourceBuffer *buffer);
*/

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

#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
/*
 * Method: create_source_mark(name=nil, category, where)
 * name: the name of the mark.
 * type: a string defining the mark type.
 * where: a location to place the mark, as a Gtk::TreeIter object.
 *
 * Creates a mark in the buffer of the given type. A mark is semantically
 * very similar to a Gtk::TextMark, except it has a type which is used by the
 * Gtk::SourceView object displaying the buffer to show a pixmap on the left
 * margin, at the line the mark is in. Because of this, a mark is generally
 * associated to a line and not a character position. Marks are also
 * accessible through a position or range in the buffer.
 *
 * Marks are implemented using Gtk::TextMark, so all characteristics and
 * restrictions to marks apply to marks too. These includes life cycle issues
 * and "mark-set" and "mark-deleted" signal emissions.
 *
 * Like a Gtk::TextMark, a Gtk::SourceMark can be anonymous if the passed
 * name is nil.
 *
 * Marks always have left gravity and are moved to the beginning of the line
 * when the user deletes the line they were in. Also, if the user deletes a
 * region of text which contained lines with marks, those are deleted.
 *
 * Typical uses for a mark are bookmarks, breakpoints, current executing
 * instruction indication in a source file, etc..
 *
 * Returns: a new Gtk::SourceMark object, owned by the buffer.
 */
static VALUE
sourcebuffer_create_source_mark (argc, argv, self)
	int argc;
	VALUE *argv, self;
{
	VALUE name, category, where;

	if (argc == 2)
		rb_scan_args (argc, argv, "21", &where, &category, &name);
	else
		rb_scan_args (argc, argv, "30", &name, &category, &where);

	return GOBJ2RVAL (gtk_source_buffer_create_source_mark (_SELF (self),
							   RVAL2CSTR (name),
							   RVAL2CSTR (category),
							   RVAL2ITR (where)));
}

static VALUE
sourcebuffer_get_source_marks_at_line (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GSList *list, *p;
    VALUE line, category;
    VALUE ary;

    rb_scan_args (argc, argv, "11", &line, &category);

    list =
        gtk_source_buffer_get_source_marks_at_line (_SELF (self),
                          NUM2INT (line),
                          NIL_P (category) ? NULL : RVAL2CSTR (category));
    ary = rb_ary_new ();

    p = (GSList *) list;
    while (p) {
        rb_ary_push (ary, GOBJ2RVAL (p->data));
        p = g_slist_next (p);
    }

    return ary;
}

static VALUE
sourcebuffer_get_source_marks_at_iter (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GSList *list, *p;
    VALUE iter, category;
    VALUE ary;

    rb_scan_args (argc, argv, "11", &iter, &category);

    list =
        gtk_source_buffer_get_source_marks_at_iter (_SELF (self),
                          RVAL2ITR (iter),
                          NIL_P (category) ? NULL : RVAL2CSTR (category));
    ary = rb_ary_new ();

    p = (GSList *) list;
    while (p) {
        rb_ary_push (ary, GOBJ2RVAL (p->data));
        p = g_slist_next (p);
    }

    return ary;
}

static VALUE
sourcebuffer_remove_source_marks (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE start, end, category;

    rb_scan_args (argc, argv, "21", &start, &end, &category);

    gtk_source_buffer_remove_source_marks (_SELF (self),
                            RVAL2ITR (start),
                            RVAL2ITR (end),
                            NIL_P (category) ? NULL : RVAL2CSTR (category));

    return self;
}

static VALUE
sourcebuffer_forward_iter_to_source_mark (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE iter, category;

    rb_scan_args (argc, argv, "11", &iter, &category);

    return
        CBOOL2RVAL (gtk_source_buffer_forward_iter_to_source_mark
                                   (_SELF (self), RVAL2ITR (iter),
                                    NIL_P (category) ? NULL : RVAL2CSTR (category)));
}

static VALUE
sourcebuffer_backward_iter_to_source_mark (argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE iter, category;

    rb_scan_args (argc, argv, "11", &iter, &category);

    return
        CBOOL2RVAL (gtk_source_buffer_backward_iter_to_source_mark
                                   (_SELF (self), RVAL2ITR (iter),
                                    NIL_P (category) ? NULL : RVAL2CSTR (category)));
}

#endif /* HAVE_GTK_SOURCE_MARK_GET_TYPE */

static VALUE
sourcebuffer_ensure_highlight (self, start, end)
    VALUE self, start, end;
{
    gtk_source_buffer_ensure_highlight (_SELF (self), RVAL2ITR (start), RVAL2ITR (end));

    return self;
}

void
Init_gtk_sourcebuffer ()
{
	VALUE cbuffer =
	    G_DEF_CLASS (GTK_TYPE_SOURCE_BUFFER, "SourceBuffer", mGtk);

	rb_define_method (cbuffer, "initialize", sourcebuffer_new, -1);
	rb_define_method (cbuffer, "redo!", sourcebuffer_redo, 0);
	rb_define_method (cbuffer, "undo!", sourcebuffer_undo, 0);
	rb_define_method (cbuffer, "begin_not_undoable_action",
			          sourcebuffer_begin_not_undoable_action, 0);
	rb_define_method (cbuffer, "end_not_undoable_action",
			          sourcebuffer_end_not_undoable_action, 0);
	rb_define_method (cbuffer, "not_undoable_action",
			          sourcebuffer_not_undoable_action, 0);
	rb_define_alias (cbuffer, "non_undoable_action", "not_undoable_action");
#ifdef HAVE_GTK_SOURCE_MARK_GET_TYPE
    rb_define_method (cbuffer, "create_source_mark",
                      sourcebuffer_create_source_mark, -1);
    rb_define_method (cbuffer, "get_source_marks_at_line",
                      sourcebuffer_get_source_marks_at_line, -1);
    rb_define_method (cbuffer, "get_source_marks_at_iter",
                      sourcebuffer_get_source_marks_at_iter, -1);
    rb_define_method (cbuffer, "remove_source_marks",
                      sourcebuffer_remove_source_marks, -1);
    rb_define_method (cbuffer, "forward_iter_to_source_mark",
                      sourcebuffer_forward_iter_to_source_mark, -1);
    rb_define_method (cbuffer, "backward_iter_to_source_mark",
                      sourcebuffer_backward_iter_to_source_mark, -1);
#endif
    rb_define_method (cbuffer, "ensure_highlight",
                      sourcebuffer_ensure_highlight, 2);

	G_DEF_SETTERS (cbuffer);
}
