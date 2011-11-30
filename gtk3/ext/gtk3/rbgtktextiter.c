/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masahiro Sakai
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

#include "global.h"

#define RG_TARGET_NAMESPACE cTextIter
#define _SELF(s) ((GtkTextIter*)RVAL2BOXED(s, GTK_TYPE_TEXT_ITER))
#define RVAL2TAG(t) (GTK_TEXT_TAG(RVAL2GOBJ(t)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))

static gboolean is_compat_240;
static ID id_pixbuf;

static VALUE
rg_buffer(VALUE self)
{
    return GOBJ2RVAL(gtk_text_iter_get_buffer(_SELF(self)));
}

static VALUE
get_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_offset(_SELF(self)));
}

static VALUE
get_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line(_SELF(self)));
}

static VALUE
get_line_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line_offset(_SELF(self)));
}

static VALUE
get_line_index(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line_index(_SELF(self)));
}

static VALUE
get_visible_line_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_visible_line_offset(_SELF(self)));
}

static VALUE
get_visible_line_index(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_visible_line_index(_SELF(self)));
}

static VALUE
rg_char(VALUE self)
{
    gchar buf[10];
    gint len = g_unichar_to_utf8(gtk_text_iter_get_char(_SELF(self)), buf);
    buf[len] = '\0';
    return CSTR2RVAL(buf);
}

static VALUE
rg_get_slice(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_slice(_SELF(self), _SELF(rhs)));
}

static VALUE
rg_get_text(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_text(_SELF(self), _SELF(rhs)));
}

static VALUE
rg_get_visible_slice(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_visible_slice(_SELF(self), _SELF(rhs)));
}

static VALUE
rg_get_visible_text(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_visible_text(_SELF(self), _SELF(rhs)));
}

static VALUE
rg_pixbuf(VALUE self)
{
    GdkPixbuf* pixbuf = gtk_text_iter_get_pixbuf(_SELF(self));
    VALUE ret = Qnil;
    if (pixbuf){
        ret = GOBJ2RVAL(pixbuf);
        G_CHILD_SET(self, id_pixbuf, ret);
    }
    return ret;
}

static VALUE
rg_marks(VALUE self)
{
    return GSLIST2ARYF(gtk_text_iter_get_marks(_SELF(self)));
}

static VALUE
rg_toggled_tags(VALUE self, VALUE toggled_on)
{
    return GSLIST2ARYF(gtk_text_iter_get_toggled_tags(_SELF(self), RVAL2CBOOL(toggled_on)));
}

static VALUE
rg_child_anchor(VALUE self)
{
    GtkTextChildAnchor* anchor = gtk_text_iter_get_child_anchor(_SELF(self));
    return anchor ? GOBJ2RVAL(anchor) : Qnil;
}

static VALUE
rg_begins_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_begins_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
rg_ends_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_ends_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
rg_toggles_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_toggles_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
rg_has_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_has_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
rg_tags(VALUE self)
{
    return GSLIST2ARYF(gtk_text_iter_get_tags(_SELF(self)));
}

static VALUE
rg_editable_p(VALUE self, VALUE default_setting)
{
    return CBOOL2RVAL(gtk_text_iter_editable(_SELF(self), RVAL2CBOOL(default_setting)));
}

static VALUE
rg_can_insert_p(VALUE self, VALUE default_setting)
{
    return CBOOL2RVAL(gtk_text_iter_can_insert(_SELF(self), RVAL2CBOOL(default_setting)));
}

static VALUE
starts_word(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_word(_SELF(self)));
}

static VALUE
ends_word(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_word(_SELF(self)));
}

static VALUE
inside_word(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_inside_word(_SELF(self)));
}

static VALUE
starts_sentence(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_sentence(_SELF(self)));
}

static VALUE
ends_sentence(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_sentence(_SELF(self)));
}

static VALUE
starts_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_line(_SELF(self)));
}

static VALUE
ends_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_line(_SELF(self)));
}

static VALUE
is_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_cursor_position(_SELF(self)));
}

