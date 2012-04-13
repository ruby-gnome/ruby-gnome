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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cTextIter
#define _SELF(s) (RVAL2GTKTEXTITER(s))

static gboolean is_compat_240;

static VALUE
rg_buffer(VALUE self)
{
    return GOBJ2RVAL(gtk_text_iter_get_buffer(_SELF(self)));
}

static VALUE
rg_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_offset(_SELF(self)));
}

static VALUE
rg_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line(_SELF(self)));
}

static VALUE
rg_line_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line_offset(_SELF(self)));
}

static VALUE
rg_line_index(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_line_index(_SELF(self)));
}

static VALUE
rg_visible_line_offset(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_visible_line_offset(_SELF(self)));
}

static VALUE
rg_visible_line_index(VALUE self)
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
    VALUE pixbuf = GOBJ2RVAL(gtk_text_iter_get_pixbuf(_SELF(self)));
    G_CHILD_SET(self, rb_intern("pixbuf"), pixbuf);
    return pixbuf;
}

static VALUE
rg_marks(VALUE self)
{
    /* need free? */
    return GOBJGSLIST2RVAL_FREE(gtk_text_iter_get_marks(_SELF(self)),
                                g_slist_free, NULL);
}

static VALUE
rg_toggled_tags(VALUE self, VALUE toggled_on)
{
    /* need free? */
    return GOBJGSLIST2RVAL_FREE(gtk_text_iter_get_toggled_tags(_SELF(self),
                                                               RVAL2CBOOL(toggled_on)),
                                g_slist_free, NULL);
}

static VALUE
rg_child_anchor(VALUE self)
{
    return GOBJ2RVAL(gtk_text_iter_get_child_anchor(_SELF(self)));
}

static VALUE
rg_begins_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_begins_tag(_SELF(self), RVAL2GTKTEXTTAG(tag)));
}

static VALUE
rg_ends_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_ends_tag(_SELF(self), RVAL2GTKTEXTTAG(tag)));
}

static VALUE
rg_toggles_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_toggles_tag(_SELF(self), RVAL2GTKTEXTTAG(tag)));
}

static VALUE
rg_has_tag_p(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_has_tag(_SELF(self), RVAL2GTKTEXTTAG(tag)));
}

static VALUE
rg_tags(VALUE self)
{
    return GOBJGSLIST2RVAL_FREE(gtk_text_iter_get_tags(_SELF(self)),
                                g_slist_free, NULL);
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
rg_starts_word_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_word(_SELF(self)));
}

static VALUE
rg_ends_word_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_word(_SELF(self)));
}

static VALUE
rg_inside_word_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_inside_word(_SELF(self)));
}

static VALUE
rg_starts_sentence_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_sentence(_SELF(self)));
}

static VALUE
rg_ends_sentence_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_sentence(_SELF(self)));
}

static VALUE
rg_starts_line_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_starts_line(_SELF(self)));
}

static VALUE
rg_ends_line_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_ends_line(_SELF(self)));
}

static VALUE
rg_cursor_position_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_cursor_position(_SELF(self)));
}

static VALUE
rg_chars_in_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_chars_in_line(_SELF(self)));
}

static VALUE
rg_bytes_in_line(VALUE self)
{
    return INT2NUM(gtk_text_iter_get_bytes_in_line(_SELF(self)));
}

static VALUE
rg_attributes(VALUE self)
{
    GtkTextAttributes attr;

    if(gtk_text_iter_get_attributes(_SELF(self), &attr) == TRUE){
        return GTKTEXTATTRIBUTES2RVAL(&attr);
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
rg_end_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_end(_SELF(self)));
}

static VALUE
rg_start_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_is_start(_SELF(self)));
}

static VALUE
rg_forward_char(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_char(_SELF(self)));
}

static VALUE
rg_backward_char(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_char(_SELF(self)));
}

static VALUE
rg_forward_chars(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_chars(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_chars(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_chars(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_line(_SELF(self)));
}

static VALUE
rg_backward_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_line(_SELF(self)));
}

static VALUE
rg_forward_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_word_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_word_end(_SELF(self)));
}

static VALUE
rg_backward_word_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_word_start(_SELF(self)));
}

static VALUE
rg_forward_word_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_word_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_word_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_word_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_sentence_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_sentence_end(_SELF(self)));
}

