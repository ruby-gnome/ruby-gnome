/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentchooser.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_CHOOSER(RVAL2GOBJ(self)))

/* Defined as Properties 
void        gtk_recent_chooser_set_show_private
                                            (GtkRecentChooser *chooser,
                                             gboolean show_private);
gboolean    gtk_recent_chooser_get_show_private
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_show_not_found
                                            (GtkRecentChooser *chooser,
                                             gboolean show_not_found);
gboolean    gtk_recent_chooser_get_show_not_found
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_show_icons
                                            (GtkRecentChooser *chooser,
                                             gboolean show_icons);
gboolean    gtk_recent_chooser_get_show_icons
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_select_multiple
                                            (GtkRecentChooser *chooser,
                                             gboolean select_multiple);
gboolean    gtk_recent_chooser_get_select_multiple
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_local_only
                                            (GtkRecentChooser *chooser,
                                             gboolean local_only);
gboolean    gtk_recent_chooser_get_local_only
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_limit    (GtkRecentChooser *chooser,
                                             gint limit);
gint        gtk_recent_chooser_get_limit    (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_show_tips
                                            (GtkRecentChooser *chooser,
                                             gboolean show_tips);
gboolean    gtk_recent_chooser_get_show_tips
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_sort_type
                                            (GtkRecentChooser *chooser,
                                             GtkRecentSortType sort_type);
GtkRecentSortType gtk_recent_chooser_get_sort_type
                                            (GtkRecentChooser *chooser);
void        gtk_recent_chooser_set_filter   (GtkRecentChooser *chooser,
                                             GtkRecentFilter *filter);
GtkRecentFilter* gtk_recent_chooser_get_filter
                                            (GtkRecentChooser *chooser);

*/
static VALUE
rc_set_show_numbers(VALUE self, VALUE val)
{
    gtk_recent_chooser_set_show_numbers(_SELF(self), RVAL2CBOOL(val));
    return self;
}

static VALUE
rc_get_show_numbers(VALUE self)
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
rc_set_sort_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_CHILD_ADD(mGtk, func);
    gtk_recent_chooser_set_sort_func(_SELF(self), (GtkRecentSortFunc)sort_func,
                                     (gpointer)func, (GDestroyNotify)remove_callback_reference);
    return self;
}

static VALUE
rc_set_current_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = gtk_recent_chooser_set_current_uri(_SELF(self),
                                                      RVAL2CSTR(uri),
                                                      &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

static VALUE
rc_get_current_uri(VALUE self)
{
    return CSTR2RVAL(gtk_recent_chooser_get_current_uri(_SELF(self)));
}

static VALUE
rc_get_current_item(VALUE self)
{
    return BOXED2RVAL(gtk_recent_chooser_get_current_item(_SELF(self)),
                      GTK_TYPE_RECENT_INFO);
}

static VALUE
rc_select_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = gtk_recent_chooser_select_uri(_SELF(self),
                                                 RVAL2CSTR(uri),
                                                 &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

static VALUE
rc_unselect_uri(VALUE self, VALUE uri)
{
    gtk_recent_chooser_unselect_uri(_SELF(self), RVAL2CSTR(uri));
    return self;
}

static VALUE
rc_select_all(VALUE self)
{
    gtk_recent_chooser_select_all(_SELF(self));
    return self;
}

static VALUE
rc_unselect_all(VALUE self)
{
    gtk_recent_chooser_unselect_all(_SELF(self));
    return self;
}

static VALUE
rc_get_items(VALUE self)
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
rc_get_uris(VALUE self)
{
    int i;
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
rc_add_filter(VALUE self, VALUE filter)
{
    gtk_recent_chooser_add_filter(_SELF(self), 
                                  GTK_RECENT_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
rc_remove_filter(VALUE self, VALUE filter)
{
    gtk_recent_chooser_remove_filter(_SELF(self),
                                     GTK_RECENT_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
rc_list_filters(VALUE self)
{
    return GSLIST2ARYF(gtk_recent_chooser_list_filters(_SELF(self)));
}
#endif

void 
Init_gtk_recent_chooser()
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE gRecentChooser = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER, "RecentChooser", mGtk);

  rb_define_method(gRecentChooser, "set_show_numbers", rc_set_show_numbers, 1);
  rb_define_method(gRecentChooser, "show_numbers", rc_get_show_numbers, 0);
  rb_define_method(gRecentChooser, "set_sort_func", rc_set_sort_func, 0);
  rb_define_method(gRecentChooser, "set_current_uri", rc_set_current_uri, 1);
  rb_define_method(gRecentChooser, "current_uri", rc_get_current_uri, 0);
  rb_define_method(gRecentChooser, "current_item", rc_get_current_item, 0);
  rb_define_method(gRecentChooser, "select_uri", rc_select_uri, 1);
  rb_define_method(gRecentChooser, "unselect_uri", rc_unselect_uri, 1);
  rb_define_method(gRecentChooser, "select_all", rc_select_all, 0);
  rb_define_method(gRecentChooser, "unselect_all", rc_unselect_all, 0);
  rb_define_method(gRecentChooser, "items", rc_get_items, 0);
  rb_define_method(gRecentChooser, "uris", rc_get_uris, 0);
  rb_define_method(gRecentChooser, "add_filter", rc_add_filter, 1);
  rb_define_method(gRecentChooser, "remove_filter", rc_remove_filter, 1);
  rb_define_method(gRecentChooser, "filters", rc_list_filters, 0);
  
  G_DEF_SETTERS(gRecentChooser);
  
  /* GtkRecentChooserError */
  G_DEF_ERROR(GTK_RECENT_CHOOSER_ERROR, "RecentChooserError", mGtk, rb_eRuntimeError,
              GTK_TYPE_RECENT_CHOOSER_ERROR);

  /* GtkRecentSortType */
  G_DEF_CLASS(GTK_TYPE_RECENT_SORT_TYPE, "SortType", gRecentChooser);
  G_DEF_CONSTANTS(gRecentChooser, GTK_TYPE_RECENT_SORT_TYPE, "GTK_RECENT_");
#endif

}
