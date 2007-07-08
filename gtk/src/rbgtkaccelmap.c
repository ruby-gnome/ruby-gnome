/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelmap.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define RVAL2MOD(mods) (NIL_P(mods) ? 0 : RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE))

static VALUE
accel_map_add_entry(self, path, key, mods)
    VALUE self, path, key, mods;
{
    gtk_accel_map_add_entry(RVAL2CSTR(path), NUM2UINT(key), RVAL2MOD(mods));
    return self;
}

static VALUE
accel_map_lookup_entry(self, path)
    VALUE self, path;
{
    GtkAccelKey key;
    if(gtk_accel_map_lookup_entry(RVAL2CSTR(path), &key))
        return BOXED2RVAL(&key, GTK_TYPE_ACCEL_KEY);
    else
        return self;
}

static VALUE
accel_map_change_entry(self, path, key, mods, replace)
    VALUE self, path, key, mods, replace;
{
    return gtk_accel_map_change_entry(RVAL2CSTR(path), NUM2UINT(key),
                                      RVAL2MOD(mods), RTEST(replace))
        ? Qtrue : Qfalse;
}

static VALUE
accel_map_load(self, filename)
    VALUE self, filename;
{
    gtk_accel_map_load(RVAL2CSTR(filename));
    return self;
}

static VALUE
accel_map_save(self, filename)
    VALUE self, filename;
{
    gtk_accel_map_save(RVAL2CSTR(filename));
    return self;
}

static VALUE
accel_map_add_filter(self, pattern)
    VALUE self, pattern;
{
    gtk_accel_map_add_filter(RVAL2CSTR(pattern));
    return self;
}

static void
accel_map_foreach_func(func, path, key, mods, changed)
    gpointer func;
    const gchar *path;
    guint key;
    GdkModifierType mods;
    gboolean changed;
{
    rb_funcall((VALUE)func, id_call, 4,
               CSTR2RVAL(path), UINT2NUM(key), GFLAGS2RVAL(mods, GDK_TYPE_MODIFIER_TYPE),
               changed ? Qtrue : Qfalse);
}

static VALUE
accel_map_foreach(self)
    VALUE self;
{
    volatile VALUE func = rb_block_proc();
    gtk_accel_map_foreach((gpointer)func,
                          (GtkAccelMapForeach)accel_map_foreach_func);
    return self;
}

static VALUE
accel_map_foreach_unfilterd(self)
    VALUE self;
{
    volatile VALUE func = rb_block_proc();
    gtk_accel_map_foreach_unfiltered((gpointer)func,
                                     (GtkAccelMapForeach)accel_map_foreach_func);
    return self;
}

/*
void        gtk_accel_map_load_scanner      (GScanner *scanner);
*/

/* We don't need them.
void        gtk_accel_map_load_fd           (gint fd);
void        gtk_accel_map_save_fd           (gint fd);
*/

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
accel_map_get(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_accel_map_get());
}

static VALUE
accel_map_lock_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_accel_map_lock_path(RVAL2CSTR(accel_path));
    return self;
}

static VALUE
accel_map_unlock_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_accel_map_unlock_path(RVAL2CSTR(accel_path));
    return self;
}
#endif

void
Init_accel_map()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE mAccelMap = G_DEF_CLASS(GTK_TYPE_ACCEL_MAP, "AccelMap", mGtk);

    rb_define_singleton_method(mAccelMap, "add_entry", accel_map_add_entry, 3);
    rb_define_singleton_method(mAccelMap, "lookup_entry", accel_map_lookup_entry, 1);
    rb_define_singleton_method(mAccelMap, "change_entry", accel_map_change_entry, 4);
    rb_define_singleton_method(mAccelMap, "load", accel_map_load, 1);
    rb_define_singleton_method(mAccelMap, "save", accel_map_save, 1);
    rb_define_singleton_method(mAccelMap, "add_filter", accel_map_add_filter, 1);
    rb_define_singleton_method(mAccelMap, "each", accel_map_foreach, 0);
    rb_define_singleton_method(mAccelMap, "each_unfilterd", accel_map_foreach_unfilterd, 0);
    rb_define_singleton_method(mAccelMap, "get", accel_map_get, 0);
    rb_define_singleton_method(mAccelMap, "lock_path", accel_map_lock_path, 1);
    rb_define_singleton_method(mAccelMap, "unlock_path", accel_map_unlock_path, 1);
#else
    VALUE mAccelMap = rb_define_module_under(mGtk, "AccelMap");
                                                                                
    rb_define_module_function(mAccelMap, "add_entry", accel_map_add_entry, 3);
    rb_define_module_function(mAccelMap, "lookup_entry", accel_map_lookup_entry, 1);
    rb_define_module_function(mAccelMap, "change_entry", accel_map_change_entry, 4);
    rb_define_module_function(mAccelMap, "load", accel_map_load, 1);
    rb_define_module_function(mAccelMap, "save", accel_map_save, 1);
    rb_define_module_function(mAccelMap, "add_filter", accel_map_add_filter, 1);
    rb_define_module_function(mAccelMap, "each", accel_map_foreach, 0);
    rb_define_module_function(mAccelMap, "each_unfilterd", accel_map_foreach_unfilterd, 0);
#endif
}
