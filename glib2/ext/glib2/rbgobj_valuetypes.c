/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2025  Ruby-GNOME Project Team
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
#include "rbgobject.h"

static const rb_data_type_t rbg_pointer_type = {
    "GLib::Pointer",
    {
        NULL,
        NULL,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY |
    RUBY_TYPED_WB_PROTECTED |
    RUBY_TYPED_FROZEN_SHAREABLE,
};

VALUE
rbgobj_ptr_new(GType type, gpointer ptr)
{
    return TypedData_Wrap_Struct(GTYPE2CLASS(type), &rbg_pointer_type, ptr);
}

gpointer
rbgobj_ptr2cptr(VALUE ptr)
{
    gpointer dest;
    if (RVAL2CBOOL(rb_obj_is_kind_of(ptr, GTYPE2CLASS(G_TYPE_POINTER)))) {
        TypedData_Get_Struct(ptr, void, &rbg_pointer_type, dest);
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(ptr, rb_cObject))) {
        dest = (gpointer)ptr;
    } else{
        rb_raise(rb_eTypeError, "not a pointer object");
    }
    return dest;
}

static VALUE
ptr_alloc(VALUE klass)
{
    gpointer pointer;
    return TypedData_Make_Struct(klass,
                                 gpointer,
                                 &rbg_pointer_type,
                                 pointer);
}


static VALUE
ptr_s_gtype(VALUE klass)
{
    return rbgobj_gtype_new(rbgobj_lookup_class(klass)->gtype);
}

static VALUE
ptr_initialize(VALUE self, VALUE address)
{
    RTYPEDDATA_DATA(self) = NUM2POINTER(address);
    return RUBY_Qnil;
}

static VALUE
ptr_gtype(VALUE self)
{
    return ptr_s_gtype(CLASS_OF(self));
}

static VALUE
ptr_to_i(VALUE self)
{
    return POINTER2NUM(rbgobj_ptr2cptr(self));
}

static void
Init_gtype_pointer(void)
{
    VALUE cPtr = G_DEF_CLASS(G_TYPE_POINTER, "Pointer", rbg_mGLib());
    rb_define_alloc_func(cPtr, ptr_alloc);
    rb_define_singleton_method(cPtr, "gtype", ptr_s_gtype, 0);
    rb_define_method(cPtr, "initialize", ptr_initialize, 1);
    rb_define_method(cPtr, "gtype", ptr_gtype, 0);
    rb_define_method(cPtr, "to_i", ptr_to_i, 0);
}

/**********************************************************************/

static gpointer
boxed_ruby_value_ref(gpointer boxed)
{
    VALUE value = POINTER2RVAL(boxed);
    if (!SPECIAL_CONST_P(value)) {
        rbg_gc_guard(boxed, value);
    }
    return boxed;
}

static void
boxed_ruby_value_unref(gpointer boxed)
{
    VALUE value = POINTER2RVAL(boxed);
    if (SPECIAL_CONST_P(value))
        return;
    rbg_gc_unguard(boxed);
}

struct transform_arg {
    const GValue *src_value;
    GValue       *dest_value;
};

static VALUE
value_transform_ruby_any_impl(VALUE arg_)
{
    struct transform_arg* arg = (struct transform_arg*)arg_;
    rbgobj_rvalue_to_gvalue(g_value_get_ruby_value(arg->src_value),
                            arg->dest_value);
    return Qnil;
}

static void
value_transform_ruby_any(const GValue *src_value,
                         GValue       *dest_value)
{
    int state;
    struct transform_arg arg;
    arg.src_value = src_value;
    arg.dest_value = dest_value;
    rb_protect(&value_transform_ruby_any_impl, (VALUE)&arg, &state);
}

static void
value_transform_any_ruby(const GValue *src_value,
                         GValue       *dest_value)
{
    g_value_set_ruby_value(dest_value, GVAL2RVAL(src_value));
}

GType
rbgobj_ruby_value_get_type(void)
{
  static GType our_type = 0;

  if (!our_type){
      const GType table[] = {
          G_TYPE_CHAR,
          G_TYPE_UCHAR,
          G_TYPE_BOOLEAN,
          G_TYPE_INT,
          G_TYPE_UINT,
          G_TYPE_LONG,
          G_TYPE_ULONG,
          G_TYPE_INT64,
          G_TYPE_UINT64,
          G_TYPE_ENUM,
          G_TYPE_FLAGS,
          G_TYPE_FLOAT,
          G_TYPE_DOUBLE,
          G_TYPE_STRING,
          G_TYPE_POINTER,
          //G_TYPE_BOXED,
          G_TYPE_PARAM,
          G_TYPE_OBJECT,
      };
      size_t i;

      our_type = g_boxed_type_register_static(
          "VALUE",
          (GBoxedCopyFunc)boxed_ruby_value_ref,
          (GBoxedFreeFunc)boxed_ruby_value_unref);

      for (i = 0; i < sizeof(table)/sizeof(table[0]); i++){
          g_value_register_transform_func(table[i], our_type,
                                          value_transform_any_ruby);
      }

      g_value_register_transform_func(our_type, G_TYPE_BOOLEAN,
                                      value_transform_ruby_any);
  }
  return our_type;
}

VALUE
g_value_get_ruby_value(const GValue* value)
{
    return POINTER2RVAL(g_value_get_boxed(value));
}

void
g_value_set_ruby_value(GValue* value, VALUE ruby)
{
    g_value_set_boxed(value, RVAL2POINTER(ruby));
}

static void
ruby_value_r2g(VALUE from, GValue* to)
{
    g_value_set_ruby_value(to, from);
}

static void
Init_boxed_ruby_value(void)
{
    rbgobj_register_g2r_func(RBGOBJ_TYPE_RUBY_VALUE, g_value_get_ruby_value);
    rbgobj_register_r2g_func(RBGOBJ_TYPE_RUBY_VALUE, ruby_value_r2g);
}

/**********************************************************************/

void
Init_gobject_gvaluetypes(void)
{
    Init_gtype_pointer();
    Init_boxed_ruby_value();
}
