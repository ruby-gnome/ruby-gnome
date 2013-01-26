/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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


VALUE
rbgobj_ptr_new(GType type, gpointer ptr)
{
    return Data_Wrap_Struct(GTYPE2CLASS(type), NULL, NULL, ptr);
}

gpointer
rbgobj_ptr2cptr(VALUE ptr)
{
    gpointer dest;
    if (rb_obj_is_kind_of(ptr, GTYPE2CLASS(G_TYPE_POINTER))){
        Data_Get_Struct(ptr, void, dest);
    } else if (rb_obj_is_kind_of(ptr, rb_cObject)){
        dest = (gpointer)ptr;
    } else{
        rb_raise(rb_eTypeError, "not a pointer object");
    }
    return dest;
}

static VALUE
ptr_s_gtype(VALUE klass)
{
    return rbgobj_gtype_new(rbgobj_lookup_class(klass)->gtype);
}

static VALUE
ptr_gtype(VALUE self)
{
    return ptr_s_gtype(CLASS_OF(self));
}

static void
Init_gtype_pointer(void)
{
    VALUE cPtr = G_DEF_CLASS(G_TYPE_POINTER, "Pointer", mGLib);
    rbg_define_singleton_method(cPtr, "gtype", ptr_s_gtype, 1);
    rbg_define_method(cPtr, "gtype", ptr_gtype, 1);
}

/**********************************************************************/

static GHashTable* boxed_ruby_value_table;
static VALUE boxed_ruby_value_table_wrapper;

typedef struct {
    VALUE obj;
    guint ref_count;
} boxed_ruby_value_counter;

static void
boxed_ruby_value_counter_mark(G_GNUC_UNUSED gpointer key,
                              gpointer value,
                              G_GNUC_UNUSED gpointer user_data)
{
    boxed_ruby_value_counter* counter = value;
    if (counter->ref_count)
        rb_gc_mark(counter->obj);
}

static void
boxed_ruby_value_table_mark(GHashTable* table)
{
    g_hash_table_foreach(table, boxed_ruby_value_counter_mark, NULL);
}

static VALUE
boxed_ruby_value_ref(VALUE val)
{
    if (!SPECIAL_CONST_P(val)){
        boxed_ruby_value_counter* counter;

        counter = g_hash_table_lookup(boxed_ruby_value_table, (gpointer)val);

        if (!counter){
            counter = g_new(boxed_ruby_value_counter, 1);
            counter->obj       = val;
            counter->ref_count = 1;
            g_hash_table_insert(boxed_ruby_value_table, (gpointer)val,
                                counter);
        } else {
            counter->ref_count += 1;
        }
    }
    return val;
}

static void
boxed_ruby_value_unref(VALUE val)
{
    if (!SPECIAL_CONST_P(val)){
        boxed_ruby_value_counter* counter;

        counter = g_hash_table_lookup(boxed_ruby_value_table, (gpointer)val);
        counter->ref_count -= 1;

        if (!counter->ref_count)
            g_hash_table_remove(boxed_ruby_value_table, (gpointer)val);
    }
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
    return (VALUE)g_value_get_boxed(value);
}

void
g_value_set_ruby_value(GValue* value, VALUE ruby)
{
    g_value_set_boxed(value, (gconstpointer)ruby);
}

static void
ruby_value_r2g(VALUE from, GValue* to)
{
    g_value_set_ruby_value(to, from);
}

static void
Init_boxed_ruby_value(void)
{
    boxed_ruby_value_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);

    boxed_ruby_value_table_wrapper =
      Data_Wrap_Struct(rb_cData,
                       boxed_ruby_value_table_mark, NULL,
                       boxed_ruby_value_table);
    rb_global_variable(&boxed_ruby_value_table_wrapper);

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
