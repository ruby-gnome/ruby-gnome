/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextbuffer.c -

  $Author: ktou $
  $Date: 2006/09/24 13:12:30 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003 Masahiro Sakai
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TEXT_BUFFER(RVAL2GOBJ(s)))
#define RVAL2ITR(i) (GtkTextIter*)RVAL2BOXED(i, GTK_TYPE_TEXT_ITER)
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))
#define RVAL2MARK(m) (GTK_TEXT_MARK(RVAL2GOBJ(m)))
#define N_RVAL2CSTR(text) (NIL_P(text) ? NULL : RVAL2CSTR(text))
#define RVAL2TAG(t) (GTK_TEXT_TAG(RVAL2GOBJ(t)))
#define RVAL2ANCHOR(a) (GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(a)))

static ID id_tagtable;

static VALUE
txt_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE table;
    rb_scan_args(argc, argv, "01", &table);
    if (NIL_P(table))
        G_INITIALIZE(self, gtk_text_buffer_new(NULL));
    else {
        G_CHILD_SET(self, id_tagtable, table);
        G_INITIALIZE(self, gtk_text_buffer_new(GTK_TEXT_TAG_TABLE(RVAL2GOBJ(table))));
    }
    return Qnil;
}

static VALUE
txt_get_line_count(self)
    VALUE self;
{
    return INT2NUM(gtk_text_buffer_get_line_count(_SELF(self)));
}

static VALUE
txt_get_char_count(self)
    VALUE self;
{
    return INT2NUM(gtk_text_buffer_get_char_count(_SELF(self)));
}

static VALUE
txt_get_tag_table(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_tag_table(_SELF(self)));
}

