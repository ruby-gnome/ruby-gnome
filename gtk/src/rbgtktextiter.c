/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktextiter.c -

  $Author: sakai $
  $Date: 2002/08/25 12:54:11 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

static VALUE cTextIter;

VALUE
make_gtktextiter(GtkTextIter* iter)
{
    return Data_Wrap_Struct(cTextIter, NULL, &gtk_text_iter_free, iter);
}

GtkTextIter*
get_gtktextiter(VALUE obj)
{
    GtkTextIter* iter;
    Data_Get_Struct(obj, GtkTextIter, iter);
    return iter;
}


// GtkTextIter *gtk_text_iter_copy     (const GtkTextIter *iter);

static VALUE
get_buffer(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_iter_get_buffer(get_gtktextiter(self)));
}

#define def_gint_getter(__name__) \
static VALUE \
get_##__name__(self) \
    VALUE self; \
{ \
    return INT2NUM(gtk_text_iter_get_##__name__(get_gtktextiter(self))); \
}

def_gint_getter(offset)
def_gint_getter(line)
def_gint_getter(line_offset)
def_gint_getter(line_index)
def_gint_getter(visible_line_offset)
def_gint_getter(visible_line_index)

static VALUE
get_char(self)
    VALUE self;
{
    return INT2NUM(gtk_text_iter_get_char(get_gtktextiter(self)));
}

static VALUE
get_slice(self, rhs)
    VALUE self, rhs;
{
    return rb_str_new2(gtk_text_iter_get_slice(get_gtktextiter(self),
                                               get_gtktextiter(rhs)));
}

static VALUE
get_text(self, rhs)
    VALUE self, rhs;
{
    return rb_str_new2(gtk_text_iter_get_slice(get_gtktextiter(self),
                                               get_gtktextiter(rhs)));
}


static VALUE
get_visible_slice(self, rhs)
    VALUE self, rhs;
{
    return rb_str_new2(gtk_text_iter_get_visible_slice(get_gtktextiter(self),
                                                       get_gtktextiter(rhs)));
}

static VALUE
get_visible_text(self, rhs)
    VALUE self, rhs;
{
    return rb_str_new2(gtk_text_iter_get_visible_slice(get_gtktextiter(self),
                                                       get_gtktextiter(rhs)));
}

static VALUE
get_pixbuf(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_iter_get_pixbuf(get_gtktextiter(self)));
}

static void
collect_marks(data, ary)
    gpointer data;
    VALUE ary;
{
    rb_ary_push(ary, GOBJ2RVAL(data));
}

static VALUE
get_marks(self)
    VALUE self;
{
    GSList* list = gtk_text_iter_get_marks(get_gtktextiter(self));
    VALUE result = rb_ary_new();
    g_slist_foreach(list, collect_marks, (gpointer)result);
    return result;
}


static VALUE
get_child_anchor(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_iter_get_child_anchor(get_gtktextiter(self)));
}

static void
collect_tags(data, user_data)
    gpointer data, user_data;
{
    VALUE ary = (VALUE)user_data;
    rb_ary_push(ary, GOBJ2RVAL(data));
}

static VALUE
get_toggled_tags(self, toggled_on)
    VALUE self, toggled_on;
{
    GSList* list = gtk_text_iter_get_toggled_tags(get_gtktextiter(self), RTEST(toggled_on));
    VALUE result = rb_ary_new();
    g_slist_foreach(list, &collect_tags, (gpointer)result);
    return result;
}

static VALUE
begins_tag(self, tag)
    VALUE self, tag;
{
    return gtk_text_iter_begins_tag(get_gtktextiter(self), GTK_TEXT_TAG(RVAL2GOBJ(tag))) ? Qtrue : Qfalse;
}

static VALUE
ends_tag(self, tag)
    VALUE self, tag;
{
    return gtk_text_iter_ends_tag(get_gtktextiter(self), GTK_TEXT_TAG(RVAL2GOBJ(tag))) ? Qtrue : Qfalse;
}

