/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

/**********************************************************************/
static VALUE rb_cGLibValue;

#define RG_TARGET_NAMESPACE rb_cGLibValue
#define _SELF(self) RVAL2GVALUE(self)

static ID id_to_s;
static GQuark qRValueToGValueFunc;
static GQuark qGValueToRValueFunc;

gboolean
rbg_is_value(VALUE object)
{
    return RVAL2CBOOL(rb_obj_is_kind_of(object, RG_TARGET_NAMESPACE));
}

void
rbgobj_register_r2g_func(GType gtype, RValueToGValueFunc func)
{
    g_type_set_qdata(gtype, qRValueToGValueFunc, func);
}

void
rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func)
{
    g_type_set_qdata(gtype, qGValueToRValueFunc, func);
}

/**********************************************************************/

VALUE
rbgobj_gvalue_to_rvalue(const GValue* value)
{
    GType type, fundamental_type;
    VALUE rvalue;

    if (!value)
        return Qnil;

    type = G_VALUE_TYPE(value);
    if (rbgobj_convert_gvalue2rvalue(type, value, &rvalue))
        return rvalue;

    fundamental_type = G_TYPE_FUNDAMENTAL(type);
    switch (fundamental_type) {
      case G_TYPE_NONE:
        return Qnil;
      case G_TYPE_CHAR:
        return CHR2FIX(g_value_get_schar(value));
      case G_TYPE_UCHAR:
        return INT2FIX(g_value_get_uchar(value));
      case G_TYPE_BOOLEAN:
        return CBOOL2RVAL(g_value_get_boolean(value));
      case G_TYPE_INT:
        return INT2NUM(g_value_get_int(value));
      case G_TYPE_UINT:
        return UINT2NUM(g_value_get_uint(value));
      case G_TYPE_LONG:
        return LONG2NUM(g_value_get_long(value));
      case G_TYPE_ULONG:
        return ULONG2NUM(g_value_get_ulong(value));
      case G_TYPE_INT64:
        return rbglib_int64_to_num(g_value_get_int64(value));
      case G_TYPE_UINT64:
        return rbglib_uint64_to_num(g_value_get_uint64(value));
      case G_TYPE_FLOAT:
        return rb_float_new(g_value_get_float(value));
      case G_TYPE_DOUBLE:
        return rb_float_new(g_value_get_double(value));
      case G_TYPE_STRING:
        return CSTR2RVAL(g_value_get_string(value));
      case G_TYPE_ENUM:
        return rbgobj_make_enum(g_value_get_enum(value), type);
      case G_TYPE_FLAGS:
        return rbgobj_make_flags(g_value_get_flags(value), type);
      case G_TYPE_OBJECT:
      case G_TYPE_INTERFACE:
        {
            GObject* gobj = g_value_get_object(value);
            return gobj ? GOBJ2RVAL(gobj) : Qnil;
        }
      case G_TYPE_PARAM:
        {
            GParamSpec* pspec = g_value_get_param(value);
            return pspec ? rbgobj_ruby_object_from_instance(pspec) : Qnil;
        }
      case G_TYPE_POINTER:
        {
            gpointer ptr = g_value_get_pointer(value);
            if (!ptr)
                return Qnil;
            else
                return rbgobj_ptr_new(type, ptr);
        }

      case G_TYPE_BOXED:
        {
            GType gtype;
            for (gtype = type;
                 gtype != G_TYPE_INVALID;
                 gtype = g_type_parent(gtype))
            {
                GValueToRValueFunc func =
                    g_type_get_qdata(gtype, qGValueToRValueFunc);
                if (!func)
                    continue;
                return func(value);
            }
        }
      case G_TYPE_VARIANT:
        {
            GVariant *variant = g_value_peek_pointer(value);
            rvalue = rbg_variant_to_ruby(variant);
            return rvalue;
        }
      default:
        if (!rbgobj_convert_gvalue2rvalue(fundamental_type, value, &rvalue)) {
            GValueToRValueFunc func;
            func = g_type_get_qdata(type, qGValueToRValueFunc);
            if (!func) {
                g_warning("rbgobj_gvalue_to_rvalue: unsupported type: %s\n",
                          g_type_name(type));
            } else {
                rvalue = func(value);
            }
        }
        return rvalue;
    }
}

