/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextbuffer.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

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
#define ATOM2RVAL(a) (BOXED2RVAL(a, GDK_TYPE_ATOM))

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
    gtk_text_buffer_set_text(_SELF(self), RVAL2CSTR(text), RSTRING_LEN(text));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
txt_backspace(self, iter, interactive, default_editable)
    VALUE self, iter, interactive, default_editable;
{
    return CBOOL2RVAL(gtk_text_buffer_backspace(_SELF(self), RVAL2ITR(iter),
                                                RVAL2CBOOL(interactive),
                                                RVAL2CBOOL(default_editable)));
}
#endif

static VALUE
txt_insert_at_cursor(self, text)
    VALUE self, text;
{
    StringValue(text);
    gtk_text_buffer_insert_at_cursor(_SELF(self),
                                     RVAL2CSTR(text), RSTRING_LEN(text));
    return self;
}

static VALUE
txt_insert_interactive(self, iter, text, editable)
    VALUE self, iter, text, editable;
{
    return CBOOL2RVAL(gtk_text_buffer_insert_interactive(_SELF(self),
                                                         RVAL2ITR(iter),
                                                         RVAL2CSTR(text),
                                                         RSTRING_LEN(text),
                                                         RVAL2CBOOL(editable)));
}

static VALUE
txt_insert_interactive_at_cursor(self, text, editable)
    VALUE self, text, editable;
{
    return CBOOL2RVAL(gtk_text_buffer_insert_interactive_at_cursor(_SELF(self),
                                                                   RVAL2CSTR(text),
                                                                   RSTRING_LEN(text),
                                                                   RVAL2CBOOL(editable)));
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
    return CBOOL2RVAL(gtk_text_buffer_insert_range_interactive(_SELF(self),
                                                               RVAL2ITR(iter),
                                                               RVAL2ITR(start),
                                                               RVAL2ITR(end),
                                                               RVAL2CBOOL(editable)));
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
    return CBOOL2RVAL(gtk_text_buffer_delete_interactive(_SELF(self),
                                                         RVAL2ITR(start),
                                                         RVAL2ITR(end),
                                                         RVAL2CBOOL(editable)));
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
            RVAL2CBOOL(include_hidden_chars));
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
            RVAL2CBOOL(include_hidden_chars));
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

    if (RVAL2CBOOL(ruby_debug))
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
    if (RVAL2CBOOL(ruby_debug))
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
                                                      RVAL2CBOOL(left_gravity)));
    G_CHILD_ADD(self, ret);
    return ret;
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
txt_add_mark(self, mark, where)
    VALUE self, mark, where;
{
    gtk_text_buffer_add_mark(_SELF(self), RVAL2MARK(mark), RVAL2ITR(where));
    return self;
}
#endif

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
    return CBOOL2RVAL(gtk_text_buffer_get_modified(_SELF(self)));
}

