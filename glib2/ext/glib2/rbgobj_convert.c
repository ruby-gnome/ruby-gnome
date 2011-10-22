/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgobject.h"
#include "rbgprivate.h"

static GHashTable *tables, *class_to_g_type_map;

void
Init_gobject_convert(void)
{
    tables = g_hash_table_new(g_int_hash, g_int_equal);
    class_to_g_type_map = g_hash_table_new(g_int_hash, g_int_equal);
}

void
rbgobj_convert_define(RGConvertTable *table)
{
    g_hash_table_insert(tables, &(table->type), table);
    if (table->klass && !NIL_P(table->klass)) {
        g_hash_table_insert(class_to_g_type_map,
                            &(table->klass), &(table->type));
    }
}

RGConvertTable *
rbgobj_convert_lookup(GType type)
{
    return g_hash_table_lookup(tables, &type);
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
        *result = table->get_superclass();
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
        table->type_init_hook(klass);
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
        table->rvalue2gvalue(value, result);
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
        *result = table->gvalue2rvalue(value);
        return TRUE;
    }

    return FALSE;
}

GType
rbgobj_convert_rvalue2gtype(VALUE value)
{
    VALUE klass;
    GType *result;

    klass = rb_class_of(value);
    result = g_hash_table_lookup(class_to_g_type_map, &klass);
    return result ? *result : 0;
}

gboolean
rbgobj_convert_initialize(GType type, VALUE obj, gpointer cobj)
{
    RGConvertTable *table;

    table = rbgobj_convert_lookup(type);
    if (table && table->initialize) {
        table->initialize(obj, cobj);
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
        *result = table->robj2instance(obj);
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
        *result = table->instance2robj(instance);
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
        table->unref(instance);
        return TRUE;
    }

    return FALSE;
}
