/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextbuffer.c -

  $Author: mutoh $
  $Date: 2002/11/13 13:39:28 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TEXT_BUFFER(RVAL2GOBJ(s)))
#define RVAL2ITR(i) (GtkTextIter*)RVAL2BOXED(i, GTK_TYPE_TEXT_ITER)
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))
#define RVAL2MARK(m) (GTK_TEXT_MARK(RVAL2GOBJ(m)))
#define N_RVAL2CSTR(text) (NIL_P(text) ? NULL : RVAL2CSTR(text))
#define RVAL2TAG(t) (GTK_TEXT_TAG(RVAL2GOBJ(t)))

static VALUE
txt_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE table;
    rb_scan_args(argc, argv, "01", &table);
    G_INITIALIZE(
        self, gtk_text_buffer_new(NIL_P(table) ? NULL : GTK_TEXT_TAG_TABLE(RVAL2GOBJ(table))));
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
    gtk_text_buffer_set_text(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

static VALUE
txt_insert(self, iter, text)
    VALUE self, iter, text;
{
    gtk_text_buffer_insert(_SELF(self), RVAL2ITR(iter), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

static VALUE
txt_insert_at_cursor(self, text)
{
    gtk_text_buffer_insert_at_cursor(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len);
    return self;
}

#if 0
gboolean gtk_text_buffer_insert_interactive           (GtkTextBuffer *buffer,
                                                       GtkTextIter   *iter,
                                                       const gchar   *text,
                                                       gint           len,
                                                       gboolean       default_editable);
gboolean gtk_text_buffer_insert_interactive_at_cursor (GtkTextBuffer *buffer,
                                                       const gchar   *text,
                                                       gint           len,
                                                       gboolean       default_editable);

#endif

#if 0

void     gtk_text_buffer_insert_range             (GtkTextBuffer     *buffer,
                                                   GtkTextIter       *iter,
                                                   const GtkTextIter *start,
                                                   const GtkTextIter *end);
gboolean gtk_text_buffer_insert_range_interactive (GtkTextBuffer     *buffer,
                                                   GtkTextIter       *iter,
                                                   const GtkTextIter *start,
                                                   const GtkTextIter *end,
                                                   gboolean           default_editable);

void    gtk_text_buffer_insert_with_tags          (GtkTextBuffer     *buffer,
                                                   GtkTextIter       *iter,
                                                   const gchar       *text,
                                                   gint               len,
                                                   GtkTextTag        *first_tag,
                                                   ...);

void    gtk_text_buffer_insert_with_tags_by_name  (GtkTextBuffer     *buffer,
                                                   GtkTextIter       *iter,
                                                   const gchar       *text,
                                                   gint               len,
                                                   const gchar       *first_tag_name,
                                                   ...);
#endif

static VALUE
txt_delete(self, start, end)
    VALUE self, start, end;
{
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(RVAL2GOBJ(self)),
                           RVAL2ITR(start), RVAL2ITR(end));
    return self;
}

#if 0
gboolean gtk_text_buffer_delete_interactive (GtkTextBuffer *buffer,
                                             GtkTextIter   *start_iter,
                                             GtkTextIter   *end_iter,
                                             gboolean       default_editable);



/* Obtain strings from the buffer */
gchar          *gtk_text_buffer_get_text            (GtkTextBuffer     *buffer,
                                                     const GtkTextIter *start,
                                                     const GtkTextIter *end,
                                                     gboolean           include_hidden_chars);

gchar          *gtk_text_buffer_get_slice           (GtkTextBuffer     *buffer,
                                                     const GtkTextIter *start,
                                                     const GtkTextIter *end,
                                                     gboolean           include_hidden_chars);

/* Insert a pixbuf */
void gtk_text_buffer_insert_pixbuf         (GtkTextBuffer *buffer,
                                            GtkTextIter   *iter,
                                            GdkPixbuf     *pixbuf);

/* Insert a child anchor */
void               gtk_text_buffer_insert_child_anchor (GtkTextBuffer      *buffer,
                                                        GtkTextIter        *iter,
                                                        GtkTextChildAnchor *anchor);

/* Convenience, create and insert a child anchor */
GtkTextChildAnchor *gtk_text_buffer_create_child_anchor (GtkTextBuffer *buffer,
                                                         GtkTextIter   *iter);

/* Mark manipulation */
GtkTextMark   *gtk_text_buffer_create_mark (GtkTextBuffer     *buffer,
                                            const gchar       *mark_name,
                                            const GtkTextIter *where,
                                            gboolean           left_gravity);
void           gtk_text_buffer_move_mark   (GtkTextBuffer     *buffer,
                                            GtkTextMark       *mark,
                                            const GtkTextIter *where);
void           gtk_text_buffer_delete_mark (GtkTextBuffer     *buffer,
                                            GtkTextMark       *mark);
GtkTextMark*   gtk_text_buffer_get_mark    (GtkTextBuffer     *buffer,
                                            const gchar       *name);

void gtk_text_buffer_move_mark_by_name   (GtkTextBuffer     *buffer,
                                          const gchar       *name,
                                          const GtkTextIter *where);
void gtk_text_buffer_delete_mark_by_name (GtkTextBuffer     *buffer,
                                          const gchar       *name);
#endif
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

#if 0
/* efficiently move insert and selection_bound to same location */
void gtk_text_buffer_place_cursor (GtkTextBuffer     *buffer,
                                   const GtkTextIter *where);
#endif


#if 0
/* Tag manipulation */
void gtk_text_buffer_apply_tag             (GtkTextBuffer     *buffer,
                                            GtkTextTag        *tag,
                                            const GtkTextIter *start,
                                            const GtkTextIter *end);
void gtk_text_buffer_remove_tag            (GtkTextBuffer     *buffer,
                                            GtkTextTag        *tag,
                                            const GtkTextIter *start,
                                            const GtkTextIter *end);
void gtk_text_buffer_apply_tag_by_name     (GtkTextBuffer     *buffer,
                                            const gchar       *name,
                                            const GtkTextIter *start,
                                            const GtkTextIter *end);
void gtk_text_buffer_remove_tag_by_name    (GtkTextBuffer     *buffer,
                                            const gchar       *name,
                                            const GtkTextIter *start,
                                            const GtkTextIter *end);
void gtk_text_buffer_remove_all_tags       (GtkTextBuffer     *buffer,
                                            const GtkTextIter *start,
                                            const GtkTextIter *end);

/* You can either ignore the return value, or use it to
 * set the attributes of the tag. tag_name can be NULL
 */
GtkTextTag    *gtk_text_buffer_create_tag (GtkTextBuffer *buffer,
                                           const gchar   *tag_name,
                                           const gchar   *first_property_name,
                                           ...);

/* Obtain iterators pointed at various places, then you can move the
 * iterator around using the GtkTextIter operators
 */
void gtk_text_buffer_get_iter_at_line_offset (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter,
                                              gint           line_number,
                                              gint           char_offset);
void gtk_text_buffer_get_iter_at_line_index  (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter,
                                              gint           line_number,
                                              gint           byte_index);
void gtk_text_buffer_get_iter_at_offset      (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter,
                                              gint           char_offset);
void gtk_text_buffer_get_iter_at_line        (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter,
                                              gint           line_number);
void gtk_text_buffer_get_start_iter          (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter);
void gtk_text_buffer_get_end_iter            (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter);
void gtk_text_buffer_get_bounds              (GtkTextBuffer *buffer,
                                              GtkTextIter   *start,
                                              GtkTextIter   *end);
void gtk_text_buffer_get_iter_at_mark        (GtkTextBuffer *buffer,
                                              GtkTextIter   *iter,
                                              GtkTextMark   *mark);

void gtk_text_buffer_get_iter_at_child_anchor (GtkTextBuffer      *buffer,
                                               GtkTextIter        *iter,
                                               GtkTextChildAnchor *anchor);
#endif

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

#if 0
void gtk_text_buffer_add_selection_clipboard    (GtkTextBuffer     *buffer,
						 GtkClipboard      *clipboard);
void gtk_text_buffer_remove_selection_clipboard (GtkTextBuffer     *buffer,
						 GtkClipboard      *clipboard);

void            gtk_text_buffer_cut_clipboard           (GtkTextBuffer *buffer,
							 GtkClipboard  *clipboard,
                                                         gboolean       default_editable);
void            gtk_text_buffer_copy_clipboard          (GtkTextBuffer *buffer,
							 GtkClipboard  *clipboard);
void            gtk_text_buffer_paste_clipboard         (GtkTextBuffer *buffer,
							 GtkClipboard  *clipboard,
							 GtkTextIter   *override_location,
                                                         gboolean       default_editable);
#endif

#if 0
gboolean        gtk_text_buffer_get_selection_bounds    (GtkTextBuffer *buffer,
                                                         GtkTextIter   *start,
                                                         GtkTextIter   *end);
#endif

static VALUE
txt_delete_selection(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE interactive, default_editable;
    rb_scan_args(argc, argv, "20", &interactive, &default_editable); /* FIXME: allow ommiting */
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
txt_get_end_iter(self)
     VALUE self;
{
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(_SELF(self), &iter);
  return ITR2RVAL(&iter);
}

static VALUE
txt_move_mark_by_name(self, name, where)
     VALUE self, name, where;
{
  gtk_text_buffer_move_mark_by_name(_SELF(self), RVAL2CSTR(name), RVAL2ITR(where));
  return self;
}

static VALUE
txt_move_mark(self, mark, where)
     VALUE self, mark, where;
{
  gtk_text_buffer_move_mark(_SELF(self), RVAL2MARK(mark), RVAL2ITR(where));
  return self;
}

/*
static VALUE
txt_create_tag(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE tag_name, first_property;
  VALUE properties;
  char *p[10];
  struct RArray *prop_array;
  int i;

  rb_scan_args(argc, argv, "2*", &tag_name, &first_property, &properties);
  prop_array = RARRAY(properties);
  for(i=0; i<prop_array->len; i++)
    {
      p[i] = RVAL2CSTR(prop_array->ptr[i]);
    }
  p[prop_array->len] = NULL;
  
  return GOBJ2RVAL(gtk_text_buffer_create_tag(_SELF(self),
                                              RVAL2CSTR(tag_name),
                                              RVAL2CSTR(first_property),
                                              p[0],p[1],p[2],p[3],p[4],
                                              p[5],p[6],p[7],p[8],p[9]));
}
*/

static VALUE
txt_insert_with_tags(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE where, text, tags;
  struct RArray *tarray;
  gint start_offset;
  GtkTextIter start;
  int i;

  rb_scan_args(argc, argv, "2*", &where, &text, &tags);
  tarray = RARRAY(tags);

  start_offset = gtk_text_iter_get_offset(RVAL2ITR(where));
  gtk_text_buffer_insert(_SELF(self), RVAL2ITR(where), RVAL2CSTR(text), RSTRING(text)->len);

  if(tarray->len == 0)
    return self;

  gtk_text_buffer_get_iter_at_offset(_SELF(self), &start, start_offset);
  
  for(i=0; i<tarray->len; i++)
    {
      GtkTextTag *tag;
      tag = RVAL2GOBJ(tarray->ptr[i]);
      gtk_text_buffer_apply_tag(_SELF(self), tag, &start, RVAL2ITR(where));
    }
  
  return self;
}

static VALUE
txt_insert_with_tags_by_name(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE where, text, tags;
  struct RArray *tarray;
  gint start_offset;
  GtkTextIter start;
  int i;

  rb_scan_args(argc, argv, "2*", &where, &text, &tags);
  tarray = RARRAY(tags);

  start_offset = gtk_text_iter_get_offset(RVAL2ITR(where));
  gtk_text_buffer_insert(_SELF(self), RVAL2ITR(where), RVAL2CSTR(text), RSTRING(text)->len);

  if(tarray->len == 0)
    return self;

  gtk_text_buffer_get_iter_at_offset(_SELF(self), &start, start_offset);
  
  for(i=0; i<tarray->len; i++)
    {
      GtkTextTag *tag;
      tag = gtk_text_tag_table_lookup(_SELF(self)->tag_table, RVAL2CSTR(tarray->ptr[i]));
      if (tag == NULL)
        {
          g_warning ("%s: no tag with name '%s'!", G_STRLOC, RVAL2CSTR(tarray->ptr[i]));
          return self;
        }
      gtk_text_buffer_apply_tag (_SELF(self), tag, &start, RVAL2ITR(where));
    }

  return self;
}

void 
Init_gtk_textbuffer()
{
    VALUE gTextBuffer = G_DEF_CLASS(GTK_TYPE_TEXT_BUFFER, "TextBuffer", mGtk);

    rb_define_method(gTextBuffer, "initialize", txt_initialize, -1);
    rb_define_method(gTextBuffer, "line_count", txt_get_line_count, 0);
    rb_define_method(gTextBuffer, "char_count", txt_get_char_count, 0);
    rb_define_method(gTextBuffer, "tag_table", txt_get_tag_table, 0);

    rb_define_method(gTextBuffer, "set_text", txt_set_text, 1);
    rb_define_method(gTextBuffer, "insert", txt_insert, 2);
    rb_define_method(gTextBuffer, "insert_at_cursor", txt_insert_at_cursor, 1);

    rb_define_method(gTextBuffer, "delete", txt_delete, 2);
/* Comment out because this method's name is very bad.
   Use Gtk::TextBuffer#get_mark("insert") instead.
    rb_define_method(gTextBuffer, "get_insert", txt_get_insert, 0);
*/
    rb_define_method(gTextBuffer, "selection_bound", txt_get_selection_bound, 0);

    rb_define_method(gTextBuffer, "modified?", txt_get_modified, 0);
    rb_define_method(gTextBuffer, "set_modified", txt_set_modified, 1);

    rb_define_method(gTextBuffer, "delete_selection", txt_delete_selection, -1);

    rb_define_method(gTextBuffer, "begin_user_action", txt_begin_user_action, 0);
    rb_define_method(gTextBuffer, "end_user_action", txt_end_user_action, 0);

    rb_define_method(gTextBuffer, "end_iter", txt_get_end_iter, 0);
    rb_define_method(gTextBuffer, "move_mark_by_name", txt_move_mark_by_name, 2);
    rb_define_method(gTextBuffer, "move_mark", txt_move_mark, 2);
/*
    rb_define_method(gTextBuffer, "create_tag", txt_create_tag, -1);
*/
    rb_define_method(gTextBuffer, "insert_with_tags", txt_insert_with_tags, -1);
    rb_define_method(gTextBuffer, "insert_with_tags_by_name", txt_insert_with_tags_by_name, -1);

    G_DEF_SETTERS(gTextBuffer);
}

