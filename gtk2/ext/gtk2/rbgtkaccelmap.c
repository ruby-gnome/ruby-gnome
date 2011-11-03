/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#define RG_TARGET_NAMESPACE cAccelMap

#define RVAL2MOD(mods) (NIL_P(mods) ? 0 : RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE))

static VALUE
rg_s_add_entry(VALUE self, VALUE path, VALUE key, VALUE mods)
{
    gtk_accel_map_add_entry(RVAL2CSTR(path), NUM2UINT(key), RVAL2MOD(mods));
    return self;
}

static VALUE
rg_s_lookup_entry(VALUE self, VALUE path)
{
    GtkAccelKey key;
    if(gtk_accel_map_lookup_entry(RVAL2CSTR(path), &key))
        return BOXED2RVAL(&key, GTK_TYPE_ACCEL_KEY);
    else
        return self;
}

static VALUE
rg_s_change_entry(G_GNUC_UNUSED VALUE self, VALUE path, VALUE key, VALUE mods, VALUE replace)
{
    return CBOOL2RVAL(gtk_accel_map_change_entry(RVAL2CSTR(path), NUM2UINT(key),
                                                 RVAL2MOD(mods), RVAL2CBOOL(replace)));
}

static VALUE
rg_s_load(VALUE self, VALUE filename)
{
    gtk_accel_map_load(RVAL2CSTR(filename));
    return self;
}

static VALUE
rg_s_save(VALUE self, VALUE filename)
{
    gtk_accel_map_save(RVAL2CSTR(filename));
    return self;
}

static VALUE
rg_s_add_filter(VALUE self, VALUE pattern)
{
    gtk_accel_map_add_filter(RVAL2CSTR(pattern));
    return self;
}

static void
accel_map_foreach_func(gpointer func, const gchar *path, guint key, GdkModifierType mods, gboolean changed)
{
    rb_funcall((VALUE)func, id_call, 4,
               CSTR2RVAL(path), UINT2NUM(key), GFLAGS2RVAL(mods, GDK_TYPE_MODIFIER_TYPE),
               CBOOL2RVAL(changed));
}

static VALUE
rg_s_each(VALUE self)
{
    volatile VALUE func = rb_block_proc();
    gtk_accel_map_foreach((gpointer)func,
                          (GtkAccelMapForeach)accel_map_foreach_func);
    return self;
}

static VALUE
rg_s_each_unfilterd(VALUE self)
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
rg_s_get(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_accel_map_get());
}

static VALUE
rg_s_lock_path(VALUE self, VALUE accel_path)
{
    gtk_accel_map_lock_path(RVAL2CSTR(accel_path));
    return self;
}

static VALUE
rg_s_unlock_path(VALUE self, VALUE accel_path)
{
    gtk_accel_map_unlock_path(RVAL2CSTR(accel_path));
    return self;
}
#endif

void
Init_gtk_accel_map(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCEL_MAP, "AccelMap", mGtk);

    RG_DEF_SMETHOD(add_entry, 3);
    RG_DEF_SMETHOD(lookup_entry, 1);
    RG_DEF_SMETHOD(change_entry, 4);
    RG_DEF_SMETHOD(load, 1);
    RG_DEF_SMETHOD(save, 1);
    RG_DEF_SMETHOD(add_filter, 1);
    RG_DEF_SMETHOD(each, 0);
    RG_DEF_SMETHOD(each_unfilterd, 0);
    RG_DEF_SMETHOD(get, 0);
    RG_DEF_SMETHOD(lock_path, 1);
    RG_DEF_SMETHOD(unlock_path, 1);
#else
    VALUE mAccelMap = rb_define_module_under(mGtk, "AccelMap");

    rb_define_module_function(mAccelMap, "add_entry", rg_s_add_entry, 3);
    rb_define_module_function(mAccelMap, "lookup_entry", rg_s_lookup_entry, 1);
    rb_define_module_function(mAccelMap, "change_entry", rg_s_change_entry, 4);
    rb_define_module_function(mAccelMap, "load", rg_s_load, 1);
    rb_define_module_function(mAccelMap, "save", rg_s_save, 1);
    rb_define_module_function(mAccelMap, "add_filter", rg_s_add_filter, 1);
    rb_define_module_function(mAccelMap, "each", rg_s_each, 0);
    rb_define_module_function(mAccelMap, "each_unfilterd", rg_s_each_unfilterd, 0);
#endif
}
