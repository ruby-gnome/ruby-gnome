/*
 * Copyright (C) 2008 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

static VALUE cIntRange, cFourcc, cFractionRange;

static RGConvertTable value_list_table = {0};
static RGConvertTable value_array_table = {0};
static RGConvertTable int_range_table = {0};
static RGConvertTable fourcc_table = {0};
static RGConvertTable fraction_table = {0};
static RGConvertTable fraction_range_table = {0};


static void
value_list_rvalue2gvalue(VALUE value, GValue *result)
{
    guint i, len;

    len = RARRAY_LEN(value);
    for (i = 0; i < len; i++) {
        GValue val = {0};
        rbgobj_initialize_gvalue(&val, RARRAY_PTR(value)[i]);
        gst_value_list_append_value(result, &val);
        g_value_unset(&val);
    }
}

static VALUE
value_list_gvalue2rvalue(const GValue *value)
{
    guint i, len;
    VALUE result;

    len = gst_value_list_get_size(value);
    result = rb_ary_new2(len);
    for (i = 0; i < len; i++) {
        rb_ary_push(result, GVAL2RVAL(gst_value_list_get_value(value, i)));
    }

    return result;
}


static void
value_array_rvalue2gvalue(VALUE value, GValue *result)
{
    guint i, len;

    len = RARRAY_LEN(value);
    for (i = 0; i < len; i++) {
        GValue val = {0};
        rbgobj_initialize_gvalue(&val, RARRAY_PTR(value)[i]);
        gst_value_array_append_value(result, &val);
        g_value_unset(&val);
    }
}

static VALUE
value_array_gvalue2rvalue(const GValue *value)
{
    guint i, len;
    VALUE result;

    len = gst_value_array_get_size(value);
    result = rb_ary_new2(len);
    for (i = 0; i < len; i++) {
        rb_ary_push(result, GVAL2RVAL(gst_value_array_get_value(value, i)));
    }

    return result;
}


static GValue *
g_value_new(GType type)
{
    GValue *value;

    value = ALLOC(GValue);
    MEMZERO(value, GValue, 1);
    g_value_init(value, type);

    return value;
}

static void
g_value_free(gpointer instance)
{
    if (instance) {
        g_value_unset(instance);
        g_free(instance);
    }
}

static VALUE
g_value_to_ruby_value(const GValue *value)
{
    VALUE klass;
    GType type;
    GValue *copied_value;

    type = G_VALUE_TYPE(value);
    klass = GTYPE2CLASS(type);
    copied_value = g_value_new(type);
    g_value_copy(value, copied_value);
    return Data_Wrap_Struct(klass, NULL, g_value_free, copied_value);
}

static VALUE
g_value_type_instance_to_ruby_object(gpointer instance)
{
    return g_value_to_ruby_value(instance);
}

static void
g_value_type_unref(gpointer instance)
{
    g_value_unset(instance);
}

#define DEF_G_VALUE_CONVERTERS(prefix, g_type, type)            \
static gpointer                                                 \
prefix ## _robj2instance(VALUE object)                          \
{                                                               \
    gpointer instance;                                          \
                                                                \
    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, c ## type))) {    \
        rb_raise(rb_eTypeError, "not a Gst::" # type);          \
    }                                                           \
    Data_Get_Struct(object, GValue, instance);                  \
    return instance;                                            \
}                                                               \
                                                                \
static VALUE                                                    \
prefix ## _allocate(VALUE klass)                                \
{                                                               \
    return Data_Wrap_Struct(klass, NULL,                        \
                            g_value_free, g_value_new(g_type)); \
}

static VALUE
g_value_to_s(VALUE self)
{
    return CSTR2RVAL2(g_strdup_value_contents(RVAL2GOBJ(self)));
}



static VALUE
int_range_initialize(VALUE self, VALUE min, VALUE max)
{
    gst_value_set_int_range(DATA_PTR(self), NUM2INT(min), NUM2INT(max));
    return Qnil;
}

static VALUE
int_range_get_min(VALUE self)
{
    return INT2NUM(gst_value_get_int_range_min(RVAL2GOBJ(self)));
}

static VALUE
int_range_set_min(VALUE self, VALUE min)
{
    GValue *value;

    value = RVAL2GOBJ(self);
    gst_value_set_int_range(value,
                            NUM2INT(min),
                            gst_value_get_int_range_max(value));
    return Qnil;
}

static VALUE
int_range_get_max(VALUE self)
{
    return INT2NUM(gst_value_get_int_range_max(RVAL2GOBJ(self)));
}

static VALUE
int_range_set_max(VALUE self, VALUE max)
{
    GValue *value;

    value = RVAL2GOBJ(self);
    gst_value_set_int_range(value,
                            gst_value_get_int_range_min(value),
                            NUM2INT(max));
    return Qnil;
}

static VALUE
int_range_set(VALUE self, VALUE min, VALUE max)
{
    gst_value_set_int_range(RVAL2GOBJ(self), NUM2INT(min), NUM2INT(max));
    return Qnil;
}

static VALUE
int_range_to_a(VALUE self)
{
    GValue *value;

    value = RVAL2GOBJ(self);
    return rb_ary_new3(2,
                       INT2NUM(gst_value_get_int_range_min(value)),
                       INT2NUM(gst_value_get_int_range_max(value)));
}

static VALUE
int_range_get_superclass(void)
{
    return rb_cObject;
}

static void
int_range_rvalue2gvalue(VALUE value, GValue *result)
{
    GValue *val;

    val = RVAL2GOBJ(value);
    gst_value_set_int_range(result,
                            gst_value_get_int_range_min(val),
                            gst_value_get_int_range_max(val));
}

DEF_G_VALUE_CONVERTERS(int_range, GST_TYPE_INT_RANGE, IntRange)


static VALUE
fourcc_get_superclass(void)
{
    return rb_cObject;
}

static void
fourcc_rvalue2gvalue(VALUE value, GValue *result)
{
    gst_value_set_fourcc(result,
                         gst_value_get_fourcc(RVAL2GOBJ(value)));
}

DEF_G_VALUE_CONVERTERS(fourcc, GST_TYPE_FOURCC, Fourcc)

static guint32
value_to_fourcc(VALUE value)
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(value, rb_cString))) {
        return GST_STR_FOURCC(RSTRING_PTR(value));
    } else {
        return NUM2UINT(value);
    }
}

static VALUE
fourcc_initialize(VALUE self, VALUE fourcc)
{
    gst_value_set_fourcc(DATA_PTR(self), value_to_fourcc(fourcc));
    return Qnil;
}

static VALUE
fourcc_replace_bang(VALUE self, VALUE fourcc)
{
    gst_value_set_fourcc(DATA_PTR(self), value_to_fourcc(fourcc));
    return Qnil;
}

static VALUE
fourcc_to_i(VALUE self)
{
    return UINT2NUM(gst_value_get_fourcc(DATA_PTR(self)));
}


static void
fraction_rvalue2gvalue(VALUE value, GValue *result)
{
    gst_value_set_fraction(result,
                           NUM2INT(rb_funcall(value, rb_intern("numerator"), 0)),
                           NUM2INT(rb_funcall(value, rb_intern("denominator"),
                                              0)));
}

static VALUE
fraction_gvalue2rvalue(const GValue *value)
{
    return rb_funcall(Qnil, rb_intern("Rational"), 2,
                      INT2NUM(gst_value_get_fraction_numerator(value)),
                      INT2NUM(gst_value_get_fraction_denominator(value)));
}


static VALUE
fraction_range_initialize(VALUE self, VALUE min, VALUE max)
{
    GValue min_value = {0}, max_value = {0};

    rbgobj_initialize_gvalue(&min_value, min);
    rbgobj_initialize_gvalue(&max_value, max);
    gst_value_set_fraction_range(DATA_PTR(self), &min_value, &max_value);
    return Qnil;
}

static VALUE
fraction_range_get_min(VALUE self)
{
    return GVAL2RVAL(gst_value_get_fraction_range_min(RVAL2GOBJ(self)));
}

static VALUE
fraction_range_set_min(VALUE self, VALUE min)
{
    GValue *value;
    GValue min_value = {0};

    value = RVAL2GOBJ(self);
    rbgobj_initialize_gvalue(&min_value, min);
    gst_value_set_fraction_range(value,
                                 &min_value,
                                 gst_value_get_fraction_range_max(value));
    return Qnil;
}

static VALUE
fraction_range_get_max(VALUE self)
{
    return GVAL2RVAL(gst_value_get_fraction_range_max(RVAL2GOBJ(self)));
}

static VALUE
fraction_range_set_max(VALUE self, VALUE max)
{
    GValue *value;
    GValue max_value = {0};

    value = RVAL2GOBJ(self);
    rbgobj_initialize_gvalue(&max_value, max);
    gst_value_set_fraction_range(value,
                                 gst_value_get_fraction_range_min(value),
                                 &max_value);
    return Qnil;
}

static VALUE
fraction_range_set(VALUE self, VALUE min, VALUE max)
{
    GValue min_value = {0}, max_value = {0};

    rbgobj_initialize_gvalue(&min_value, min);
    rbgobj_initialize_gvalue(&max_value, max);
    gst_value_set_fraction_range(RVAL2GOBJ(self), &min_value, &max_value);
    return Qnil;
}

static VALUE
fraction_range_to_a(VALUE self)
{
    GValue *value;

    value = RVAL2GOBJ(self);
    return rb_ary_new3(2,
                       GVAL2RVAL(gst_value_get_fraction_range_min(value)),
                       GVAL2RVAL(gst_value_get_fraction_range_max(value)));
}

static VALUE
fraction_range_get_superclass(void)
{
    return rb_cObject;
}

static void
fraction_range_rvalue2gvalue(VALUE value, GValue *result)
{
    GValue *val;

    val = RVAL2GOBJ(value);
    gst_value_set_fraction_range(result,
                                 gst_value_get_fraction_range_min(val),
                                 gst_value_get_fraction_range_max(val));
}

DEF_G_VALUE_CONVERTERS(fraction_range, GST_TYPE_FRACTION_RANGE, FractionRange)


void
Init_gst_value(void)
{
    value_list_table.type = GST_TYPE_LIST;
    value_list_table.rvalue2gvalue = value_list_rvalue2gvalue;
    value_list_table.gvalue2rvalue = value_list_gvalue2rvalue;

    RG_DEF_CONVERSION(&value_list_table);


    value_array_table.type = GST_TYPE_ARRAY;
    value_array_table.rvalue2gvalue = value_array_rvalue2gvalue;
    value_array_table.gvalue2rvalue = value_array_gvalue2rvalue;

    RG_DEF_CONVERSION(&value_array_table);


    int_range_table.type = GST_TYPE_INT_RANGE;
    int_range_table.get_superclass = int_range_get_superclass;
    int_range_table.type_init_hook = NULL;
    int_range_table.rvalue2gvalue = int_range_rvalue2gvalue;
    int_range_table.gvalue2rvalue = g_value_to_ruby_value;
    int_range_table.initialize = NULL;
    int_range_table.robj2instance = int_range_robj2instance;
    int_range_table.instance2robj = g_value_type_instance_to_ruby_object;
    int_range_table.unref = g_value_type_unref;

    RG_DEF_CONVERSION(&int_range_table);

    cIntRange = G_DEF_CLASS(GST_TYPE_INT_RANGE, "IntRange", mGst);

    rb_define_alloc_func(cIntRange, int_range_allocate);

    rb_define_method(cIntRange, "initialize", int_range_initialize, 2);

    rb_define_method(cIntRange, "min", int_range_get_min, 0);
    rb_define_method(cIntRange, "set_min", int_range_set_min, 1);
    rb_define_method(cIntRange, "max", int_range_get_max, 0);
    rb_define_method(cIntRange, "set_max", int_range_set_max, 1);

    rb_define_method(cIntRange, "set", int_range_set, 2);

    rb_define_method(cIntRange, "to_a", int_range_to_a, 0);
    rb_define_method(cIntRange, "to_s", g_value_to_s, 0);

    G_DEF_SETTERS(cIntRange);


    fourcc_table.type = GST_TYPE_FOURCC;
    fourcc_table.get_superclass = fourcc_get_superclass;
    fourcc_table.type_init_hook = NULL;
    fourcc_table.rvalue2gvalue = fourcc_rvalue2gvalue;
    fourcc_table.gvalue2rvalue = g_value_to_ruby_value;
    fourcc_table.initialize = NULL;
    fourcc_table.robj2instance = fourcc_robj2instance;
    fourcc_table.instance2robj = g_value_type_instance_to_ruby_object;
    fourcc_table.unref = g_value_type_unref;

    RG_DEF_CONVERSION(&fourcc_table);

    cFourcc = G_DEF_CLASS(GST_TYPE_FOURCC, "Fourcc", mGst);

    rb_define_alloc_func(cFourcc, fourcc_allocate);

    rb_define_method(cFourcc, "initialize", fourcc_initialize, 1);

    rb_define_method(cFourcc, "replace!", fourcc_replace_bang, 1);

    rb_define_method(cFourcc, "to_i", fourcc_to_i, 0);
    rb_define_method(cFourcc, "to_s", g_value_to_s, 0);

    G_DEF_SETTERS(cFourcc);


    fraction_table.type = GST_TYPE_FRACTION;
    fraction_table.klass = rb_const_get(rb_cObject, rb_intern("Rational"));
    fraction_table.rvalue2gvalue = fraction_rvalue2gvalue;
    fraction_table.gvalue2rvalue = fraction_gvalue2rvalue;

    RG_DEF_CONVERSION(&fraction_table);


    fraction_range_table.type = GST_TYPE_FRACTION_RANGE;
    fraction_range_table.get_superclass = fraction_range_get_superclass;
    fraction_range_table.type_init_hook = NULL;
    fraction_range_table.rvalue2gvalue = fraction_range_rvalue2gvalue;
    fraction_range_table.gvalue2rvalue = g_value_to_ruby_value;
    fraction_range_table.initialize = NULL;
    fraction_range_table.robj2instance = fraction_range_robj2instance;
    fraction_range_table.instance2robj = g_value_type_instance_to_ruby_object;
    fraction_range_table.unref = g_value_type_unref;

    RG_DEF_CONVERSION(&fraction_range_table);

    cFractionRange = G_DEF_CLASS(GST_TYPE_FRACTION_RANGE, "FractionRange", mGst);

    rb_define_alloc_func(cFractionRange, fraction_range_allocate);

    rb_define_method(cFractionRange, "initialize", fraction_range_initialize, 2);

    rb_define_method(cFractionRange, "min", fraction_range_get_min, 0);
    rb_define_method(cFractionRange, "set_min", fraction_range_set_min, 1);
    rb_define_method(cFractionRange, "max", fraction_range_get_max, 0);
    rb_define_method(cFractionRange, "set_max", fraction_range_set_max, 1);

    rb_define_method(cFractionRange, "set", fraction_range_set, 2);

    rb_define_method(cFractionRange, "to_a", fraction_range_to_a, 0);
    rb_define_method(cFractionRange, "to_s", g_value_to_s, 0);

    G_DEF_SETTERS(cFractionRange);
}