static VALUE
txt_set_text(self, text)
    VALUE self, text;
{
    StringValue(text);
    gtk_text_buffer_set_text(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
txt_backspace(self, iter, interactive, default_editable)
    VALUE self, iter, interactive, default_editable;
{
    return CBOOL2RVAL(gtk_text_buffer_backspace(_SELF(self), RVAL2ITR(iter),
                                                RTEST(interactive),
                                                RTEST(default_editable)));
}
#endif

static VALUE
txt_insert_at_cursor(self, text)
    VALUE self, text;
{
    StringValue(text);
    gtk_text_buffer_insert_at_cursor(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

static VALUE
txt_insert_interactive(self, iter, text, editable)
    VALUE self, iter, text, editable;
{
    StringValue(text);
    return gtk_text_buffer_insert_interactive(_SELF(self), RVAL2ITR(iter),
                                              RVAL2CSTR(text),
                                              RSTRING(text)->len,
                                              RTEST(editable))
        ? Qtrue : Qfalse;
}

static VALUE
txt_insert_interactive_at_cursor(self, text, editable)
    VALUE self, text, editable;
{
    StringValue(text);
    return gtk_text_buffer_insert_interactive_at_cursor(_SELF(self),
                                                        RVAL2CSTR(text),
                                                        RSTRING(text)->len,
                                                        RTEST(editable))
        ? Qtrue : Qfalse;
}

static VALUE
txt_insert_range(self, iter, start, end)
    VALUE self, iter, start, end;
{
    gtk_text_buffer_insert_range(_SELF(self), RVAL2ITR(iter),
                                 RVAL2ITR(start), RVAL2ITR(end));
    return self;
}

static VALUE
txt_insert_range_interactive(self, iter, start, end, editable)
    VALUE self, iter, start, end, editable;
{
    return gtk_text_buffer_insert_range_interactive(_SELF(self),
                                                    RVAL2ITR(iter),
                                                    RVAL2ITR(start),
                                                    RVAL2ITR(end),
                                                    RTEST(editable))
        ? Qtrue : Qfalse;
}

static VALUE
txt_delete(self, start, end)
    VALUE self, start, end;
{
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(RVAL2GOBJ(self)),
                           RVAL2ITR(start), RVAL2ITR(end));
    return self;
}

static VALUE
txt_delete_interactive(self, start, end, editable)
    VALUE self, start, end, editable;
{
    return gtk_text_buffer_delete_interactive(_SELF(self),
                                              RVAL2ITR(start),
                                              RVAL2ITR(end),
                                              RTEST(editable))
        ? Qtrue : Qfalse;
}

static VALUE
txt_get_text(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE start, end, include_hidden_chars;
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer* buffer = _SELF(self);
    gchar* ret;
    VALUE result;

    rb_scan_args(argc, argv, "03", &start, &end, &include_hidden_chars);

    if (NIL_P(start)) gtk_text_buffer_get_start_iter(buffer, &start_iter);
    if (NIL_P(end)) gtk_text_buffer_get_end_iter(buffer, &end_iter);
    if (NIL_P(include_hidden_chars)) include_hidden_chars = Qfalse;

    ret = gtk_text_buffer_get_text(
            buffer,
            NIL_P(start) ? &start_iter : RVAL2ITR(start),
            NIL_P(end) ? &end_iter : RVAL2ITR(end),
            RTEST(include_hidden_chars));
    result = CSTR2RVAL(ret);
    g_free(ret);

    return result;
}

static VALUE
txt_get_text_all(self)
    VALUE self;
{
    return txt_get_text(0, NULL, self);
}

static VALUE
txt_get_slice(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE start, end, include_hidden_chars;
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer* buffer = _SELF(self);
    gchar* ret;
    VALUE result;

    rb_scan_args(argc, argv, "03", &start, &end, &include_hidden_chars);

    if (NIL_P(start)) gtk_text_buffer_get_start_iter(buffer, &start_iter);
    if (NIL_P(end)) gtk_text_buffer_get_end_iter(buffer, &end_iter);
    if (NIL_P(include_hidden_chars)) include_hidden_chars = Qfalse;

    ret = gtk_text_buffer_get_slice(
            buffer,
            NIL_P(start) ? &start_iter : RVAL2ITR(start),
            NIL_P(end) ? &end_iter : RVAL2ITR(end),
            RTEST(include_hidden_chars));
    result = CSTR2RVAL(ret);
    g_free(ret);

    return result;
}

static VALUE
txt_get_slice_all(self)
    VALUE self;
{
    return txt_get_slice(0, NULL, self);
}

static VALUE
txt_insert_pixbuf(self, iter, pixbuf)
    VALUE self, iter, pixbuf;
{
    G_CHILD_ADD(self, iter);
    G_CHILD_ADD(iter, pixbuf);

    if (RTEST(ruby_debug))
        rb_warning("Gtk::TextBuffer#insert_pixbuf is deprecated. Use Gtk::TextBuffer#insert instead.");
    gtk_text_buffer_insert_pixbuf(_SELF(self), RVAL2ITR(iter),
                                  GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}

static VALUE
txt_insert_child_anchor(self, iter, anchor)
    VALUE self, iter, anchor;
{
    G_CHILD_ADD(self, iter);
    G_CHILD_ADD(iter, anchor);
    if (RTEST(ruby_debug))
        rb_warning("Gtk::TextBuffer#insert_child_anchor is deprecated. Use Gtk::TextBuffer#insert instead.");
    gtk_text_buffer_insert_child_anchor(_SELF(self), RVAL2ITR(iter),
                                        GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(anchor)));
    return self;
}

static VALUE
txt_create_child_anchor(self, iter)
    VALUE self, iter;
{
    VALUE ret = GOBJ2RVAL(gtk_text_buffer_create_child_anchor(_SELF(self), RVAL2ITR(iter)));
    G_CHILD_ADD(self, ret);
    return ret;
}

static VALUE
txt_create_mark(self, name, where, left_gravity)
    VALUE self, name, where, left_gravity;
{
    VALUE ret = GOBJ2RVAL(gtk_text_buffer_create_mark(_SELF(self),
                                                      NIL_P(name) ? NULL : RVAL2CSTR(name),
                                                      RVAL2ITR(where),
                                                      RTEST(left_gravity)));
    G_CHILD_ADD(self, ret);
    return ret;
}

static VALUE
txt_delete_mark(self, mark)
    VALUE self, mark;
{
    if (rb_obj_is_kind_of(mark, GTYPE2CLASS(GTK_TYPE_TEXT_MARK))){
        G_CHILD_REMOVE(self, mark);
        gtk_text_buffer_delete_mark(_SELF(self), RVAL2MARK(mark));
    } else {
        G_CHILD_REMOVE(self, GOBJ2RVAL(gtk_text_buffer_get_mark(_SELF(self), RVAL2CSTR(mark))));
        gtk_text_buffer_delete_mark_by_name(_SELF(self), RVAL2CSTR(mark));
    }
    return self;
}

static VALUE
txt_get_mark(self, name)
    VALUE self, name;
{
    return GOBJ2RVAL(gtk_text_buffer_get_mark(_SELF(self), RVAL2CSTR(name)));
}

/*
static VALUE
txt_get_insert(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_insert(_SELF(self)));
}
*/

static VALUE
txt_get_selection_bound(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_selection_bound(_SELF(self)));
}

static VALUE
txt_place_cursor(self, where)
    VALUE self, where;
{
    gtk_text_buffer_place_cursor(_SELF(self), RVAL2ITR(where));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
txt_select_range(self, ins, bound)
    VALUE self, ins, bound;
{
    gtk_text_buffer_select_range(_SELF(self), RVAL2ITR(ins), RVAL2ITR(bound));
    return self;
}
#endif

static VALUE
txt_get_iter_at_child_anchor(self, anchor)
    VALUE self, anchor;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_child_anchor(_SELF(self), &iter, RVAL2ANCHOR(anchor));
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_modified(self)
    VALUE self;
{
    return gtk_text_buffer_get_modified(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
txt_set_modified(self, setting)
    VALUE self, setting;
{
    gtk_text_buffer_set_modified(_SELF(self), RTEST(setting));
    return setting;
}

static VALUE
txt_add_selection_clipboard(self, clipboard)
    VALUE self, clipboard;
{
    G_CHILD_ADD(self, clipboard);
    gtk_text_buffer_add_selection_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard));
    return self;
}

static VALUE
txt_remove_selection_clipboard(self, clipboard)
    VALUE self, clipboard;
{
    G_CHILD_REMOVE(self, clipboard);
    gtk_text_buffer_remove_selection_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard));
    return self;
}

static VALUE
txt_cut_clipboard(self, clipboard, default_editable)
    VALUE self, clipboard, default_editable;
{
    G_CHILD_ADD(self, clipboard);
    gtk_text_buffer_cut_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard), RTEST(default_editable));
    return self;
}

