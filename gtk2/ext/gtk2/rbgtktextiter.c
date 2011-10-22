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

#define _SELF(s) ((GtkTextIter*)RVAL2BOXED(s, GTK_TYPE_TEXT_ITER))
#define RVAL2TAG(t) (GTK_TEXT_TAG(RVAL2GOBJ(t)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))

static gboolean is_compat_240;
static ID id_pixbuf;

static VALUE
get_buffer(VALUE self)
{
    return GOBJ2RVAL(gtk_text_iter_get_buffer(_SELF(self)));
}

#define def_gint_getter(__name__) \
static VALUE \
get_##__name__(VALUE self) \
{ \
    return INT2NUM(gtk_text_iter_get_##__name__(_SELF(self))); \
}

def_gint_getter(offset)
def_gint_getter(line)
def_gint_getter(line_offset)
def_gint_getter(line_index)
def_gint_getter(visible_line_offset)
def_gint_getter(visible_line_index)

static VALUE
get_char(VALUE self)
{
    gchar buf[10];
    gint len = g_unichar_to_utf8(gtk_text_iter_get_char(_SELF(self)), buf);
    buf[len] = '\0';
    return CSTR2RVAL(buf);
}

static VALUE
get_slice(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_slice(_SELF(self), _SELF(rhs)));
}

static VALUE
get_text(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_text(_SELF(self), _SELF(rhs)));
}

static VALUE
get_visible_slice(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_visible_slice(_SELF(self), _SELF(rhs)));
}

static VALUE
get_visible_text(VALUE self, VALUE rhs)
{
    return CSTR2RVAL(gtk_text_iter_get_visible_text(_SELF(self), _SELF(rhs)));
}

static VALUE
get_pixbuf(VALUE self)
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
get_marks(VALUE self)
{
    return GSLIST2ARYF(gtk_text_iter_get_marks(_SELF(self)));
}

static VALUE
get_toggled_tags(VALUE self, VALUE toggled_on)
{
    return GSLIST2ARYF(gtk_text_iter_get_toggled_tags(_SELF(self), RVAL2CBOOL(toggled_on)));
}

static VALUE
get_child_anchor(VALUE self)
{
    GtkTextChildAnchor* anchor = gtk_text_iter_get_child_anchor(_SELF(self));
    return anchor ? GOBJ2RVAL(anchor) : Qnil;
}

