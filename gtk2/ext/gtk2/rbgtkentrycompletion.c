/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cEntryCompletion
#define _SELF(self) (GTK_ENTRY_COMPLETION(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_entry_completion_new());
    return Qnil;
}

static VALUE
rg_entry(VALUE self)
{
    return GOBJ2RVAL(gtk_entry_completion_get_entry(_SELF(self)));
}

static gboolean
entryc_match_func(GtkEntryCompletion *completion, const gchar *key, GtkTreeIter *iter, gpointer func)
{
    iter->user_data3 = gtk_entry_completion_get_model(completion);
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 3, GOBJ2RVAL(completion),
                            CSTR2RVAL(key), GTKTREEITER2RVAL(iter)));
}

static VALUE
rg_set_match_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_entry_completion_set_match_func(_SELF(self), 
                                        (GtkEntryCompletionMatchFunc)entryc_match_func,
                                        (gpointer)func, NULL);
    return self;
}

static VALUE
rg_complete(VALUE self)
{
    gtk_entry_completion_complete(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_insert_prefix(VALUE self)
{
    gtk_entry_completion_insert_prefix(_SELF(self));
    return self;
}
#endif

static VALUE
rg_insert_action_text(VALUE self, VALUE index, VALUE text)
{
    gtk_entry_completion_insert_action_text(_SELF(self), NUM2INT(index), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_insert_action_markup(VALUE self, VALUE index, VALUE markup)
{
    gtk_entry_completion_insert_action_markup(_SELF(self), NUM2INT(index), RVAL2CSTR(markup));
    return self;
}

static VALUE
rg_delete_action(VALUE self, VALUE index)
{
    gtk_entry_completion_delete_action(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
entryc_set_text_column(VALUE self, VALUE column)
{
    gtk_entry_completion_set_text_column(_SELF(self), NUM2INT(column));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_completion_prefix(VALUE self)
{
    return CSTR2RVAL(gtk_entry_completion_get_completion_prefix(_SELF(self)));
}
#endif

#endif

void 
Init_gtk_entry_completion(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ENTRY_COMPLETION, "EntryCompletion", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(entry, 0);
    RG_DEF_METHOD(set_match_func, 0);
    RG_DEF_METHOD(complete, 0);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(insert_prefix, 0);
#endif
    RG_DEF_METHOD(insert_action_text, 2);
    RG_DEF_METHOD(insert_action_markup, 2);
    RG_DEF_METHOD(delete_action, 1);

    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "text_column", entryc_set_text_column, 1);

#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(completion_prefix, 0);
#endif
#endif
}