static VALUE
txt_copy_clipboard(self, clipboard)
    VALUE self, clipboard;
{
    G_CHILD_ADD(self, clipboard);
    gtk_text_buffer_copy_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard));
    return self;
}

static VALUE
txt_paste_clipboard(self, clipboard, location, default_editable)
    VALUE self, clipboard, location, default_editable;
{
    G_CHILD_ADD(self, clipboard);
    gtk_text_buffer_paste_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard),
                                    NIL_P(location) ? NULL : RVAL2ITR(location),
                                    RTEST(default_editable));
    return self;
}

static VALUE
txt_get_selection_bounds(self)
    VALUE self;
{
    GtkTextIter start, end;
    
    gboolean ret = gtk_text_buffer_get_selection_bounds(_SELF(self), &start, &end);
    return rb_ary_new3(3, ITR2RVAL(&start), ITR2RVAL(&end), CBOOL2RVAL(ret));
}

static VALUE
txt_delete_selection(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE interactive, default_editable;
    rb_scan_args(argc, argv, "20", &interactive, &default_editable); 
    return gtk_text_buffer_delete_selection(_SELF(self),
        RTEST(interactive), RTEST(default_editable)) ? Qtrue : Qfalse;
}

static VALUE
txt_end_user_action(self)
    VALUE self;
{
    gtk_text_buffer_end_user_action(_SELF(self));
    return self;
}