static VALUE
get_chars_in_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_chars_in_line(_SELF(self)));
}

static VALUE
get_bytes_in_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_bytes_in_line(_SELF(self)));
}

static VALUE
rg_attributes(VALUE self)
{
    GtkTextAttributes attr;

    if(gtk_text_iter_get_attributes(_SELF(self), &attr) == TRUE){
        return BOXED2RVAL(&attr, GTK_TYPE_TEXT_ATTRIBUTES);
    } else {
        return Qnil;
    }
}

static VALUE
rg_language(VALUE self)
{
    return CSTR2RVAL(pango_language_to_string(gtk_text_iter_get_language(_SELF(self))));
}

static VALUE
is_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_end(_SELF(self)));
}

static VALUE
is_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_start(_SELF(self)));
}

static VALUE
forward_char(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_char(_SELF(self)));
}

static VALUE
backward_char(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_char(_SELF(self)));
}

static VALUE
forward_chars(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_chars(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_chars(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_chars(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_line(_SELF(self)));
}

static VALUE
backward_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_line(_SELF(self)));
}

static VALUE
forward_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_word_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_word_end(_SELF(self)));
}

static VALUE
backward_word_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_word_start(_SELF(self)));
}

static VALUE
forward_word_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_word_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_word_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_word_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_sentence_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_sentence_end(_SELF(self)));
}

static VALUE
backward_sentence_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_sentence_start(_SELF(self)));
}

static VALUE
forward_sentence_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_sentence_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_sentence_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_sentence_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_visible_word_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_word_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_visible_word_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_word_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_visible_word_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_word_end(_SELF(self)));
}

static VALUE
backward_visible_word_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_word_start(_SELF(self)));
}

static VALUE
forward_visible_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_cursor_position(_SELF(self)));
}

static VALUE
backward_visible_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_cursor_position(_SELF(self)));
}

static VALUE
forward_visible_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_visible_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_visible_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_line(_SELF(self)));
}

static VALUE
backward_visible_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_line(_SELF(self)));
}

static VALUE
forward_visible_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_visible_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_cursor_position(_SELF(self)));
}

static VALUE
backward_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_cursor_position(_SELF(self)));
}

static VALUE
forward_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
backward_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
forward_to_line_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_to_line_end(_SELF(self)));
}

static VALUE
rg_forward_to_end(VALUE self)
{
    gtk_text_iter_forward_to_end(_SELF(self));
    return self;
}

