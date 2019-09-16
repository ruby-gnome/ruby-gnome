/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2018-2019  Ruby-GNOME Project Team
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

#include "rbgprivate.h"

static GHashTable *rbg_objects = NULL;

static void
gc_marker_mark_each(G_GNUC_UNUSED gpointer key,
                    gpointer value,
                    G_GNUC_UNUSED gpointer user_data)
{
    VALUE rb_object = (VALUE)value;
    rb_gc_mark(rb_object);
}

static void
gc_marker_mark(void *data)
{
    GHashTable *hash_table = data;
    g_hash_table_foreach(hash_table, gc_marker_mark_each, NULL);
}

static void
gc_marker_free(void *data)
{
    GHashTable *hash_table = data;
    g_hash_table_unref(hash_table);
    if (hash_table == rbg_objects) {
        rbg_objects = NULL;
    }
}

static const rb_data_type_t rbg_gc_marker_type = {
    "GLib::GCMarker",
    {
        gc_marker_mark,
        gc_marker_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

void
rbg_gc_guard(gpointer key, VALUE rb_object)
{
    if (!rbg_objects)
        return;

    g_hash_table_insert(rbg_objects,
                        key,
                        (gpointer)rb_object);
}

void
rbg_gc_unguard(gpointer key)
{
    if (!rbg_objects)
        return;

    g_hash_table_remove(rbg_objects, key);
}

static VALUE
rbg_gc_marker_new_raw(GHashTable *rb_objects)
{
    VALUE gc_marker;

    gc_marker = TypedData_Wrap_Struct(rb_cData,
                                      &rbg_gc_marker_type,
                                      rb_objects);
    return gc_marker;
}

VALUE
rbg_gc_marker_new(void)
{
    GHashTable *rb_objects;

    rb_objects = g_hash_table_new(g_direct_hash, g_direct_equal);
    return rbg_gc_marker_new_raw(rb_objects);
}

void
rbg_gc_marker_guard(VALUE rb_gc_marker, VALUE rb_object)
{
    GHashTable *rb_objects;

    TypedData_Get_Struct(rb_gc_marker,
                         GHashTable,
                         &rbg_gc_marker_type,
                         rb_objects);
    g_hash_table_insert(rb_objects,
                        (gpointer)rb_object,
                        (gpointer)rb_object);
}

void
rbg_gc_marker_unguard(VALUE rb_gc_marker, VALUE rb_object)
{
    GHashTable *rb_objects;

    if (!RB_TYPE_P(rb_gc_marker, RUBY_T_DATA))
        return;
    if (!RTYPEDDATA_P(rb_gc_marker))
        return;

    TypedData_Get_Struct(rb_gc_marker,
                         GHashTable,
                         &rbg_gc_marker_type,
                         rb_objects);
    g_hash_table_remove(rb_objects, (gpointer)rb_object);
}

void
rbg_gc_marker_unguard_all(VALUE rb_gc_marker)
{
    GHashTable *rb_objects;

    TypedData_Get_Struct(rb_gc_marker,
                         GHashTable,
                         &rbg_gc_marker_type,
                         rb_objects);
    g_hash_table_remove_all(rb_objects);
}

void
Init_glib_gc(void)
{
    VALUE gc_marker;

    rbg_objects = g_hash_table_new(g_direct_hash, g_direct_equal);
    gc_marker = rbg_gc_marker_new_raw(rbg_objects);
    rb_ivar_set(mGLib, rb_intern("gc_marker"), gc_marker);
}