static VALUE
txt_begin_user_action(self)
    VALUE self;
{
    gtk_text_buffer_begin_user_action(_SELF(self));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, txt_end_user_action, self);
    return self;
}

static VALUE
txt_get_start_iter(self)
    VALUE self;
{
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter(_SELF(self), &iter);
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_end_iter(self)
    VALUE self;
{
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(_SELF(self), &iter);
    return ITR2RVAL(&iter);
}

static VALUE
txt_move_mark(self, mark, where)
    VALUE self, mark, where;
{
    if (rb_obj_is_kind_of(mark, GTYPE2CLASS(GTK_TYPE_TEXT_MARK)))
        gtk_text_buffer_move_mark(_SELF(self), RVAL2MARK(mark), RVAL2ITR(where));
    else
        gtk_text_buffer_move_mark_by_name(_SELF(self), RVAL2CSTR(mark), RVAL2ITR(where));
    return self;
}

static VALUE
txt_create_tag(self, tag_name, properties)
    VALUE self, tag_name, properties;
{
    GtkTextTag *tag;
    VALUE ret;

    tag = gtk_text_tag_new(NIL_P(tag_name) ? (gchar*)NULL : RVAL2CSTR(tag_name));
    gtk_text_tag_table_add (gtk_text_buffer_get_tag_table(_SELF(self)), tag);

    G_SET_PROPERTIES(GOBJ2RVAL(tag), properties);
    
    ret = GOBJ2RVAL(tag);

    G_CHILD_ADD(self, ret);

    return ret;
}

static VALUE
txt_insert(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE where, value, tags;
    struct RArray *tarray;
    gint start_offset;
    GtkTextIter start;
    int i;

    rb_scan_args(argc, argv, "2*", &where, &value, &tags);
    tarray = RARRAY(tags);

    G_CHILD_ADD(self, where);
    G_CHILD_ADD(self, value);
    if (rb_obj_is_kind_of(value, GTYPE2CLASS(GDK_TYPE_PIXBUF))){
        gtk_text_buffer_insert_pixbuf(_SELF(self), RVAL2ITR(where),
                                      GDK_PIXBUF(RVAL2GOBJ(value)));
    } else if (rb_obj_is_kind_of(value, GTYPE2CLASS(GTK_TYPE_TEXT_CHILD_ANCHOR))){
        gtk_text_buffer_insert_child_anchor(_SELF(self), RVAL2ITR(where),
                                            GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(value)));
    } else {
        start_offset = gtk_text_iter_get_offset(RVAL2ITR(where));
        StringValue(value);
        gtk_text_buffer_insert(_SELF(self), RVAL2ITR(where), RVAL2CSTR(value), RSTRING(value)->len);
        
        if(tarray->len == 0)
            return self;

        G_CHILD_ADD(self, tags);
        
        gtk_text_buffer_get_iter_at_offset(_SELF(self), &start, start_offset);
        
        for(i=0; i<tarray->len; i++) {
            GtkTextTag *tag;
            
            if (rb_obj_is_kind_of(tarray->ptr[i], GTYPE2CLASS(GTK_TYPE_TEXT_TAG))) {
                tag = RVAL2GOBJ(tarray->ptr[i]);
            } else {
                tag = gtk_text_tag_table_lookup(_SELF(self)->tag_table, RVAL2CSTR(tarray->ptr[i]));
                if (tag == NULL) {
                    g_warning ("%s: no tag with name '%s'!", G_STRLOC, RVAL2CSTR(tarray->ptr[i]));
                    return self;
                }
            }
            gtk_text_buffer_apply_tag(_SELF(self), tag, &start, RVAL2ITR(where));
        }
    }
    return self;
}

static VALUE
txt_insert_with_tags(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    if (RTEST(ruby_debug))
        rb_warning("Gtk::TextBuffer#insert_with_tags is deprecated. Use Gtk::TextBuffer#insert instead.");
    txt_insert(argc, argv, self);
    return self;
}