static VALUE
set_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_offset(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
set_line(VALUE self, VALUE val)
{
    gtk_text_iter_set_line(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
set_line_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_line_offset(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
set_line_index(VALUE self, VALUE val)
{
    gtk_text_iter_set_line_index(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
set_visible_line_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_visible_line_offset(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
set_visible_line_index(VALUE self, VALUE val)
{
    gtk_text_iter_set_visible_line_index(_SELF(self), NUM2INT(val));
    return val;
}

static VALUE
rg_forward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
{
    VALUE tag;

    rb_scan_args(argc, argv, "01", &tag);
    return CBOOL2RVAL(gtk_text_iter_forward_to_tag_toggle(_SELF(self),
                                                          NIL_P(tag) ? NULL : RVAL2TAG(tag)));
}

static VALUE
rg_backward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
{
    VALUE tag;

    rb_scan_args(argc, argv, "01", &tag);
    return CBOOL2RVAL(gtk_text_iter_backward_to_tag_toggle(_SELF(self),
                                                           NIL_P(tag) ? NULL : RVAL2TAG(tag)));
}

static gboolean
char_predicate_func(guint32 ch, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 1, UINT2NUM(ch)));
}

static VALUE
rg_forward_find_char(int argc, VALUE *argv, VALUE self)
{
    VALUE limit;
    volatile VALUE func = rb_block_proc();

    rb_scan_args(argc, argv, "01", &limit);
    return CBOOL2RVAL(gtk_text_iter_forward_find_char(_SELF(self),
                                                      (GtkTextCharPredicate)char_predicate_func, 
                                                      (gpointer)func,
                                                      NIL_P(limit) ? NULL : _SELF(limit)));
}

static VALUE
rg_backward_find_char(int argc, VALUE *argv, VALUE self)
{
    VALUE limit;
    volatile VALUE func = rb_block_proc();

    rb_scan_args(argc, argv, "01", &limit);
    return CBOOL2RVAL(gtk_text_iter_backward_find_char(_SELF(self),
                                                       (GtkTextCharPredicate)char_predicate_func,
                                                       (gpointer)func,
                                                       NIL_P(limit) ? NULL : _SELF(limit)));
}

static VALUE
rg_forward_search(int argc, VALUE *argv, VALUE self)
{
    GtkTextIter m_start, m_end;
    VALUE str, flags, limit;
    gboolean ret;

    rb_scan_args(argc, argv, "21", &str, &flags, &limit);

    if (is_compat_240){
        ret = gtk_text_iter_forward_search(_SELF(self), RVAL2CSTR(str),
                                           RVAL2GFLAGS(flags, GTK_TYPE_TEXT_SEARCH_FLAGS), 
                                           &m_start, &m_end,
                                           NIL_P(limit) ? NULL : _SELF(limit));
    } else {
        ret = gtk_text_iter_forward_search(_SELF(self), RVAL2CSTR(str),
                                           RVAL2GENUM(flags, GTK_TYPE_TEXT_SEARCH_FLAGS), 
                                           &m_start, &m_end,
                                           NIL_P(limit) ? NULL : _SELF(limit));
    }
    return ret ? rb_ary_new3(2, ITR2RVAL(&m_start), ITR2RVAL(&m_end)) : Qnil;
}

static VALUE
rg_backward_search(int argc, VALUE *argv, VALUE self)
{
    GtkTextIter m_start, m_end;
    VALUE str, flags, limit;
    gboolean ret;

    rb_scan_args(argc, argv, "21", &str, &flags, &limit);
    if (is_compat_240){
        ret = gtk_text_iter_backward_search(_SELF(self), RVAL2CSTR(str),
                                            RVAL2GFLAGS(flags, GTK_TYPE_TEXT_SEARCH_FLAGS), 
                                            &m_start, &m_end,
                                            NIL_P(limit) ? NULL : _SELF(limit));
    } else {
        ret = gtk_text_iter_backward_search(_SELF(self), RVAL2CSTR(str),
                                            RVAL2GENUM(flags, GTK_TYPE_TEXT_SEARCH_FLAGS), 
                                            &m_start, &m_end,
                                            NIL_P(limit) ? NULL : _SELF(limit));
    }
    return ret ? rb_ary_new3(2, ITR2RVAL(&m_start), ITR2RVAL(&m_end)) : Qnil;
}

static VALUE
rg_operator_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gtk_text_iter_equal(_SELF(self), _SELF(other)));
}

static VALUE
rg_operator_compare(VALUE self, VALUE rhs)
{
    return INT2NUM(gtk_text_iter_compare(_SELF(self), _SELF(rhs)));
}

/*
  The following methods don't have to be implimented.
  Including Comparable module is enough.

gboolean    gtk_text_iter_in_range          (const GtkTextIter *iter,
                                             const GtkTextIter *start,
                                             const GtkTextIter *end);
void        gtk_text_iter_order             (GtkTextIter *first,
                                             GtkTextIter *second);
*/

void
Init_gtk_textiter(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_ITER, "TextIter", mGtk);
    rb_include_module(RG_TARGET_NAMESPACE, rb_mComparable);

    is_compat_240 = gtk_check_version(2, 4, 0) ? FALSE : TRUE;

    id_pixbuf = rb_intern("pixbuf");

    RG_DEF_METHOD(buffer, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "offset", get_offset, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "line", get_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "line_offset", get_line_offset, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "line_index", get_line_index, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "visible_line_offset", get_visible_line_offset, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "visible_line_index", get_visible_line_index, 0);
    RG_DEF_METHOD(char, 0);

    RG_DEF_METHOD(get_slice, 1);
    RG_DEF_METHOD(get_text, 1);
    RG_DEF_METHOD(get_visible_slice, 1);
    RG_DEF_METHOD(get_visible_text, 1);

    RG_DEF_METHOD(pixbuf, 0);
    RG_DEF_METHOD(marks, 0);
    RG_DEF_METHOD(child_anchor, 0);

    RG_DEF_METHOD(toggled_tags, 1);
    RG_DEF_METHOD_P(begins_tag, 1);
    RG_DEF_METHOD_P(ends_tag, 1);
    RG_DEF_METHOD_P(toggles_tag, 1);
    RG_DEF_METHOD_P(has_tag, 1);
    RG_DEF_METHOD(tags, 0);

    RG_DEF_METHOD_P(editable, 1);
    RG_DEF_METHOD_P(can_insert, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "starts_word?", starts_word, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ends_word?", ends_word, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "inside_word?", inside_word, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "starts_sentence?", starts_sentence, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ends_sentence?", ends_sentence, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "starts_line?", starts_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ends_line?", ends_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "cursor_position?", is_cursor_position, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "chars_in_line", get_chars_in_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "bytes_in_line", get_bytes_in_line, 0);

    RG_DEF_METHOD(attributes, 0);

    RG_DEF_METHOD(language, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "end?", is_end, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "start?", is_start, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "forward_char", forward_char, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_char", backward_char, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_chars", forward_chars, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_chars", backward_chars, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_line", forward_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_line", backward_line, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_lines", forward_lines, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_lines", backward_lines, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_word_end", forward_word_end, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_word_start", backward_word_start, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_word_ends", forward_word_ends, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_word_starts", backward_word_starts, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_sentence_end", forward_sentence_end, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_sentence_start", backward_sentence_start, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_sentence_ends", forward_sentence_ends, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_sentence_starts", backward_sentence_starts, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_word_ends", forward_visible_word_ends, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_visible_word_starts", backward_visible_word_starts, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_word_end", forward_visible_word_end, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backword_visible_word_start", backward_visible_word_start, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_cursor_position", forward_visible_cursor_position, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_visible_cursor_position", backward_visible_cursor_position, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_cursor_positions", forward_visible_cursor_positions, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_visible_cursor_positions", backward_visible_cursor_positions, 1); 
    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_line", forward_visible_line, 0); 
    rb_define_method(RG_TARGET_NAMESPACE, "backward_visible_line", backward_visible_line, 0); 
    rb_define_method(RG_TARGET_NAMESPACE, "forward_visible_lines", forward_visible_lines, 1); 
    rb_define_method(RG_TARGET_NAMESPACE, "backward_visible_lines", backward_visible_lines, 1); 
    rb_define_method(RG_TARGET_NAMESPACE, "forward_cursor_position", forward_cursor_position, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_cursor_position", backward_cursor_position, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_cursor_positions", forward_cursor_positions, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "backward_cursor_positions", backward_cursor_positions, 1);
    RG_DEF_METHOD(forward_to_end, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "forward_to_line_end", forward_to_line_end, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "set_offset", set_offset, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_line", set_line, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_line_offset", set_line_offset, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_line_index", set_line_index, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "set_visible_line_offset", set_visible_line_offset, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_visible_line_index", set_visible_line_index, 1);

    RG_DEF_METHOD(forward_to_tag_toggle, -1);
    RG_DEF_METHOD(backward_to_tag_toggle, -1);
    RG_DEF_METHOD(forward_find_char, -1);
    RG_DEF_METHOD(backward_find_char, -1);
    RG_DEF_METHOD(forward_search, -1);
    RG_DEF_METHOD(backward_search, -1);

    RG_DEF_METHOD_OPERATOR("==", equal, 1);
    RG_DEF_METHOD_OPERATOR("<=>", compare, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkTextSearchFlags */
    G_DEF_CLASS(GTK_TYPE_TEXT_SEARCH_FLAGS, "SearchFlags", RG_TARGET_NAMESPACE);
}
