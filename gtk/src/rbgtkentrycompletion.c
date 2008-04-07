/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkentrycompletion.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2004,2005 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)
#define _SELF(self) (GTK_ENTRY_COMPLETION(RVAL2GOBJ(self)))

static VALUE
entryc_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_entry_completion_new());
    return Qnil;
}

static VALUE
entryc_get_entry(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_entry_completion_get_entry(_SELF(self)));
}

static gboolean
entryc_match_func(completion, key, iter, func)
    GtkEntryCompletion* completion;
    const gchar* key;
    GtkTreeIter* iter;
    gpointer func;
{
    iter->user_data3 = gtk_entry_completion_get_model(completion);
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 3, GOBJ2RVAL(completion),
                            CSTR2RVAL(key), BOXED2RVAL(iter, GTK_TYPE_TREE_ITER)));
}

static VALUE
entryc_set_match_func(self)
    VALUE self;
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_entry_completion_set_match_func(_SELF(self), 
                                        (GtkEntryCompletionMatchFunc)entryc_match_func,
                                        (gpointer)func, NULL);
    return self;
}

static VALUE
entryc_complete(self)
    VALUE self;
{
    gtk_entry_completion_complete(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
entryc_insert_prefix(self)
    VALUE self;
{
    gtk_entry_completion_insert_prefix(_SELF(self));
    return self;
}
#endif

static VALUE
entryc_insert_action_text(self, index, text)
    VALUE self, index, text;
{
    gtk_entry_completion_insert_action_text(_SELF(self), NUM2INT(index), RVAL2CSTR(text));
    return self;
}

static VALUE
entryc_insert_action_markup(self, index, markup)
    VALUE self, index, markup;
{
    gtk_entry_completion_insert_action_markup(_SELF(self), NUM2INT(index), RVAL2CSTR(markup));
    return self;
}

static VALUE
entryc_delete_action(self, index)
    VALUE self, index;
{
    gtk_entry_completion_delete_action(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
entryc_set_text_column(self, column)
    VALUE self, column;
{
    gtk_entry_completion_set_text_column(_SELF(self), NUM2INT(column));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
entryc_get_completion_prefix(VALUE self)
{
    return CSTR2RVAL(gtk_entry_completion_get_completion_prefix(_SELF(self)));
}
#endif

/* Defined as property
gint        gtk_entry_completion_get_text_column
                                            (GtkEntryCompletion *completion);
void        gtk_entry_completion_set_inline_completion
                                            (GtkEntryCompletion *completion,
                                             gboolean inline_completion);
gboolean    gtk_entry_completion_get_inline_completion
                                            (GtkEntryCompletion *completion);
void        gtk_entry_completion_set_popup_completion
                                            (GtkEntryCompletion *completion,
                                             gboolean popup_completion);
gboolean    gtk_entry_completion_get_popup_completion
                                            (GtkEntryCompletion *completion);
void        gtk_entry_completion_set_popup_set_width
                                            (GtkEntryCompletion *completion,
                                             gboolean popup_set_width);
gboolean    gtk_entry_completion_get_popup_set_width
                                            (GtkEntryCompletion *completion);
void        gtk_entry_completion_set_popup_single_match
                                            (GtkEntryCompletion *completion,
                                             gboolean popup_single_match);
gboolean    gtk_entry_completion_get_popup_single_match
                                            (GtkEntryCompletion *completion);
*/

#endif

void 
Init_gtk_entry_completion()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gEntryC = G_DEF_CLASS(GTK_TYPE_ENTRY_COMPLETION, "EntryCompletion", mGtk);

    rb_define_method(gEntryC, "initialize", entryc_initialize, 0);
    rb_define_method(gEntryC, "entry", entryc_get_entry, 0);
    rb_define_method(gEntryC, "set_match_func", entryc_set_match_func, 0);
    rb_define_method(gEntryC, "complete", entryc_complete, 0);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gEntryC, "insert_prefix", entryc_insert_prefix, 0);
#endif
    rb_define_method(gEntryC, "insert_action_text", entryc_insert_action_text, 2);
    rb_define_method(gEntryC, "insert_action_markup", entryc_insert_action_markup, 2);
    rb_define_method(gEntryC, "delete_action", entryc_delete_action, 1);

    rb_undef_method(gEntryC, "text_column=");
    rb_define_method(gEntryC, "set_text_column", entryc_set_text_column, 1);

#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(gEntryC, "completion_prefix", entryc_get_completion_prefix, 0);
#endif

    G_DEF_SETTERS(gEntryC);
#endif
}