static VALUE
txt_apply_tag(self, tag, start, end)
    VALUE self, tag, start, end;
{
    if (rb_obj_is_kind_of(tag, GTYPE2CLASS(GTK_TYPE_TEXT_TAG)))
        gtk_text_buffer_apply_tag(_SELF(self), RVAL2TAG(tag), RVAL2ITR(start), RVAL2ITR(end));
    else
        gtk_text_buffer_apply_tag_by_name(_SELF(self), RVAL2CSTR(tag), RVAL2ITR(start), RVAL2ITR(end));

    return self;
}

static VALUE
txt_remove_tag(self, tag, start, end)
    VALUE self, tag, start, end;
{
    if (rb_obj_is_kind_of(tag, GTYPE2CLASS(GTK_TYPE_TEXT_TAG)))
        gtk_text_buffer_remove_tag(_SELF(self), RVAL2TAG(tag), RVAL2ITR(start), RVAL2ITR(end));
    else
        gtk_text_buffer_remove_tag_by_name(_SELF(self), RVAL2CSTR(tag), RVAL2ITR(start), RVAL2ITR(end));

    return self;
}

static VALUE
txt_remove_all_tags(self, start, end)
    VALUE self, start, end;
{
    gtk_text_buffer_remove_all_tags(_SELF(self), RVAL2ITR(start), RVAL2ITR(end));
    return self;
}

static VALUE
txt_get_iter_at_line_offset(self, line_number, char_offset)
    VALUE self, line_number, char_offset;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line_offset(_SELF(self), &iter, NUM2INT(line_number), NUM2INT(char_offset));
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_iter_at_line_index(self, line_number, byte_index)
    VALUE self, line_number, byte_index;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line_index(_SELF(self), &iter, NUM2INT(line_number), NUM2INT(byte_index));
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_iter_at_offset(self, char_offset)
    VALUE self, char_offset;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_offset(_SELF(self), &iter, NUM2INT(char_offset));
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_iter_at_line(self, line_number)
    VALUE self, line_number;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line(_SELF(self), &iter, NUM2INT(line_number));
    return ITR2RVAL(&iter);
}

static VALUE
txt_get_bounds(self)
    VALUE self;
{
    GtkTextIter start, end;
    VALUE result;
    
    gtk_text_buffer_get_bounds(_SELF(self), &start, &end);
    result = rb_ary_new();
    rb_ary_push(result, ITR2RVAL(&start));
    rb_ary_push(result, ITR2RVAL(&end));

    return result;
}

static VALUE
txt_get_iter_at_mark(self, mark)
    VALUE self, mark;
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_mark(_SELF(self), &iter, RVAL2MARK(mark));
    return ITR2RVAL(&iter);
}


void 
Init_gtk_textbuffer()
{
    VALUE gTextBuffer = G_DEF_CLASS(GTK_TYPE_TEXT_BUFFER, "TextBuffer", mGtk);

    id_tagtable = rb_intern("tagtable");

    rb_define_method(gTextBuffer, "initialize", txt_initialize, -1);
    rb_define_method(gTextBuffer, "line_count", txt_get_line_count, 0);
    rb_define_method(gTextBuffer, "char_count", txt_get_char_count, 0);
    rb_define_method(gTextBuffer, "tag_table", txt_get_tag_table, 0);

    rb_define_method(gTextBuffer, "set_text", txt_set_text, 1);
    rb_define_method(gTextBuffer, "insert", txt_insert, -1);
    rb_define_method(gTextBuffer, "insert_with_tags", txt_insert_with_tags, -1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gTextBuffer, "backspace", txt_backspace, 3);
#endif
    rb_define_method(gTextBuffer, "insert_at_cursor", txt_insert_at_cursor, 1);
    rb_define_method(gTextBuffer, "insert_interactive", txt_insert_interactive, 3);
    rb_define_method(gTextBuffer, "insert_interactive_at_cursor", txt_insert_interactive_at_cursor, 2);
    rb_define_method(gTextBuffer, "insert_range", txt_insert_range, 3);
    rb_define_method(gTextBuffer, "insert_range_interactive", txt_insert_range_interactive, 4);

    rb_define_method(gTextBuffer, "delete", txt_delete, 2);
    rb_define_method(gTextBuffer, "delete_interactive", txt_delete_interactive, 3);

    rb_define_method(gTextBuffer, "get_text", txt_get_text, -1);
    rb_define_method(gTextBuffer, "text", txt_get_text_all, 0);
    rb_define_method(gTextBuffer, "get_slice", txt_get_slice, -1);
    rb_define_method(gTextBuffer, "slice", txt_get_slice_all, 0);

    rb_define_method(gTextBuffer, "insert_pixbuf", txt_insert_pixbuf, 2);
    rb_define_method(gTextBuffer, "insert_child_anchor", txt_insert_child_anchor, 2);
    rb_define_method(gTextBuffer, "create_child_anchor", txt_create_child_anchor, 1);
    
    rb_define_method(gTextBuffer, "create_mark", txt_create_mark, 3);
    rb_define_method(gTextBuffer, "delete_mark", txt_delete_mark, 1);

    rb_define_method(gTextBuffer, "get_mark", txt_get_mark, 1);
/* Comment out because this method's name is very bad.
   Use Gtk::TextBuffer#get_mark("insert") instead.
    rb_define_method(gTextBuffer, "get_insert", txt_get_insert, 0);
*/
    rb_define_method(gTextBuffer, "selection_bound", txt_get_selection_bound, 0);
    rb_define_method(gTextBuffer, "place_cursor", txt_place_cursor, 1);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gTextBuffer, "select_range", txt_select_range, 2);
