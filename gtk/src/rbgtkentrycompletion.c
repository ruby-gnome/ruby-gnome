/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkentrycompletion.c -

  $Author: mutoh $
  $Date: 2004/05/24 16:18:58 $

  Copyright (C) 2004 Masao Mutoh
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
    return RTEST(rb_funcall((VALUE)func, id_call, 3, GOBJ2RVAL(completion),
                            CSTR2RVAL(key), BOXED2RVAL(iter, GTK_TYPE_TREE_ITER)));
}

static VALUE
entryc_set_match_func(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_entry_completion_set_match_func(_SELF(self), entryc_match_func,
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
    rb_define_method(gEntryC, "insert_action_text", entryc_insert_action_text, 2);
    rb_define_method(gEntryC, "insert_action_markup", entryc_insert_action_markup, 2);
    rb_define_method(gEntryC, "delete_action", entryc_delete_action, 1);
    rb_define_method(gEntryC, "set_text_column", entryc_set_text_column, 1);

    G_DEF_SETTERS(gEntryC);
#endif
}
