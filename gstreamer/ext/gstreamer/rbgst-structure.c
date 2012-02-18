/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cStructure
#define SELF(obj) (RVAL2GST_STRUCT(obj))

static VALUE
rg_s_parse(VALUE self, VALUE string)
{
    GstStructure *structure;
    gchar *end;

    structure = gst_structure_from_string(RVAL2CSTR(string), &end);

    return rb_ary_new3(2, GST_STRUCT2RVAL(structure), CSTR2RVAL(end));
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, fields;
    GstStructure *structure;

    rb_scan_args(argc, argv, "11", &name, &fields);

    structure = gst_structure_empty_new(RVAL2CSTR(name));
    if (!NIL_P(fields)) {
        /* set fields */
    }

    G_INITIALIZE(self, structure);

    return Qnil;
}

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(gst_structure_get_name(SELF(self)));
}

static VALUE
rg_set_name(VALUE self, VALUE name)
{
    gst_structure_set_name(SELF(self), RVAL2CSTR(name));
    return Qnil;
}

static VALUE
rg_has_name_p(VALUE self, VALUE name)
{
    return CBOOL2RVAL(gst_structure_has_name(SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_operator_get_value(VALUE self, VALUE name)
{
    return GVAL2RVAL(gst_structure_get_value(SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_operator_set_value(VALUE self, VALUE name, VALUE rb_value)
{
    GValue value = G_VALUE_INIT;

    rbgobj_initialize_gvalue(&value, rb_value);
    gst_structure_set_value(SELF(self), RVAL2CSTR(name), &value);
    g_value_unset(&value);

    return Qnil;
}

static VALUE
rg_remove(int argc, VALUE *argv, VALUE self)
{
    int i;
    VALUE name, rest;
    GstStructure *structure;

    rb_scan_args(argc, argv, "1*", &name, &rest);

    structure = SELF(self);
    for (i = 0; i < argc; i++) {
        name = argv[i];

        gst_structure_remove_field(structure, RVAL2CSTR(name));
    }

    return Qnil;
}

static VALUE
rg_remove_all(VALUE self)
{
    gst_structure_remove_all_fields(SELF(self));
    return Qnil;
}

typedef struct _EachCallbackInfo
{
    int state;
    VALUE proc;
} EachCallbackInfo;

typedef struct _EachCallbackData
{
    VALUE proc;
    VALUE key;
    VALUE value;
} EachCallbackData;

typedef struct _RValueToGValueData
{
    VALUE rvalue;
    GValue *gvalue;
} RValueToGValueData;

static VALUE
invoke_proc_func(VALUE user_data)
{
    EachCallbackData *data = (EachCallbackData *)user_data;

    return rb_funcall(data->proc,
                      rb_intern("call"),
                      2,
                      data->key,
                      data->value);
}

static VALUE
invoke_proc(GQuark field_id, const GValue *value,
            EachCallbackInfo *info)
{
    EachCallbackData data;

    data.proc = info->proc;
    data.key = rb_str_new2(g_quark_to_string(field_id));
    data.value = GVAL2RVAL(value);
    return rb_protect(invoke_proc_func, (VALUE)(&data), &(info->state));
}

static gboolean
foreach_cb(GQuark field_id, const GValue *value, gpointer user_data)
{
    EachCallbackInfo *info = user_data;

    invoke_proc(field_id, value, info);

    return info->state == 0;
}

static VALUE
rg_each(VALUE self)
{
    EachCallbackInfo info;

    info.state = 0;
    info.proc = rb_block_proc();

    if (!gst_structure_foreach(SELF(self), foreach_cb, &info))
        rb_jump_tag(info.state);

    return Qnil;
}

static VALUE
invoke_rvalue_to_gvalue(VALUE user_data)
{
    RValueToGValueData *data = (RValueToGValueData *)user_data;

    g_value_unset(data->gvalue);
    rbgobj_initialize_gvalue(data->gvalue, data->rvalue);

    return Qnil;
}

static gboolean
map_in_place_cb(GQuark field_id, GValue *value, gpointer user_data)
{
    EachCallbackInfo *info = user_data;
    VALUE result;

    result = invoke_proc(field_id, value, info);
    if (info->state == 0) {
        RValueToGValueData convert_data;
        convert_data.rvalue = result;
        convert_data.gvalue = value;
        rb_protect(invoke_rvalue_to_gvalue,
                   (VALUE)(&convert_data), &(info->state));
    }

    return info->state == 0;
}

static VALUE
rg_map_bang(VALUE self)
{
    EachCallbackInfo info;

    info.state = 0;
    info.proc = rb_block_proc();

    if (!gst_structure_map_in_place(SELF(self), map_in_place_cb, &info))
        rb_jump_tag(info.state);

    return Qnil;
}

static VALUE
rg_size(VALUE self)
{
    return INT2NUM(gst_structure_n_fields(SELF(self)));
}

static VALUE
rg_empty_p(VALUE self)
{
    return CBOOL2RVAL(gst_structure_n_fields(SELF(self)) == 0);
}

static VALUE
rg_nth_field_name(VALUE self, VALUE index)
{
    return CSTR2RVAL(gst_structure_nth_field_name(SELF(self), NUM2INT(index)));
}

static VALUE
rg_has_field_p(VALUE self, VALUE name)
{
    return CBOOL2RVAL(gst_structure_has_field(SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_to_s(VALUE self)
{
    return CSTR2RVAL_FREE(gst_structure_to_string(SELF(self)));
}

void
Init_gst_structure(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_STRUCTURE, "Structure", mGst);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_SMETHOD(parse, 1);

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(set_name, 1);
    RG_DEF_METHOD_P(has_name, 1);
    RG_DEF_ALIAS("have_name?", "has_name?");

    RG_DEF_METHOD_OPERATOR("[]", get_value, 1);
    RG_DEF_METHOD_OPERATOR("[]=", set_value, 2);

    RG_DEF_METHOD(remove, -1);
    RG_DEF_ALIAS("delete", "remove");
    RG_DEF_METHOD(remove_all, 0);
    RG_DEF_ALIAS("clear", "remove_all");

    RG_DEF_METHOD(each, 0);

    RG_DEF_METHOD_BANG(map, 0);
    RG_DEF_ALIAS("collect!", "map!");

    RG_DEF_METHOD(size, 0);
    RG_DEF_ALIAS("length", "size");
    RG_DEF_METHOD_P(empty, 0);

    RG_DEF_METHOD(nth_field_name, 1);

    RG_DEF_METHOD_P(has_field, 1);
    RG_DEF_ALIAS("have_field?", "has_field?");

    RG_DEF_METHOD(to_s, 0);
}