static VALUE
begins_tag(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_begins_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
ends_tag(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_ends_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
toggles_tag(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_toggles_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
has_tag(VALUE self, VALUE tag)
{
    return CBOOL2RVAL(gtk_text_iter_has_tag(_SELF(self), RVAL2TAG(tag)));
}

static VALUE
get_tags(VALUE self)
{
    return GSLIST2ARYF(gtk_text_iter_get_tags(_SELF(self)));
}

static VALUE
editable(VALUE self, VALUE default_setting)
{
    return CBOOL2RVAL(gtk_text_iter_editable(_SELF(self), RVAL2CBOOL(default_setting)));
}

static VALUE
can_insert(VALUE self, VALUE default_setting)
{
    return CBOOL2RVAL(gtk_text_iter_can_insert(_SELF(self), RVAL2CBOOL(default_setting)));
}

    
#define def_predicate(__name__) \
static VALUE \
__name__(VALUE self) \
{ \
    return CBOOL2RVAL(gtk_text_iter_##__name__(_SELF(self)));   \
}

def_predicate(starts_word)
def_predicate(ends_word)
def_predicate(inside_word)
def_predicate(starts_sentence)
def_predicate(ends_sentence)
def_predicate(starts_line)
def_predicate(ends_line)
def_predicate(is_cursor_position)

def_gint_getter(chars_in_line)
def_gint_getter(bytes_in_line)

static VALUE
get_attributes(VALUE self)
{
    GtkTextAttributes attr;
    
    if(gtk_text_iter_get_attributes(_SELF(self), &attr) == TRUE){
        return BOXED2RVAL(&attr, GTK_TYPE_TEXT_ATTRIBUTES);
    } else {
        return Qnil;
    }
}
    
static VALUE
get_language(VALUE self)
{
    return CSTR2RVAL(pango_language_to_string(gtk_text_iter_get_language(_SELF(self))));
}

def_predicate(is_end)
def_predicate(is_start)


#define def_move(__name__) \
static VALUE \
__name__(VALUE self) \
{ \
    return CBOOL2RVAL(gtk_text_iter_##__name__(_SELF(self))); \
}

#define def_move_gint(__name__) \
static VALUE \
__name__(VALUE self, VALUE i) \
{ \
    return CBOOL2RVAL(gtk_text_iter_##__name__(_SELF(self), NUM2INT(i))); \
}

def_move(forward_char)
def_move(backward_char)
def_move_gint(forward_chars)
def_move_gint(backward_chars)
def_move(forward_line)
def_move(backward_line)
def_move_gint(forward_lines)
def_move_gint(backward_lines)
def_move(forward_word_end)
def_move(backward_word_start)
def_move_gint(forward_word_ends)
def_move_gint(backward_word_starts)
def_move(forward_sentence_end)
def_move(backward_sentence_start)
def_move_gint(forward_sentence_ends)
def_move_gint(backward_sentence_starts)

#if GTK_CHECK_VERSION(2,4,0)
def_move_gint(forward_visible_word_ends)
def_move_gint(backward_visible_word_starts)
def_move(forward_visible_word_end)
def_move(backward_visible_word_start)
def_move(forward_visible_cursor_position)
def_move(backward_visible_cursor_position)
def_move_gint(forward_visible_cursor_positions)
def_move_gint(backward_visible_cursor_positions)
#endif
#if GTK_CHECK_VERSION(2,8,0)
def_move(forward_visible_line)
def_move(backward_visible_line)
def_move_gint(forward_visible_lines)
def_move_gint(backward_visible_lines)
#endif
def_move(forward_cursor_position)
def_move(backward_cursor_position)
def_move_gint(forward_cursor_positions)
def_move_gint(backward_cursor_positions)
def_move(forward_to_line_end)

static VALUE
forward_to_end(VALUE self)
{
    gtk_text_iter_forward_to_end(_SELF(self));
    return self;
}

#define def_gint_setter(__name__) \
static VALUE \
set_##__name__(VALUE self, VALUE val) \
{ \
    gtk_text_iter_set_##__name__(_SELF(self), NUM2INT(val)); \
    return val; \
}

def_gint_setter(offset)
def_gint_setter(line)
def_gint_setter(line_offset)
def_gint_setter(line_index)

def_gint_setter(visible_line_offset)
def_gint_setter(visible_line_index)

static VALUE
forward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
{
    VALUE tag;

    rb_scan_args(argc, argv, "01", &tag);
    return CBOOL2RVAL(gtk_text_iter_forward_to_tag_toggle(_SELF(self),
                                                          NIL_P(tag) ? NULL : RVAL2TAG(tag)));
}

static VALUE
backward_to_tag_toggle(int argc, VALUE *argv, VALUE self)
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
forward_find_char(int argc, VALUE *argv, VALUE self)
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
backward_find_char(int argc, VALUE *argv, VALUE self)
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
forward_search(int argc, VALUE *argv, VALUE self)
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
backward_search(int argc, VALUE *argv, VALUE self)
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
equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gtk_text_iter_equal(_SELF(self), _SELF(other)));
}

static VALUE
compare(VALUE self, VALUE rhs)
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
Init_gtk_textiter(void)
{
    VALUE cTextIter = G_DEF_CLASS(GTK_TYPE_TEXT_ITER, "TextIter", mGtk);
    rb_include_module(cTextIter, rb_mComparable);

    is_compat_240 = gtk_check_version(2, 4, 0) ? FALSE : TRUE;

    id_pixbuf = rb_intern("pixbuf");

    rb_define_method(cTextIter, "buffer", get_buffer, 0);
    rb_define_method(cTextIter, "offset", get_offset, 0);
    rb_define_method(cTextIter, "line", get_line, 0);
    rb_define_method(cTextIter, "line_offset", get_line_offset, 0);
    rb_define_method(cTextIter, "line_index", get_line_index, 0);
    rb_define_method(cTextIter, "visible_line_offset", get_visible_line_offset, 0);
    rb_define_method(cTextIter, "visible_line_index", get_visible_line_index, 0);
    rb_define_method(cTextIter, "char", get_char, 0);

    rb_define_method(cTextIter, "get_slice", get_slice, 1);
    rb_define_method(cTextIter, "get_text", get_text, 1);
    rb_define_method(cTextIter, "get_visible_slice", get_visible_slice, 1);
    rb_define_method(cTextIter, "get_visible_text", get_visible_text, 1);

    rb_define_method(cTextIter, "pixbuf", get_pixbuf, 0);
    rb_define_method(cTextIter, "marks", get_marks, 0);
    rb_define_method(cTextIter, "child_anchor", get_child_anchor, 0);

    rb_define_method(cTextIter, "toggled_tags", get_toggled_tags, 1);
    rb_define_method(cTextIter, "begins_tag?", begins_tag, 1);
    rb_define_method(cTextIter, "ends_tag?", ends_tag, 1);
    rb_define_method(cTextIter, "toggles_tag?", toggles_tag, 1);
    rb_define_method(cTextIter, "has_tag?", has_tag, 1);
    rb_define_method(cTextIter, "tags", get_tags, 0);

    rb_define_method(cTextIter, "editable?", editable, 1);
    rb_define_method(cTextIter, "can_insert?", can_insert, 1);

    rb_define_method(cTextIter, "starts_word?", starts_word, 0);
    rb_define_method(cTextIter, "ends_word?", ends_word, 0);
    rb_define_method(cTextIter, "inside_word?", inside_word, 0);
    rb_define_method(cTextIter, "starts_sentence?", starts_sentence, 0);
    rb_define_method(cTextIter, "ends_sentence?", ends_sentence, 0);
    rb_define_method(cTextIter, "starts_line?", starts_line, 0);
    rb_define_method(cTextIter, "ends_line?", ends_line, 0);
    rb_define_method(cTextIter, "cursor_position?", is_cursor_position, 0);

    rb_define_method(cTextIter, "chars_in_line", get_chars_in_line, 0);
    rb_define_method(cTextIter, "bytes_in_line", get_bytes_in_line, 0);

    rb_define_method(cTextIter, "attributes", get_attributes, 0);

    rb_define_method(cTextIter, "language", get_language, 0);
    rb_define_method(cTextIter, "end?", is_end, 0);
    rb_define_method(cTextIter, "start?", is_start, 0);

    rb_define_method(cTextIter, "forward_char", forward_char, 0);
    rb_define_method(cTextIter, "backward_char", backward_char, 0);
    rb_define_method(cTextIter, "forward_chars", forward_chars, 1);
    rb_define_method(cTextIter, "backward_chars", backward_chars, 1);
    rb_define_method(cTextIter, "forward_line", forward_line, 0);
    rb_define_method(cTextIter, "backward_line", backward_line, 0);
    rb_define_method(cTextIter, "forward_lines", forward_lines, 1);
    rb_define_method(cTextIter, "backward_lines", backward_lines, 1);
    rb_define_method(cTextIter, "forward_word_end", forward_word_end, 0);
    rb_define_method(cTextIter, "backward_word_start", backward_word_start, 0);
    rb_define_method(cTextIter, "forward_word_ends", forward_word_ends, 1);
    rb_define_method(cTextIter, "backward_word_starts", backward_word_starts, 1);
    rb_define_method(cTextIter, "forward_sentence_end", forward_sentence_end, 0);
    rb_define_method(cTextIter, "backward_sentence_start", backward_sentence_start, 0);
    rb_define_method(cTextIter, "forward_sentence_ends", forward_sentence_ends, 1);
    rb_define_method(cTextIter, "backward_sentence_starts", backward_sentence_starts, 1);

#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(cTextIter, "forward_visible_word_ends", forward_visible_word_ends, 1);
    rb_define_method(cTextIter, "backward_visible_word_starts", backward_visible_word_starts, 1);
    rb_define_method(cTextIter, "forward_visible_word_end", forward_visible_word_end, 0);
    rb_define_method(cTextIter, "backword_visible_word_start", backward_visible_word_start, 0);
    rb_define_method(cTextIter, "forward_visible_cursor_position", forward_visible_cursor_position, 0);
    rb_define_method(cTextIter, "backward_visible_cursor_position", backward_visible_cursor_position, 0);
    rb_define_method(cTextIter, "forward_visible_cursor_positions", forward_visible_cursor_positions, 1);
    rb_define_method(cTextIter, "backward_visible_cursor_positions", backward_visible_cursor_positions, 1); 
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(cTextIter, "forward_visible_line", forward_visible_line, 0); 
    rb_define_method(cTextIter, "backward_visible_line", backward_visible_line, 0); 
    rb_define_method(cTextIter, "forward_visible_lines", forward_visible_lines, 1); 
    rb_define_method(cTextIter, "backward_visible_lines", backward_visible_lines, 1); 
#endif
    rb_define_method(cTextIter, "forward_cursor_position", forward_cursor_position, 0);
    rb_define_method(cTextIter, "backward_cursor_position", backward_cursor_position, 0);
    rb_define_method(cTextIter, "forward_cursor_positions", forward_cursor_positions, 1);
    rb_define_method(cTextIter, "backward_cursor_positions", backward_cursor_positions, 1);
    rb_define_method(cTextIter, "forward_to_end", forward_to_end, 0);
    rb_define_method(cTextIter, "forward_to_line_end", forward_to_line_end, 0);


    rb_define_method(cTextIter, "set_offset", set_offset, 1);
    rb_define_method(cTextIter, "set_line", set_line, 1);
    rb_define_method(cTextIter, "set_line_offset", set_line_offset, 1);
    rb_define_method(cTextIter, "set_line_index", set_line_index, 1);

    rb_define_method(cTextIter, "set_visible_line_offset", set_visible_line_offset, 1);
    rb_define_method(cTextIter, "set_visible_line_index", set_visible_line_index, 1);

    rb_define_method(cTextIter, "forward_to_tag_toggle", forward_to_tag_toggle, -1);
    rb_define_method(cTextIter, "backward_to_tag_toggle", backward_to_tag_toggle, -1);
    rb_define_method(cTextIter, "forward_find_char", forward_find_char, -1);
    rb_define_method(cTextIter, "backward_find_char", backward_find_char, -1);
    rb_define_method(cTextIter, "forward_search", forward_search, -1);
    rb_define_method(cTextIter, "backward_search", backward_search, -1);

    rb_define_method(cTextIter, "==", equal, 1);
    rb_define_method(cTextIter, "<=>", compare, 1);

    G_DEF_SETTERS(cTextIter);

    /* GtkTextSearchFlags */
    G_DEF_CLASS(GTK_TYPE_TEXT_SEARCH_FLAGS, "SearchFlags", cTextIter);
    G_DEF_CONSTANTS(cTextIter, GTK_TYPE_TEXT_SEARCH_FLAGS, "GTK_TEXT_");
}
