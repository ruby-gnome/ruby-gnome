/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktextbuffer.c -

  $Author: sakai $
  $Date: 2002/08/25 12:54:11 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

extern VALUE make_gtktextiter(GtkTextIter* iter);
extern GtkTextIter* get_gtktextiter(VALUE obj);

static VALUE
txt_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE table;
    rb_scan_args(argc, argv, "01", &table);
    RBGOBJ_INITIALIZE(
        self, gtk_text_buffer_new(NIL_P(table) ? NULL : GTK_TEXT_TAG_TABLE(RVAL2GOBJ(table))));
    return Qnil;
}

static VALUE
txt_get_line_count(self)
    VALUE self;
{
    return INT2NUM(gtk_text_buffer_get_line_count(GTK_TEXT_BUFFER(RVAL2GOBJ(self))));
}

static VALUE
txt_get_char_count(self)
    VALUE self;
{
    return INT2NUM(gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(RVAL2GOBJ(self))));
}

static VALUE
txt_get_tag_table(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(RVAL2GOBJ(self))));
}

static VALUE
txt_set_text(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE text, len;
    rb_scan_args(argc, argv, "11", &text, &len);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(RVAL2GOBJ(self)),
                             STR2CSTR(text),
                             NIL_P(len) ? RSTRING(text)->len : NUM2INT(len));
    return self;
}

static VALUE
txt_set_text2(self, text)
    VALUE self, text;
{
    txt_set_text(1, &text, self);
    return text;
}

#if 0

/* Insert into the buffer */
void gtk_text_buffer_insert            (GtkTextBuffer *buffer,
                                        GtkTextIter   *iter,
                                        const gchar   *text,
                                        gint           len);
void gtk_text_buffer_insert_at_cursor  (GtkTextBuffer *buffer,
                                        const gchar   *text,
                                        gint           len);

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
                           get_gtktextiter(start),
                           get_gtktextiter(end));
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

static VALUE
txt_get_insert(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_insert(GTK_TEXT_BUFFER(RVAL2GOBJ(self))));
}

static VALUE
txt_get_selection_bound(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_buffer_get_selection_bound(GTK_TEXT_BUFFER(RVAL2GOBJ(self))));
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
    return gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

static VALUE
txt_set_modified(self, setting)
    VALUE self, setting;
{
    gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(RVAL2GOBJ(self)), RTEST(setting));
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
    return gtk_text_buffer_delete_selection(
        GTK_TEXT_BUFFER(RVAL2GOBJ(self)),
        RTEST(interactive),
        RTEST(default_editable)) ? Qtrue : Qfalse;
}

static VALUE
txt_end_user_action(self)
    VALUE self;
{
    gtk_text_buffer_end_user_action(GTK_TEXT_BUFFER(RVAL2GOBJ(self)));
    return self;
}

static VALUE
txt_begin_user_action(self)
    VALUE self;
{
    gtk_text_buffer_begin_user_action(GTK_TEXT_BUFFER(RVAL2GOBJ(self)));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, txt_end_user_action, self);
    return self;
}

void Init_gtk_textbuffer()
{
    extern void Init_gtk_textiter();
    VALUE gTextBuffer = G_DEF_CLASS(GTK_TYPE_TEXT_BUFFER, "TextBuffer", mGtk);

    rb_define_method(gTextBuffer, "initialize", txt_initialize, -1);
    rb_define_method(gTextBuffer, "line_count", txt_get_line_count, 0);
    rb_define_method(gTextBuffer, "cher_count", txt_get_char_count, 0);
    rb_define_method(gTextBuffer, "tag_table", txt_get_tag_table, 0);

    rb_define_method(gTextBuffer, "set_text", txt_set_text, -1);
    rb_define_method(gTextBuffer, "text=", txt_set_text2, 1);

    rb_define_method(gTextBuffer, "delete", txt_delete, 2);

    rb_define_method(gTextBuffer, "get_insert", txt_get_insert, 0);
    rb_define_method(gTextBuffer, "selection_bound", txt_get_selection_bound, 0);

    rb_define_method(gTextBuffer, "modified", txt_get_modified, 0);
    rb_define_method(gTextBuffer, "modified=", txt_set_modified, 1);

    rb_define_method(gTextBuffer, "delete_selection", txt_delete_selection, -1);

    rb_define_method(gTextBuffer, "begin_user_action", txt_begin_user_action, 0);
    rb_define_method(gTextBuffer, "end_user_action", txt_end_user_action, 0);

    Init_gtk_textiter();
}