static VALUE
txt_set_modified(self, setting)
    VALUE self, setting;
{
    gtk_text_buffer_set_modified(_SELF(self), RVAL2CBOOL(setting));
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

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
txt_deserialize(self, content_buffer, format, iter, data)
    VALUE self, content_buffer, format, iter, data;
{
    GError* error = NULL;
    gboolean ret;

    Check_Type(data, T_STRING);
    ret = gtk_text_buffer_deserialize(_SELF(self), _SELF(content_buffer),
                                      RVAL2ATOM(format),
                                      RVAL2ITR(iter),
                                      (const guint8*)RSTRING_PTR(data),
                                      (gsize)RSTRING_LEN(data),
                                      &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

static VALUE
txt_deserialize_get_can_create_tags(self, format)
    VALUE self, format;
{
    return CBOOL2RVAL(gtk_text_buffer_deserialize_get_can_create_tags(_SELF(self), 
                                                                      RVAL2ATOM(format)));

}

static VALUE
txt_deserialize_set_can_create_tags(self, format, can_create_tags)
    VALUE self, format, can_create_tags;
{
    gtk_text_buffer_deserialize_set_can_create_tags(_SELF(self),
                                                    RVAL2ATOM(format),
                                                    RVAL2CBOOL(can_create_tags));
    return self;
}

/* Defined as Properties
GtkTargetList* gtk_text_buffer_get_copy_target_list
                                            (GtkTextBuffer *buffer);
*/

static VALUE
txt_get_deserialize_formats(self)
    VALUE self;
{
    gint i;
    gint n_formats;
    GdkAtom* atoms = gtk_text_buffer_get_deserialize_formats(_SELF(self), &n_formats);
    VALUE ary = rb_ary_new();

    for (i = 0; i < n_formats; i++){
        rb_ary_push(ary, ATOM2RVAL(atoms[i]));
    }
    return ary;
}

/* Defined as Properties
GtkTargetList* gtk_text_buffer_get_paste_target_list
                                            (GtkTextBuffer *buffer);
*/

static VALUE
txt_get_serialize_formats(self)
    VALUE self;
{
    gint i;
    gint n_formats;
    GdkAtom* atoms = gtk_text_buffer_get_serialize_formats(_SELF(self), &n_formats);
    VALUE ary = rb_ary_new();

    for (i = 0; i < n_formats; i++){
        rb_ary_push(ary, ATOM2RVAL(atoms[i]));
    }
    return ary;
}

struct callback_arg
{
    VALUE callback;
    int argc;
    VALUE *argv;
};

static VALUE
invoke_callback(VALUE data)
{
    struct callback_arg *arg = (struct callback_arg *)data;

    return rb_funcall2(arg->callback, id_call, arg->argc, arg->argv);
}

static gboolean
deserialize_func(register_buffer, content_buffer, iter, data, length, create_tags, func, error)
    GtkTextBuffer* register_buffer;
    GtkTextBuffer* content_buffer;
    GtkTextIter* iter;
    const guint8 *data;
    gsize length;
    gboolean create_tags;
    gpointer func;
    GError** error;
{
    VALUE result;
    VALUE argv[5];
    struct callback_arg arg;

    argv[0] = GOBJ2RVAL(register_buffer);
    argv[1] = GOBJ2RVAL(content_buffer);
    argv[2] = ITR2RVAL(iter);
    argv[3] = rb_str_new((char*)data, length);
    argv[4] = CBOOL2RVAL(create_tags);

    arg.callback = (VALUE)func;
    arg.argc = 5;
    arg.argv = argv;

    result = G_PROTECT_CALLBACK(invoke_callback, &arg);
    return NIL_P(rb_errinfo()) ? RVAL2CBOOL(result) : FALSE;
}

static void
remove_callback_reference(gpointer callback)
{
    G_CHILD_REMOVE(mGtk, (VALUE)callback);
}

static VALUE
txt_register_deserialize_format(self, mime_type)
    VALUE self, mime_type;
{
    VALUE block = rb_block_proc();
    GdkAtom atom;
    G_CHILD_ADD(mGtk, block);
    atom = gtk_text_buffer_register_deserialize_format(_SELF(self),
                                                       (const gchar*)RVAL2CSTR(mime_type),
                                                       (GtkTextBufferDeserializeFunc)deserialize_func,
                                                       (gpointer)block,
                                                       (GDestroyNotify)remove_callback_reference);
    return ATOM2RVAL(atom);
}

static VALUE
txt_register_deserialize_target(self, tagset_name)
    VALUE self, tagset_name;
{
    return ATOM2RVAL(gtk_text_buffer_register_deserialize_tagset(_SELF(self),
                                                                 (const gchar*)(NIL_P(tagset_name) ? NULL : RVAL2CSTR(tagset_name))));
}

static guint8*
serialize_func(register_buffer, content_buffer, start, end, length, func)
    GtkTextBuffer* register_buffer;
    GtkTextBuffer* content_buffer;
    GtkTextIter* start;
    GtkTextIter* end;
    gsize* length;
    gpointer func;
{
    VALUE result;
    VALUE argv[4];
    struct callback_arg arg;

    argv[0] = GOBJ2RVAL(register_buffer);
    argv[1] = GOBJ2RVAL(content_buffer);
    argv[2] = ITR2RVAL(start);
    argv[3] = ITR2RVAL(end);

    arg.callback = (VALUE)func;
    arg.argc = 4;
    arg.argv = argv;

    /* This should return data as String */
    result = G_PROTECT_CALLBACK(invoke_callback, &arg);
    Check_Type(result, T_STRING);
    *length = RSTRING_LEN(result);
    return (guint8*)(NIL_P(rb_errinfo()) ? RSTRING_PTR(result) : NULL);
}

static VALUE
txt_register_serialize_format(self, mime_type)
    VALUE self, mime_type;
{
    VALUE block = rb_block_proc();
    GdkAtom atom;
    G_CHILD_ADD(mGtk, block);
    atom = gtk_text_buffer_register_serialize_format(_SELF(self),
                                                     (const gchar*)RVAL2CSTR(mime_type),
                                                     (GtkTextBufferSerializeFunc)serialize_func,
                                                     (gpointer)block,
                                                     (GDestroyNotify)remove_callback_reference);
    return ATOM2RVAL(atom);
}

static VALUE
txt_register_serialize_target(self, tagset_name)
    VALUE self, tagset_name;
{
    return ATOM2RVAL(gtk_text_buffer_register_serialize_tagset(_SELF(self),
                                                               NIL_P(tagset_name) ? NULL : RVAL2CSTR(tagset_name)));
}

static VALUE
txt_serialize(self, content_buffer, format, start, end)
    VALUE self, content_buffer, format, start, end;
{
    gsize length;
    guint8* ret = gtk_text_buffer_serialize(_SELF(self), _SELF(self),
                                            RVAL2ATOM(format),
                                            RVAL2ITR(start), RVAL2ITR(end),
                                            &length);
    return rb_str_new((char*)ret, length);
}

static VALUE
txt_unregister_deserialize_format(self, format)
    VALUE self, format;
{
    gtk_text_buffer_unregister_deserialize_format(_SELF(self), RVAL2ATOM(format));
    return self;
}

static VALUE
txt_unregister_serialize_format(self, format)
    VALUE self, format;
{
    gtk_text_buffer_unregister_serialize_format(_SELF(self), RVAL2ATOM(format));
    return self;
}
#endif

static VALUE
txt_cut_clipboard(self, clipboard, default_editable)
    VALUE self, clipboard, default_editable;
{
    G_CHILD_ADD(self, clipboard);
    gtk_text_buffer_cut_clipboard(_SELF(self), RVAL2CLIPBOARD(clipboard), RVAL2CBOOL(default_editable));
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
                                    RVAL2CBOOL(default_editable));
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

/* Defined as a proprety
gboolean    gtk_text_buffer_get_has_selection
                                            (GtkTextBuffer *buffer);
*/

static VALUE
txt_delete_selection(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE interactive, default_editable;
    rb_scan_args(argc, argv, "20", &interactive, &default_editable); 
    return CBOOL2RVAL(gtk_text_buffer_delete_selection(_SELF(self),
                                                       RVAL2CBOOL(interactive), RVAL2CBOOL(default_editable)));
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
        gtk_text_buffer_insert(_SELF(self), RVAL2ITR(where),
                               RVAL2CSTR(value), RSTRING_LEN(value));
        
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
    if (RVAL2CBOOL(ruby_debug))
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
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gTextBuffer, "add_mark", txt_add_mark, 2);
#endif
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
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gTextBuffer, "deserialize", txt_deserialize, 4);
    rb_define_method(gTextBuffer, "deserialize_can_create_tags?", txt_deserialize_get_can_create_tags, 1);
    rb_define_method(gTextBuffer, "deserialize_set_can_create_tags", txt_deserialize_set_can_create_tags, 2);
    rb_define_method(gTextBuffer, "deserialize_formats", txt_get_deserialize_formats, 0);
    rb_define_method(gTextBuffer, "serialize_formats", txt_get_serialize_formats, 0);
    rb_define_method(gTextBuffer, "register_deserialize_format", txt_register_deserialize_format, 1);
    rb_define_method(gTextBuffer, "register_deserialize_target", txt_register_deserialize_target, 1);
    rb_define_method(gTextBuffer, "register_serialize_format", txt_register_serialize_format, 1);
    rb_define_method(gTextBuffer, "register_serialize_target", txt_register_serialize_target, 1);
    rb_define_method(gTextBuffer, "serialize", txt_serialize, 4);
    rb_define_method(gTextBuffer, "unregister_deserialize_format", txt_unregister_deserialize_format, 1);
    rb_define_method(gTextBuffer, "unregister_serialize_format", txt_unregister_serialize_format, 1);
#endif
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

