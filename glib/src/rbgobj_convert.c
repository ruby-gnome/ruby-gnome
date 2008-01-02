/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   rbgobj_convert.c -

   $Author: mutoh $
   $Date: 2006/05/22 17:13:05 $

   Copyright (C) 2006 Sjoerd Simons, Masao Mutoh
************************************************/

#include "rbgobject.h"
#include "rbgprivate.h"

static GHashTable *tables;

void
Init_gobject_convert(void)
{
    tables = g_hash_table_new(g_int_hash, g_int_equal);
}

void
rbgobj_convert_define(RGConvertTable *table)
{
    g_hash_table_insert(tables, &(table->type), table);
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
    return 0; /* FIXME */
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