static VALUE
rg_backward_sentence_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_sentence_start(_SELF(self)));
}

static VALUE
rg_forward_sentence_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_sentence_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_sentence_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_sentence_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_visible_word_ends(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_word_ends(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_visible_word_starts(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_word_starts(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_visible_word_end(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_word_end(_SELF(self)));
}

static VALUE
rg_backward_visible_word_start(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_word_start(_SELF(self)));
}

static VALUE
rg_forward_visible_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_cursor_position(_SELF(self)));
}

static VALUE
rg_backward_visible_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_cursor_position(_SELF(self)));
}

static VALUE
rg_forward_visible_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_visible_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_visible_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_line(_SELF(self)));
}

static VALUE
rg_backward_visible_line(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_line(_SELF(self)));
}

static VALUE
rg_forward_visible_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_visible_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_visible_lines(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_visible_lines(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_forward_cursor_position(_SELF(self)));
}

static VALUE
rg_backward_cursor_position(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_backward_cursor_position(_SELF(self)));
}

static VALUE
rg_forward_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_forward_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_backward_cursor_positions(VALUE self, VALUE i)
{
    return CBOOL2RVAL(gtk_text_iter_backward_cursor_positions(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_forward_to_line_end(VALUE self)
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
rg_set_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_offset(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_set_line(VALUE self, VALUE val)
{
    gtk_text_iter_set_line(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_set_line_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_line_offset(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_set_line_index(VALUE self, VALUE val)
{
    gtk_text_iter_set_line_index(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_set_visible_line_offset(VALUE self, VALUE val)
{
    gtk_text_iter_set_visible_line_offset(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_set_visible_line_index(VALUE self, VALUE val)
{
    gtk_text_iter_set_visible_line_index(_SELF(self), NUM2INT(val));
    return self;
}

static VALUE
rg_forward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
{
    VALUE tag;

    rb_scan_args(argc, argv, "01", &tag);
    return CBOOL2RVAL(gtk_text_iter_forward_to_tag_toggle(_SELF(self),
                                                          NIL_P(tag) ? NULL : RVAL2GTKTEXTTAG(tag)));
}

static VALUE
rg_backward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
{
    VALUE tag;

    rb_scan_args(argc, argv, "01", &tag);
    return CBOOL2RVAL(gtk_text_iter_backward_to_tag_toggle(_SELF(self),
                                                           NIL_P(tag) ? NULL : RVAL2GTKTEXTTAG(tag)));
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
                                           RVAL2GTKTEXTSEARCHFLAGS(flags), 
                                           &m_start, &m_end,
                                           NIL_P(limit) ? NULL : _SELF(limit));
    } else {
        ret = gtk_text_iter_forward_search(_SELF(self), RVAL2CSTR(str),
                                           RVAL2GTKTEXTSEARCHFLAGS(flags), 
                                           &m_start, &m_end,
                                           NIL_P(limit) ? NULL : _SELF(limit));
    }
    return ret ? rb_ary_new3(2, GTKTEXTITER2RVAL(&m_start), GTKTEXTITER2RVAL(&m_end)) : Qnil;
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
                                            RVAL2GTKTEXTSEARCHFLAGS(flags), 
                                            &m_start, &m_end,
                                            NIL_P(limit) ? NULL : _SELF(limit));
    } else {
        ret = gtk_text_iter_backward_search(_SELF(self), RVAL2CSTR(str),
                                            RVAL2GTKTEXTSEARCHFLAGS(flags), 
                                            &m_start, &m_end,
                                            NIL_P(limit) ? NULL : _SELF(limit));
    }
    return ret ? rb_ary_new3(2, GTKTEXTITER2RVAL(&m_start), GTKTEXTITER2RVAL(&m_end)) : Qnil;
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

static VALUE
rg_inside_sentence(VALUE self)
{
    return CBOOL2RVAL(gtk_text_iter_inside_sentence(_SELF(self)));
}

void
Init_gtk_textiter(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_ITER, "TextIter", mGtk);
    rb_include_module(RG_TARGET_NAMESPACE, rb_mComparable);

    is_compat_240 = gtk_check_version(2, 4, 0) ? FALSE : TRUE;

    RG_DEF_METHOD(buffer, 0);
    RG_DEF_METHOD(offset, 0);
    RG_DEF_METHOD(line, 0);
    RG_DEF_METHOD(line_offset, 0);
    RG_DEF_METHOD(line_index, 0);
    RG_DEF_METHOD(visible_line_offset, 0);
    RG_DEF_METHOD(visible_line_index, 0);
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

    RG_DEF_METHOD_P(starts_word, 0);
    RG_DEF_METHOD_P(ends_word, 0);
    RG_DEF_METHOD_P(inside_word, 0);
    RG_DEF_METHOD_P(starts_sentence, 0);
    RG_DEF_METHOD_P(ends_sentence, 0);
    RG_DEF_METHOD_P(starts_line, 0);
    RG_DEF_METHOD_P(ends_line, 0);
    RG_DEF_METHOD_P(cursor_position, 0);

    RG_DEF_METHOD(chars_in_line, 0);
    RG_DEF_METHOD(bytes_in_line, 0);

    RG_DEF_METHOD(attributes, 0);

    RG_DEF_METHOD(language, 0);
    RG_DEF_METHOD_P(end, 0);
    RG_DEF_METHOD_P(start, 0);

    RG_DEF_METHOD(forward_char, 0);
    RG_DEF_METHOD(backward_char, 0);
    RG_DEF_METHOD(forward_chars, 1);
    RG_DEF_METHOD(backward_chars, 1);
    RG_DEF_METHOD(forward_line, 0);
    RG_DEF_METHOD(backward_line, 0);
    RG_DEF_METHOD(forward_lines, 1);
    RG_DEF_METHOD(backward_lines, 1);
    RG_DEF_METHOD(forward_word_end, 0);
    RG_DEF_METHOD(backward_word_start, 0);
    RG_DEF_METHOD(forward_word_ends, 1);
    RG_DEF_METHOD(backward_word_starts, 1);
    RG_DEF_METHOD(forward_sentence_end, 0);
    RG_DEF_METHOD(backward_sentence_start, 0);
    RG_DEF_METHOD(forward_sentence_ends, 1);
    RG_DEF_METHOD(backward_sentence_starts, 1);

    RG_DEF_METHOD(forward_visible_word_ends, 1);
    RG_DEF_METHOD(backward_visible_word_starts, 1);
    RG_DEF_METHOD(forward_visible_word_end, 0);
    RG_DEF_METHOD(backward_visible_word_start, 0);
    RG_DEF_METHOD(forward_visible_cursor_position, 0);
    RG_DEF_METHOD(backward_visible_cursor_position, 0);
    RG_DEF_METHOD(forward_visible_cursor_positions, 1);
    RG_DEF_METHOD(backward_visible_cursor_positions, 1); 
    RG_DEF_METHOD(forward_visible_line, 0); 
    RG_DEF_METHOD(backward_visible_line, 0); 
    RG_DEF_METHOD(forward_visible_lines, 1); 
    RG_DEF_METHOD(backward_visible_lines, 1); 
    RG_DEF_METHOD(forward_cursor_position, 0);
    RG_DEF_METHOD(backward_cursor_position, 0);
    RG_DEF_METHOD(forward_cursor_positions, 1);
    RG_DEF_METHOD(backward_cursor_positions, 1);
    RG_DEF_METHOD(forward_to_end, 0);
    RG_DEF_METHOD(forward_to_line_end, 0);

    RG_DEF_METHOD(set_offset, 1);
    RG_DEF_METHOD(set_line, 1);
    RG_DEF_METHOD(set_line_offset, 1);
    RG_DEF_METHOD(set_line_index, 1);

    RG_DEF_METHOD(set_visible_line_offset, 1);
    RG_DEF_METHOD(set_visible_line_index, 1);

    RG_DEF_METHOD(forward_to_tag_toggle, -1);
    RG_DEF_METHOD(backward_to_tag_toggle, -1);
    RG_DEF_METHOD(forward_find_char, -1);
    RG_DEF_METHOD(backward_find_char, -1);
    RG_DEF_METHOD(forward_search, -1);
    RG_DEF_METHOD(backward_search, -1);

    RG_DEF_METHOD_OPERATOR("==", equal, 1);
    RG_DEF_METHOD_OPERATOR("<=>", compare, 1);
    RG_DEF_METHOD(inside_sentence, 0);

    G_DEF_CLASS(GTK_TYPE_TEXT_SEARCH_FLAGS, "SearchFlags", RG_TARGET_NAMESPACE);
}