static VALUE
toggles_tag(self, tag)
    VALUE self, tag;
{
    return gtk_text_iter_toggles_tag(get_gtktextiter(self), GTK_TEXT_TAG(RVAL2GOBJ(tag))) ? Qtrue : Qfalse;
}

static VALUE
has_tag(self, tag)
    VALUE self, tag;
{
    return gtk_text_iter_toggles_tag(get_gtktextiter(self), GTK_TEXT_TAG(RVAL2GOBJ(tag))) ? Qtrue : Qfalse;
}

static VALUE
get_tags(self)
    VALUE self;
{
    GSList* list = gtk_text_iter_get_tags(get_gtktextiter(self));
    VALUE result = rb_ary_new();
    g_slist_foreach(list, &collect_tags, (gpointer)result);
    return result;
}

#if 0
gboolean gtk_text_iter_editable          (const GtkTextIter   *iter,
                                          gboolean             default_setting);
gboolean gtk_text_iter_can_insert        (const GtkTextIter   *iter,
                                          gboolean             default_editability);
#endif

#define def_predicate(__name__) \
static VALUE \
__name__(self) \
    VALUE self; \
{ \
    return gtk_text_iter_##__name__(get_gtktextiter(self)) ? Qtrue : Qfalse; \
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

#if 0
gboolean       gtk_text_iter_get_attributes (const GtkTextIter *iter,
					     GtkTextAttributes *values);
#endif

static VALUE
get_language(self)
    VALUE self;
{
    return rb_str_new2(pango_language_to_string(gtk_text_iter_get_language(get_gtktextiter(self))));
}

def_predicate(is_end)
def_predicate(is_start)


#define def_move(__name__) \
static VALUE \
__name__(self) \
    VALUE self; \
{ \
    return gtk_text_iter_##__name__(get_gtktextiter(self)) ? Qtrue : Qfalse; \
}

#define def_move_gint(__name__) \
static VALUE \
__name__(self, i) \
    VALUE self, i; \
{ \
    return gtk_text_iter_##__name__(get_gtktextiter(self), NUM2INT(i)) ? Qtrue : Qfalse; \
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
def_move(forward_cursor_position)
def_move(backward_cursor_position)
def_move_gint(forward_cursor_positions)
def_move_gint(backward_cursor_positions)
def_move(forward_to_line_end)

//def_move(forward_to_end)

#define def_gint_setter(__name__) \
static VALUE \
set_##__name__(self, val) \
    VALUE self, val; \
{ \
    gtk_text_iter_set_##__name__(get_gtktextiter(self), NUM2INT(val)); \
    return val; \
}

def_gint_setter(offset)
def_gint_setter(line)
def_gint_setter(line_offset)
def_gint_setter(line_index)

def_gint_setter(visible_line_offset)
def_gint_setter(visible_line_index)

#if 0
/* returns TRUE if a toggle was found; NULL for the tag pointer
 * means "any tag toggle", otherwise the next toggle of the
 * specified tag is located.
 */
gboolean gtk_text_iter_forward_to_tag_toggle (GtkTextIter *iter,
                                              GtkTextTag  *tag);

gboolean gtk_text_iter_backward_to_tag_toggle (GtkTextIter *iter,
                                               GtkTextTag  *tag);

typedef gboolean (* GtkTextCharPredicate) (gunichar ch, gpointer user_data);

gboolean gtk_text_iter_forward_find_char  (GtkTextIter          *iter,
                                           GtkTextCharPredicate  pred,
                                           gpointer              user_data,
                                           const GtkTextIter    *limit);
gboolean gtk_text_iter_backward_find_char (GtkTextIter          *iter,
                                           GtkTextCharPredicate  pred,
                                           gpointer              user_data,
                                           const GtkTextIter    *limit);

gboolean gtk_text_iter_forward_search  (const GtkTextIter *iter,
                                        const gchar       *str,
                                        GtkTextSearchFlags flags,
                                        GtkTextIter       *match_start,
                                        GtkTextIter       *match_end,
                                        const GtkTextIter *limit);

gboolean gtk_text_iter_backward_search (const GtkTextIter *iter,
                                        const gchar       *str,
                                        GtkTextSearchFlags flags,
                                        GtkTextIter       *match_start,
                                        GtkTextIter       *match_end,
                                        const GtkTextIter *limit);

#endif

#if 0
gboolean gtk_text_iter_equal           (const GtkTextIter *lhs,
                                        const GtkTextIter *rhs);
#endif

static VALUE
compare(self, rhs)
    VALUE self, rhs;
{
    return INT2NUM(gtk_text_iter_compare(get_gtktextiter(self), get_gtktextiter(rhs)));
}
    
#if 0
gboolean gtk_text_iter_in_range        (const GtkTextIter *iter,
                                        const GtkTextIter *start,
                                        const GtkTextIter *end);

/* Put these two in ascending order */
void     gtk_text_iter_order           (GtkTextIter *first,
                                        GtkTextIter *second);
#endif

void
Init_gtk_textiter()
{
    cTextIter = rb_define_class_under(mGtk, "TextIter", rb_cData);
    rb_include_module(cTextIter, rb_mComparable);

#if 0
typedef enum {
  GTK_TEXT_SEARCH_VISIBLE_ONLY,
  GTK_TEXT_SEARCH_TEXT_ONLY
  /* Possible future plans: SEARCH_CASE_INSENSITIVE, SEARCH_REGEXP */
} GtkTextSearchFlags;
#endif

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
    rb_define_method(cTextIter, "begins_tag", begins_tag, 1);
    rb_define_method(cTextIter, "ends_tag", ends_tag, 1);
    rb_define_method(cTextIter, "toggles_tag", toggles_tag, 1);
    rb_define_method(cTextIter, "has_tag?", has_tag, 1);
    rb_define_method(cTextIter, "tags", get_tags, 0);

    rb_define_method(cTextIter, "starts_word?", starts_word, 0);
    rb_define_method(cTextIter, "ends_word?", ends_word, 0);
    rb_define_method(cTextIter, "inside_word?", inside_word, 0);
    rb_define_method(cTextIter, "starts_sentence?", starts_sentence, 0);
    rb_define_method(cTextIter, "ends_sentence?", ends_sentence, 0);
    rb_define_method(cTextIter, "starts_line?", starts_line, 0);
    rb_define_method(cTextIter, "ends_line?", ends_line, 0);
    rb_define_method(cTextIter, "is_cursor_position?", is_cursor_position, 0);

    rb_define_method(cTextIter, "chars_in_line", get_chars_in_line, 0);
    rb_define_method(cTextIter, "bytes_in_line", get_bytes_in_line, 0);

    rb_define_method(cTextIter, "language", get_language, 0);
    rb_define_method(cTextIter, "is_end?", is_end, 0);
    rb_define_method(cTextIter, "is_start?", is_start, 0);

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
    rb_define_method(cTextIter, "forward_cursor_position", forward_cursor_position, 0);
    rb_define_method(cTextIter, "backward_cursor_position", backward_cursor_position, 0);
    rb_define_method(cTextIter, "forward_cursor_positions", forward_cursor_positions, 1);
    rb_define_method(cTextIter, "backward_cursor_positions", backward_cursor_positions, 1);
    //rb_define_method(cTextIter, "forward_to_end", forward_to_end, 0);
    rb_define_method(cTextIter, "forward_to_line_end", forward_to_line_end, 0);


    rb_define_method(cTextIter, "offset=", set_offset, 1);
    rb_define_method(cTextIter, "line=", set_line, 1);
    rb_define_method(cTextIter, "line_offset=", set_line_offset, 1);
    rb_define_method(cTextIter, "line_index=", set_line_index, 1);

    rb_define_method(cTextIter, "visible_line_offset=", set_visible_line_offset, 1);
    rb_define_method(cTextIter, "visible_line_index=", set_visible_line_index, 1);

    rb_define_method(cTextIter, "<=>", compare, 1);
}