#endif
    rb_define_method(gTextBuffer, "modified?", txt_get_modified, 0);
    rb_define_method(gTextBuffer, "set_modified", txt_set_modified, 1);

    rb_define_method(gTextBuffer, "add_selection_clipboard", txt_add_selection_clipboard, 1);
    rb_define_method(gTextBuffer, "remove_selection_clipboard", txt_remove_selection_clipboard, 1);
    rb_define_method(gTextBuffer, "cut_clipboard", txt_cut_clipboard, 2);
    rb_define_method(gTextBuffer, "copy_clipboard", txt_copy_clipboard, 1);
    rb_define_method(gTextBuffer, "paste_clipboard", txt_paste_clipboard, 3);
    
    rb_define_method(gTextBuffer, "selection_bounds", txt_get_selection_bounds, 0);
    rb_define_method(gTextBuffer, "delete_selection", txt_delete_selection, -1);

    rb_define_method(gTextBuffer, "begin_user_action", txt_begin_user_action, 0);
    rb_define_method(gTextBuffer, "end_user_action", txt_end_user_action, 0);

    rb_define_method(gTextBuffer, "start_iter", txt_get_start_iter, 0);
    rb_define_method(gTextBuffer, "end_iter", txt_get_end_iter, 0);
    rb_define_method(gTextBuffer, "get_iter_at_line_offset", txt_get_iter_at_line_offset, 2);
    rb_define_method(gTextBuffer, "get_iter_at_line_index", txt_get_iter_at_line_index, 2);
    rb_define_method(gTextBuffer, "get_iter_at_offset", txt_get_iter_at_offset, 1);
    rb_define_method(gTextBuffer, "get_iter_at_line", txt_get_iter_at_line, 1);
    rb_define_method(gTextBuffer, "bounds", txt_get_bounds, 0);
    rb_define_method(gTextBuffer, "get_iter_at_mark", txt_get_iter_at_mark, 1);
    rb_define_method(gTextBuffer, "move_mark", txt_move_mark, 2);
    rb_define_method(gTextBuffer, "get_iter_at_child_anchor", txt_get_iter_at_child_anchor, 1);
    
    rb_define_method(gTextBuffer, "create_tag", txt_create_tag, 2);
    rb_define_method(gTextBuffer, "apply_tag", txt_apply_tag, 3);
    rb_define_method(gTextBuffer, "remove_tag", txt_remove_tag, 3);
    rb_define_method(gTextBuffer, "remove_all_tags", txt_remove_all_tags, 2);

    G_DEF_SETTERS(gTextBuffer);
}

