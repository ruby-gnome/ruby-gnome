/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelmap.c -

  $Author: ogs $
  $Date: 2002/12/12 15:52:22 $

  Copyright (C) 2002 OGASAWARA, Takeshi
************************************************/
#include "global.h"

static VALUE
accel_map_add_entry(self, path, key, mods)
    VALUE self, path, key, mods;
{
    gtk_accel_map_add_entry(RVAL2CSTR(path), NUM2UINT(key), FIX2INT(mods));
    return Qnil;
}

static VALUE
accel_map_lookup_entry(self, path)
    VALUE self, path;
{
    GtkAccelKey key;
    if(gtk_accel_map_lookup_entry(RVAL2CSTR(path), &key))
        return BOXED2RVAL(&key, GTK_TYPE_ACCEL_KEY);
    else
        return Qnil;
}

static VALUE
accel_map_change_entry(self, path, key, mods, replace)
    VALUE self, path, key, mods, replace;
{
    return gtk_accel_map_change_entry(RVAL2CSTR(path), NUM2UINT(key),
                                      FIX2INT(mods), RTEST(replace))
        ? Qtrue : Qfalse;
}

static VALUE
accel_map_load(self, filename)
    VALUE self, filename;
{
    gtk_accel_map_load(RVAL2CSTR(filename));
    return Qnil;
}

static VALUE
accel_map_save(self, filename)
    VALUE self, filename;
{
    gtk_accel_map_save(RVAL2CSTR(filename));
    return Qnil;
}

static VALUE
accel_map_add_filter(self, pattern)
    VALUE self, pattern;
{
    gtk_accel_map_add_filter(RVAL2CSTR(pattern));
    return Qnil;
}

void
accel_map_foreach_func(func, path, key, mods, changed)
    gpointer func;
    const gchar *path;
    guint key;
    GdkModifierType mods;
    gboolean changed;
{
    rb_funcall((VALUE)func, id_call, 4,
               CSTR2RVAL(path), UINT2NUM(key), INT2FIX(mods),
               changed ? Qtrue : Qfalse);
}

static VALUE
accel_map_foreach(self)
    VALUE self;
{
    VALUE func = rb_f_lambda();
    G_RELATIVE(self, func);
    gtk_accel_map_foreach((gpointer)func,
                          (GtkAccelMapForeach)accel_map_foreach_func);
    return Qnil;
}

static VALUE
accel_map_foreach_unfilterd(self)
    VALUE self;
{
    VALUE func = rb_f_lambda();
    G_RELATIVE(self, func);
    gtk_accel_map_foreach_unfiltered((gpointer)func,
                                     (GtkAccelMapForeach)accel_map_foreach_func);
    return Qnil;
}

#if 0
void        gtk_accel_map_load_scanner      (GScanner *scanner);
void        gtk_accel_map_load_fd           (gint fd);
void        gtk_accel_map_save_fd           (gint fd);
#endif

void
Init_accel_map()
{
    VALUE mAccelMap = rb_define_module_under(mGtk, "AccelMap");

    rb_define_module_function(mAccelMap, "add_entry", accel_map_add_entry, 3);
    rb_define_module_function(mAccelMap, "lookup_entry", accel_map_lookup_entry, 1);
    rb_define_module_function(mAccelMap, "change_entry", accel_map_change_entry, 4);
    rb_define_module_function(mAccelMap, "load", accel_map_load, 1);
    rb_define_module_function(mAccelMap, "save", accel_map_save, 1);
    rb_define_module_function(mAccelMap, "add_filter", accel_map_add_filter, 1);
    rb_define_module_function(mAccelMap, "each", accel_map_foreach, 0);
    rb_define_module_function(mAccelMap, "each_unfilterd", accel_map_foreach_unfilterd, 0);
}
