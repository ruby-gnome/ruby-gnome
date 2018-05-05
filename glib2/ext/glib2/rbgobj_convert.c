/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2018  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Sjoerd Simons, Masao Mutoh
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

static GHashTable *tables, *class_to_g_type_map;

static void
rg_convert_table_free(gpointer data)
{
    RGConvertTable *table = data;

    if (table->notify) {
        table->notify(table->user_data);
    }
    g_free(table);
}

void
Init_gobject_convert(void)
{
    /* TODO: unref the below tables on exit. */
    tables = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL,
                                   rg_convert_table_free);
    class_to_g_type_map = g_hash_table_new(g_direct_hash, g_direct_equal);
}

void
rbgobj_convert_define(const RGConvertTable *table)
{
    RGConvertTable *copied_table;
    copied_table = g_memdup(table, sizeof(RGConvertTable));
    g_hash_table_insert(tables,
                        GUINT_TO_POINTER(copied_table->type),
                        copied_table);
    if (copied_table->klass != Qfalse && !NIL_P(copied_table->klass)) {
        g_hash_table_insert(class_to_g_type_map,
                            GUINT_TO_POINTER(copied_table->klass),
                            GUINT_TO_POINTER(copied_table->type));
    }
}

RGConvertTable *
rbgobj_convert_lookup(GType type)
{
    return g_hash_table_lookup(tables, GUINT_TO_POINTER(type));
}

gboolean
rbgobj_convert_has_type(GType type)
{
    return rbgobj_convert_lookup(type) != NULL;
}

gboolean
rbgobj_convert_get_superclass(GType type, VALUE *result)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->get_superclass) {
        *result = table->get_superclass(table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_type_init_hook(GType type, VALUE klass)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->type_init_hook) {
        table->type_init_hook(klass, table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_rvalue2gvalue(GType type, VALUE value, GValue *result)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->rvalue2gvalue) {
        table->rvalue2gvalue(value, result, table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_gvalue2rvalue(GType type, const GValue *value, VALUE *result)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->gvalue2rvalue) {
        *result = table->gvalue2rvalue(value, table->user_data);
        return TRUE;
    }

    return FALSE;
}

GType
rbgobj_convert_rvalue2gtype(VALUE value)
{
    VALUE klass;
    gpointer result;

    klass = rb_class_of(value);
    result = g_hash_table_lookup(class_to_g_type_map, GUINT_TO_POINTER(klass));
    return GPOINTER_TO_UINT(result);
}

gboolean
rbgobj_convert_initialize(GType type, VALUE obj, gpointer cobj)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->initialize) {
        table->initialize(obj, cobj, table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_robj2instance(GType type, VALUE obj, gpointer *result)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->robj2instance) {
        *result = table->robj2instance(obj, table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_instance2robj(GType type, gpointer instance, VALUE *result)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->instance2robj) {
        *result = table->instance2robj(instance, table->user_data);
        return TRUE;
    }

    return FALSE;
}

gboolean
rbgobj_convert_unref(GType type, gpointer instance)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->unref) {
        table->unref(instance, table->user_data);
        return TRUE;
    }

    return FALSE;
}