static VALUE
rbgobj_gvalue_to_rvalue_unset_body(VALUE value)
{
    return GVAL2RVAL((GValue *)value);
}

static VALUE
rbgobj_gvalue_to_rvalue_unset_ensure(VALUE value)
{
    g_value_unset((GValue *)value);

    return Qnil;
}

VALUE
rbgobj_gvalue_to_rvalue_unset(GValue *value)
{
    return rb_ensure(rbgobj_gvalue_to_rvalue_unset_body, (VALUE)value,
                     rbgobj_gvalue_to_rvalue_unset_ensure, (VALUE)value);
}

void
rbgobj_initialize_gvalue(GValue *result, VALUE value)
{
    GType type;

    type = rbgobj_convert_rvalue2gtype(value);
    if (type == 0) {
        switch (TYPE(value)) {
          case T_NONE:
          case T_NIL:
            type = G_TYPE_NONE;
            break;
          case T_FLOAT:
            type = G_TYPE_DOUBLE;
            break;
          case T_STRING:
          case T_SYMBOL:
            type = G_TYPE_STRING;
            break;
          case T_FIXNUM:
            type = G_TYPE_INT;
            break;
          case T_BIGNUM:
            type = G_TYPE_INT64;
            break;
          case T_TRUE:
          case T_FALSE:
            type = G_TYPE_BOOLEAN;
            break;
          default:
	    if (RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_cEnum))) {
		type = G_TYPE_ENUM;
	    }
	    else if (RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_cFlags)) ||
                     RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_cBoxed)) ||
                     RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_cObject))) {
		type = RVAL2GTYPE(value);
	    }
	    else if (RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_cParam))) {
		type = G_TYPE_PARAM;
	    }
	    else if (RVAL2CBOOL(rb_obj_is_kind_of(value, rbgobj_mInterface))) {
		/* should use rbgobj_mMetaInterface? */
		type = G_TYPE_INTERFACE;
	    }
	    else {
		VALUE inspected_value;
		inspected_value = rb_funcall(value, rb_intern("inspect"), 0);
		rb_raise(rb_eArgError,
			 "unsupported value type: %s",
			 RSTRING_PTR(inspected_value));
	    }
	    break;
        }
    }

    g_value_init(result, type);
    rbgobj_rvalue_to_gvalue(value, result);
}

