/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE cBuffer

/* Class: Gtk::SourceBuffer
 * Text buffer object for Gtk::SourceView.
 */

#define _SELF(self) (RVAL2GTKSOURCEBUFFER(self))

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
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE val;

    rb_scan_args (argc, argv, "01", &val);
    if (NIL_P (val)) {
        G_INITIALIZE (self, gtk_source_buffer_new (NULL));
    } else
        if (rb_obj_is_kind_of
        (val, GTYPE2CLASS (GTK_TYPE_TEXT_TAG_TABLE))) {
        G_INITIALIZE (self,
                  gtk_source_buffer_new(RVAL2GTKTEXTTAGTABLE(val)));
    } else
        if (rb_obj_is_kind_of
        (val, GTYPE2CLASS (GTK_SOURCE_TYPE_LANGUAGE))) {
        G_INITIALIZE (self,
                  gtk_source_buffer_new_with_language(RVAL2GTKSOURCELANGUAGE(val)));
    } else {
        rb_raise (rb_eArgError,
              "invalid argument %s (expect nil, Gtk::TextTagTable or Gtk::SourceLanguage)",
              rb_class2name (CLASS_OF (val)));
    }
    return Qnil;
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
rg_redo_bang(VALUE self)
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
rg_undo_bang(VALUE self)
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
rg_begin_not_undoable_action(VALUE self)
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
rg_end_not_undoable_action(VALUE self)
{
    gtk_source_buffer_end_not_undoable_action (_SELF (self));
    return self;
}

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
rg_create_source_mark(int argc, VALUE *argv, VALUE self)
{
    VALUE name, category, where;

    if (argc == 2)
        rb_scan_args (argc, argv, "21", &where, &category, &name);
    else
        rb_scan_args (argc, argv, "30", &name, &category, &where);

    return GOBJ2RVAL (gtk_source_buffer_create_source_mark (_SELF (self),
                               RVAL2CSTR (name),
                               RVAL2CSTR_ACCEPT_SYMBOL (category),
                               RVAL2GTKTEXTITER (where)));
}

static VALUE
rg_get_source_marks_at_line(int argc, VALUE *argv, VALUE self)
{
    VALUE line, category;

    rb_scan_args (argc, argv, "11", &line, &category);

    /* TODO: need free? */
    return GOBJGSLIST2RVAL_FREE(gtk_source_buffer_get_source_marks_at_line(_SELF(self),
                                        NUM2INT(line),
                                        RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(category)),
                                g_slist_free, NULL);
}

static VALUE
rg_get_source_marks_at_iter(int argc, VALUE *argv, VALUE self)
{
    VALUE iter, category;

    rb_scan_args (argc, argv, "11", &iter, &category);

    /* TODO: need free? */
    return GOBJGSLIST2RVAL_FREE(gtk_source_buffer_get_source_marks_at_iter(_SELF(self),
                                        RVAL2GTKTEXTITER(iter),
                                        RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(category)),
                                g_slist_free, NULL);
}

static VALUE
rg_remove_source_marks(int argc, VALUE *argv, VALUE self)
{
    VALUE start, end, category;

    rb_scan_args (argc, argv, "21", &start, &end, &category);

    gtk_source_buffer_remove_source_marks (_SELF (self),
                            RVAL2GTKTEXTITER (start),
                            RVAL2GTKTEXTITER (end),
                            RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL (category));

    return self;
}

static VALUE
rg_forward_iter_to_source_mark(int argc, VALUE *argv, VALUE self)
{
    VALUE iter, category;

    rb_scan_args (argc, argv, "11", &iter, &category);

    return
        CBOOL2RVAL (gtk_source_buffer_forward_iter_to_source_mark
                                   (_SELF (self), RVAL2GTKTEXTITER (iter),
                                    RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL (category)));
}

static VALUE
rg_backward_iter_to_source_mark(int argc, VALUE *argv, VALUE self)
{
    VALUE iter, category;

    rb_scan_args (argc, argv, "11", &iter, &category);

    return
        CBOOL2RVAL (gtk_source_buffer_backward_iter_to_source_mark
                                   (_SELF (self), RVAL2GTKTEXTITER (iter),
                                    RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL (category)));
}

static VALUE
rg_ensure_highlight(VALUE self, VALUE start, VALUE end)
{
    gtk_source_buffer_ensure_highlight (_SELF (self), RVAL2GTKTEXTITER (start), RVAL2GTKTEXTITER (end));

    return self;
}

void
Init_gtksource_buffer (VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS (GTK_SOURCE_TYPE_BUFFER, "Buffer", mGtkSource);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD_BANG(redo, 0);
    RG_DEF_METHOD_BANG(undo, 0);
    RG_DEF_METHOD(begin_not_undoable_action, 0);
    RG_DEF_METHOD(end_not_undoable_action, 0);
    RG_DEF_METHOD(create_source_mark, -1);
    RG_DEF_METHOD(get_source_marks_at_line, -1);
    RG_DEF_METHOD(get_source_marks_at_iter, -1);
    RG_DEF_METHOD(remove_source_marks, -1);
    RG_DEF_METHOD(forward_iter_to_source_mark, -1);
    RG_DEF_METHOD(backward_iter_to_source_mark, -1);
    RG_DEF_METHOD(ensure_highlight, 2);
}
