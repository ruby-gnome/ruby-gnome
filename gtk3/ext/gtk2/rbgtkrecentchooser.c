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

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cRecentChooser
#define _SELF(self) (GTK_RECENT_CHOOSER(RVAL2GOBJ(self)))

static VALUE
rg_set_show_numbers(VALUE self, VALUE val)
{
    gtk_recent_chooser_set_show_numbers(_SELF(self), RVAL2CBOOL(val));
    return self;
}

static VALUE
rg_show_numbers(VALUE self)
{
    return CBOOL2RVAL(gtk_recent_chooser_get_show_numbers(_SELF(self)));
}

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
    G_CHILD_REMOVE(mGtk, (VALUE)callback);
}

static gint
sort_func(GtkRecentInfo *a, GtkRecentInfo *b, gpointer func)
{
    struct callback_arg arg;

    arg.callback = (VALUE)func;
    arg.a = BOXED2RVAL(a, GTK_TYPE_RECENT_INFO);
    arg.b = BOXED2RVAL(b, GTK_TYPE_RECENT_INFO);
    return NUM2INT(G_PROTECT_CALLBACK(invoke_callback, &arg));
}

static VALUE
rg_set_sort_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_CHILD_ADD(mGtk, func);
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
    return BOXED2RVAL(gtk_recent_chooser_get_current_item(_SELF(self)),
                      GTK_TYPE_RECENT_INFO);
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
        rb_ary_push(ary, BOXED2RVAL(list->data, GTK_TYPE_RECENT_INFO));
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
                                  GTK_RECENT_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
rg_remove_filter(VALUE self, VALUE filter)
{
    gtk_recent_chooser_remove_filter(_SELF(self),
                                     GTK_RECENT_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
rg_filters(VALUE self)
{
    return GSLIST2ARYF(gtk_recent_chooser_list_filters(_SELF(self)));
}
#endif

void 
Init_gtk_recent_chooser(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER, "RecentChooser", mGtk);

  RG_DEF_METHOD(set_show_numbers, 1);
  G_DEF_SETTER(RG_TARGET_NAMESPACE, "show_numbers");
  RG_DEF_METHOD(show_numbers, 0);
  RG_DEF_METHOD(set_sort_func, 0);
  RG_DEF_METHOD(set_current_uri, 1);
  G_DEF_SETTER(RG_TARGET_NAMESPACE, "current_uri");
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

  /* GtkRecentChooserError */
  G_DEF_ERROR(GTK_RECENT_CHOOSER_ERROR, "RecentChooserError", mGtk, rb_eRuntimeError,
              GTK_TYPE_RECENT_CHOOSER_ERROR);

  /* GtkRecentSortType */
  G_DEF_CLASS(GTK_TYPE_RECENT_SORT_TYPE, "SortType", RG_TARGET_NAMESPACE);
  G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_RECENT_SORT_TYPE, "GTK_RECENT_");
#endif

}
