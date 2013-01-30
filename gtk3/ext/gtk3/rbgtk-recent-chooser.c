/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#define RG_TARGET_NAMESPACE mRecentChooser
#define _SELF(self) (RVAL2GTKRECENTCHOOSER(self))

static VALUE rb_mGtk;

struct callback_arg
{
    VALUE callback;
    VALUE a, b;
};

static VALUE
invoke_callback(VALUE data)
{
    struct callback_arg *arg = (struct callback_arg *)data;
    return rb_funcall(arg->callback, id_call, 1, arg->a, arg->b);
}

static void
remove_callback_reference(gpointer callback)
{
    G_CHILD_REMOVE(rb_mGtk, (VALUE)callback);
}

static gint
sort_func(GtkRecentInfo *a, GtkRecentInfo *b, gpointer func)
{
    struct callback_arg arg;

    arg.callback = (VALUE)func;
    arg.a = GTKRECENTINFO2RVAL(a);
    arg.b = GTKRECENTINFO2RVAL(b);
    return NUM2INT(G_PROTECT_CALLBACK(invoke_callback, &arg));
}

static VALUE
rg_set_sort_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_CHILD_ADD(rb_mGtk, func);
    gtk_recent_chooser_set_sort_func(_SELF(self), (GtkRecentSortFunc)sort_func,
                                     (gpointer)func, (GDestroyNotify)remove_callback_reference);
    return self;
}

static VALUE
rg_set_current_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = gtk_recent_chooser_set_current_uri(_SELF(self),
                                                      RVAL2CSTR(uri),
                                                      &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

static VALUE
rg_current_uri(VALUE self)
{
    return CSTR2RVAL(gtk_recent_chooser_get_current_uri(_SELF(self)));
}

static VALUE
rg_current_item(VALUE self)
{
    return GTKRECENTINFO2RVAL(gtk_recent_chooser_get_current_item(_SELF(self)));
}

static VALUE
rg_select_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = gtk_recent_chooser_select_uri(_SELF(self),
                                                 RVAL2CSTR(uri),
                                                 &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

static VALUE
rg_unselect_uri(VALUE self, VALUE uri)
{
    gtk_recent_chooser_unselect_uri(_SELF(self), RVAL2CSTR(uri));
    return self;
}

static VALUE
rg_select_all(VALUE self)
{
    gtk_recent_chooser_select_all(_SELF(self));
    return self;
}

static VALUE
rg_unselect_all(VALUE self)
{
    gtk_recent_chooser_unselect_all(_SELF(self));
    return self;
}

static VALUE
rg_items(VALUE self)
{
    GList* list = gtk_recent_chooser_get_items(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GTKRECENTINFO2RVAL(list->data));
        gtk_recent_info_unref(list->data);
        list = list->next;
    }
    g_list_free(list);
    return ary;
}

static VALUE
rg_uris(VALUE self)
{
    gsize i;
    gsize size;
    gchar** uris = gtk_recent_chooser_get_uris(_SELF(self), &size);
    VALUE ary = rb_ary_new();
    for (i = 0; i < size; i++){
        rb_ary_push(ary, CSTR2RVAL(uris[i]));
    }
    g_strfreev(uris);
    return ary;
}

static VALUE
rg_add_filter(VALUE self, VALUE filter)
{
    gtk_recent_chooser_add_filter(_SELF(self), 
                                  RVAL2GTKRECENTFILTER(filter));
    return self;
}

static VALUE
rg_remove_filter(VALUE self, VALUE filter)
{
    gtk_recent_chooser_remove_filter(_SELF(self),
                                     RVAL2GTKRECENTFILTER(filter));
    return self;
}

static VALUE
rg_filters(VALUE self)
{
    return GOBJGSLIST2RVAL_FREE(gtk_recent_chooser_list_filters(_SELF(self)),
                                g_slist_free, NULL);
}

void 
Init_gtk_recent_chooser(VALUE mGtk)
{
    rb_mGtk = mGtk;
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_RECENT_CHOOSER, "RecentChooser", mGtk);

    RG_DEF_METHOD(set_sort_func, 0);
    RG_DEF_METHOD(set_current_uri, 1);
    RG_DEF_METHOD(current_uri, 0);
    RG_DEF_METHOD(current_item, 0);
    RG_DEF_METHOD(select_uri, 1);
    RG_DEF_METHOD(unselect_uri, 1);
    RG_DEF_METHOD(select_all, 0);
    RG_DEF_METHOD(unselect_all, 0);
    RG_DEF_METHOD(items, 0);
    RG_DEF_METHOD(uris, 0);
    RG_DEF_METHOD(add_filter, 1);
    RG_DEF_METHOD(remove_filter, 1);
    RG_DEF_METHOD(filters, 0);

    G_DEF_ERROR(GTK_RECENT_CHOOSER_ERROR, "RecentChooserError", mGtk, rb_eRuntimeError,
                GTK_TYPE_RECENT_CHOOSER_ERROR);
    G_DEF_CLASS(GTK_TYPE_RECENT_SORT_TYPE, "SortType", RG_TARGET_NAMESPACE);
}