void
rbgobj_rvalue_to_gvalue(VALUE val, GValue* result)
{
    GType type, fundamental_type;

    type = G_VALUE_TYPE(result);
    if (rbgobj_convert_rvalue2gvalue(type, val, result))
        return;

    fundamental_type = G_TYPE_FUNDAMENTAL(type);
    switch (fundamental_type) {
      case G_TYPE_NONE:
        return;
      case G_TYPE_CHAR:
        g_value_set_schar(result, NUM2INT(val));
        return;
      case G_TYPE_UCHAR:
        g_value_set_uchar(result, NUM2UINT(val));
        return;
      case G_TYPE_BOOLEAN:
        g_value_set_boolean(result, RVAL2CBOOL(val));
        return;
      case G_TYPE_INT:
        g_value_set_int(result, NUM2INT(val));
        return;
      case G_TYPE_UINT:
        g_value_set_uint(result, NUM2UINT(val));
        return;
      case G_TYPE_LONG:
        g_value_set_long(result, NUM2LONG(val));
        return;
      case G_TYPE_ULONG:
        g_value_set_ulong(result, NUM2ULONG(val));
        return;
      case G_TYPE_INT64:
        g_value_set_int64(result, rbglib_num_to_int64(val));
        return;
      case G_TYPE_UINT64:
        g_value_set_uint64(result, rbglib_num_to_uint64(val));
        return;
      case G_TYPE_ENUM:
        g_value_set_enum(result, rbgobj_get_enum(val, G_VALUE_TYPE(result)));
        return;
      case G_TYPE_FLAGS:
        g_value_set_flags(result, rbgobj_get_flags(val, G_VALUE_TYPE(result)));
        return;
      case G_TYPE_FLOAT:
        g_value_set_float(result, NUM2DBL(val));
        return;
      case G_TYPE_DOUBLE:
        g_value_set_double(result, NUM2DBL(val));
        return;
      case G_TYPE_STRING:
        {
            if (SYMBOL_P(val))
                val = rb_funcall(val, id_to_s, 0);
            g_value_set_string(result, RVAL2CSTR_ACCEPT_NIL(val));
            return;
        }
      case G_TYPE_OBJECT:
      case G_TYPE_INTERFACE:
        g_value_set_object(result, NIL_P(val) ? NULL : RVAL2GOBJ(val));
        return;
      case G_TYPE_PARAM:
        g_value_set_param(result, NIL_P(val) ? NULL : RVAL2GOBJ(val));
        return;
      case G_TYPE_POINTER:
        g_value_set_pointer(result, NIL_P(val) ? NULL : rbgobj_ptr2cptr(val));
        return;
      case G_TYPE_BOXED:
        {
            GType gtype;
            for (gtype = type;
                 gtype != G_TYPE_INVALID;
                 gtype = g_type_parent(gtype))
            {
                RValueToGValueFunc func =
                    g_type_get_qdata(gtype, qRValueToGValueFunc);
                if (!func)
                    continue;
                func(val, result);
                return;
            }
        }
      case G_TYPE_VARIANT:
        g_value_set_variant(result, rbg_variant_from_ruby(val));
        break;
      default:
        if (!rbgobj_convert_rvalue2gvalue(fundamental_type, val, result)) {
            RValueToGValueFunc func =
                g_type_get_qdata(type, qRValueToGValueFunc);
            if (!func){
                g_warning("rbgobj_rvalue_to_gvalue: unsupported type: %s\n",
                          g_type_name(type));
            } else {
                func(val, result);
            }
        }
    }
}

/**********************************************************************/

void
rbgobj_gc_mark_gvalue(GValue* value)
{
    GType gtype = G_VALUE_TYPE(value);
    /* FIXME */
    if (G_TYPE_FUNDAMENTAL(gtype) == G_TYPE_OBJECT)
        rbgobj_gc_mark_instance(g_value_get_object(value));
}

/**********************************************************************/

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GValue value = G_VALUE_INIT;
    VALUE rb_gtype;
    VALUE rb_value;

    rb_scan_args(argc, argv, "11", &rb_gtype, &rb_value);

    g_value_init(&value, NUM2ULONG(rb_to_int(rb_gtype)));
    if (argc == 2) {
        rbgobj_rvalue_to_gvalue(rb_value, &value);
    }
    G_INITIALIZE(self, g_boxed_copy(G_TYPE_VALUE, &value));
    g_value_unset(&value);

    return Qnil;
}

static VALUE
rg_type(VALUE self)
{
    GValue *value;

    value = _SELF(self);

    return rbgobj_gtype_new(value->g_type);
}

static VALUE
rg_value(VALUE self)
{
    GValue *value;

    value = _SELF(self);

    return GVAL2RVAL(value);
}

static VALUE
rg_to_s(VALUE self)
{
    GValue *value;
    gchar *string_value;

    value = _SELF(self);
    string_value = g_strdup_value_contents(value);
    return CSTR2RVAL_FREE(string_value);
}

void
Init_gobject_gvalue(void)
{
    id_to_s = rb_intern("to_s");
    qRValueToGValueFunc = g_quark_from_static_string("__ruby_r2g_func__");
    qGValueToRValueFunc = g_quark_from_static_string("__ruby_g2r_func__");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VALUE, "Value", mGLib);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(value, 0);
    RG_DEF_METHOD(to_s, 0);
}
