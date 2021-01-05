/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2018-2020  Ruby-GNOME Project Team
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

typedef struct {
    VALUE object;
    guint ref_count;
} RBGGCMarkItem;

typedef struct {
    GHashTable *items;
    GMutex mutex;
} RBGGCMarker;

static RBGGCMarker *rbg_gc_marker_raw = NULL;

static void
gc_marker_mark_each(G_GNUC_UNUSED gpointer key,
                    gpointer value,
                    G_GNUC_UNUSED gpointer user_data)
{
    RBGGCMarkItem *item = value;
    rb_gc_mark(item->object);
}

static void
gc_marker_mark(void *data)
{
    RBGGCMarker *marker = data;
    g_mutex_lock(&(marker->mutex));
    g_hash_table_foreach(marker->items, gc_marker_mark_each, NULL);
    g_mutex_unlock(&(marker->mutex));
}

static void
gc_marker_free(void *data)
{
    RBGGCMarker *marker_raw = data;
    if (marker_raw == rbg_gc_marker_raw) {
        rbg_gc_marker_raw = NULL;
    }
    g_hash_table_unref(marker_raw->items);
    g_free(marker_raw);
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

static VALUE
rbg_gc_marker_new_raw(void)
{
    RBGGCMarker *gc_marker_raw = g_new(RBGGCMarker, 1);
    gc_marker_raw->items = g_hash_table_new_full(g_direct_hash,
                                                 g_direct_equal,
                                                 NULL,
                                                 g_free);
    g_mutex_init(&(gc_marker_raw->mutex));

    return TypedData_Wrap_Struct(rb_cObject,
                                 &rbg_gc_marker_type,
                                 gc_marker_raw);
}

static void
rbg_gc_guard_raw(RBGGCMarker *gc_marker, gpointer key, VALUE rb_object)
{
    g_mutex_lock(&(gc_marker->mutex));
    RBGGCMarkItem *item = g_hash_table_lookup(gc_marker->items, key);
    if (item) {
        item->ref_count++;
    } else {
        item = g_new(RBGGCMarkItem, 1);
        item->object = rb_object;
        item->ref_count = 1;
        g_hash_table_insert(gc_marker->items, key, item);
    }
    g_mutex_unlock(&(gc_marker->mutex));
}

static void
rbg_gc_unguard_raw(RBGGCMarker *gc_marker, gpointer key)
{
    g_mutex_lock(&(gc_marker->mutex));
    RBGGCMarkItem *item = g_hash_table_lookup(gc_marker->items, key);
    if (item) {
        item->ref_count--;
        if (item->ref_count == 0) {
            g_hash_table_remove(gc_marker->items, key);
        }
    }
    g_mutex_unlock(&(gc_marker->mutex));
}

static void
rbg_gc_unguard_all_raw(RBGGCMarker *gc_marker)
{
    g_mutex_lock(&(gc_marker->mutex));
    g_hash_table_remove_all(gc_marker->items);
    g_mutex_unlock(&(gc_marker->mutex));
}

void
rbg_gc_guard(gpointer key, VALUE rb_object)
{
    if (!rbg_gc_marker_raw)
        return;

    rbg_gc_guard_raw(rbg_gc_marker_raw, key, rb_object);
}

void
rbg_gc_unguard(gpointer key)
{
    if (!rbg_gc_marker_raw)
        return;

    rbg_gc_unguard_raw(rbg_gc_marker_raw, key);
}

VALUE
rbg_gc_marker_new(void)
{
    return rbg_gc_marker_new_raw();
}

void
rbg_gc_marker_guard(VALUE rb_gc_marker, VALUE rb_object)
{
    RBGGCMarker *gc_marker;
    TypedData_Get_Struct(rb_gc_marker,
                         RBGGCMarker,
                         &rbg_gc_marker_type,
                         gc_marker);
    rbg_gc_guard_raw(gc_marker, RVAL2POINTER(rb_object), rb_object);
}

void
rbg_gc_marker_unguard(VALUE rb_gc_marker, VALUE rb_object)
{
    if (!RB_TYPE_P(rb_gc_marker, RUBY_T_DATA))
        return;
    if (!RTYPEDDATA_P(rb_gc_marker))
        return;

    RBGGCMarker *gc_marker;
    TypedData_Get_Struct(rb_gc_marker,
                         RBGGCMarker,
                         &rbg_gc_marker_type,
                         gc_marker);
    rbg_gc_unguard_raw(gc_marker, RVAL2POINTER(rb_object));
}

void
rbg_gc_marker_unguard_all(VALUE rb_gc_marker)
{
    RBGGCMarker *gc_marker;
    TypedData_Get_Struct(rb_gc_marker,
                         RBGGCMarker,
                         &rbg_gc_marker_type,
                         gc_marker);
    rbg_gc_unguard_all_raw(gc_marker);
}

void
Init_glib_gc(void)
{
    VALUE gc_marker = rbg_gc_marker_new_raw();
    rb_ivar_set(mGLib, rb_intern("gc_marker"), gc_marker);
    TypedData_Get_Struct(gc_marker,
                         RBGGCMarker,
                         &rbg_gc_marker_type,
                         rbg_gc_marker_raw);
}
