/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_valuetypes.c -

  $Author: sakai $
  $Date: 2003/08/06 02:14:03 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"
#ifdef RBGOBJ_USE_DLPTR
#include "dl.h"
#endif


VALUE
rbgobj_ptr_new(type, ptr)
    GType type;
    gpointer ptr;
{
#ifdef RBGOBJ_USE_DLPTR
    return rb_dlptr_new2(GTYPE2CLASS(type), 0, NULL);
#else
    return Data_Wrap_Struct(GTYPE2CLASS(type), NULL, NULL, ptr);
#endif
}

gpointer
rbgobj_ptr2cptr(ptr)
    VALUE ptr;
{
#ifdef RBGOBJ_USE_DLPTR
    return rb_dlptr2cptr(ptr);
#else
    gpointer dest;
    if (rb_obj_is_kind_of(ptr, GTYPE2CLASS(G_TYPE_POINTER))){
        Data_Get_Struct(ptr, void, dest);
    } else if (rb_obj_is_kind_of(ptr, rb_cObject)){
        dest = (gpointer)ptr;
    } else{
        rb_raise(rb_eTypeError, "not a pointer object");
    }
    return dest;
#endif
}

static VALUE
ptr_s_gtype(klass)
    VALUE klass;
{
    return rbgobj_gtype_new(rbgobj_lookup_class(klass)->gtype);
}

static VALUE
ptr_gtype(self)
    VALUE self;
{
    return ptr_s_gtype(CLASS_OF(self));
}

#ifndef RBGOBJ_USE_DLPTR
static VALUE
ptr_inspect(self)
    VALUE self;
{
    gpointer ptr;
    gchar* s;
    VALUE result;

    Data_Get_Struct(self, void, ptr);

    s = g_strdup_printf("#<%s:%p ptr=%p>",
                        rb_class2name(CLASS_OF(self)),
                        (void *)self,
                        ptr);

    result = rb_str_new2(s);
    g_free(s);

    return result;
}

static VALUE
ptr_eql(self, other)
    VALUE self, other;
{
    gpointer ptr1, ptr2;
    if (!rb_obj_is_kind_of(other, GTYPE2CLASS(G_TYPE_POINTER)))
        return Qnil;
    Data_Get_Struct(self, void, ptr1);
    Data_Get_Struct(other, void, ptr2);
    return (ptr1==ptr2) ? Qtrue : Qfalse;
}

static VALUE
ptr_hash(self)
    VALUE self;
{
    gpointer ptr;
    Data_Get_Struct(self, void, ptr);
    return INT2FIX((long)ptr);
}
#endif

static void
Init_gtype_pointer()
{
    VALUE cPtr = G_DEF_CLASS(G_TYPE_POINTER, "Pointer", mGLib);
    rb_define_singleton_method(cPtr, "gtype", ptr_s_gtype, 1);
    rb_define_method(cPtr, "gtype", ptr_gtype, 1);
#ifndef RBGOBJ_USE_DLPTR
    rb_define_method(cPtr, "inspect", ptr_inspect, 0);
    rb_define_method(cPtr, "==", ptr_eql, 1);
    rb_define_method(cPtr, "eql?", ptr_eql, 1);
    rb_define_method(cPtr, "hash", ptr_hash, 0);
#endif
}

/**********************************************************************/

static GHashTable* boxed_ruby_value_table;
static VALUE boxed_ruby_value_table_wrapper;

typedef struct {
    VALUE obj;
    guint ref_count;
} boxed_ruby_value_counter;

static void
boxed_ruby_value_counter_mark(gpointer       key,
                              gpointer       value,
                              gpointer       user_data)
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

GType rbgobj_ruby_value_get_type()
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
      int i;

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
Init_boxed_ruby_value()
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
Init_gobject_gvaluetypes()
{
    Init_gtype_pointer();
    Init_boxed_ruby_value();
}
