/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Sjoerd Simons
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

static VALUE
value_array_to_ruby(const GValue *from)
{
    VALUE ary;
    guint i;

    GValueArray *array = (GValueArray *)g_value_get_boxed(from);
    if (array == NULL)
        return Qnil;
  
    ary = rb_ary_new();
    for (i = 0; i < array->n_values; i++)
        rb_ary_push(ary, GVAL2RVAL(g_value_array_get_nth(array, i)));

    return ary;
}

struct value_array_from_ruby_args {
    VALUE ary;
    long n;
    GValueArray *result;
};

static VALUE
value_array_from_ruby_body(VALUE value)
{
    long i;
    struct value_array_from_ruby_args *args = (struct value_array_from_ruby_args *)value;

    for (i = 0; i < args->n; i++) {
        GValue v = G_VALUE_INIT;

        g_value_init(&v, RVAL2GTYPE(RARRAY_PTR(args->ary)[i]));
        rbgobj_rvalue_to_gvalue(RARRAY_PTR(args->ary)[i], &v);

        g_value_array_append(args->result, &v);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
value_array_from_ruby_rescue(VALUE value)
{
    g_value_array_free(((struct value_array_from_ruby_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static void
value_array_from_ruby(const VALUE from, GValue *to)
{
    struct value_array_from_ruby_args args;

    if (NIL_P(from)) {
        g_value_set_boxed(to, NULL);

        return;
    }

    args.ary = rb_ary_to_ary(from);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_value_array_new(args.n);

    rb_rescue(value_array_from_ruby_body, (VALUE)&args,
              value_array_from_ruby_rescue, (VALUE)&args);

    g_value_set_boxed(to, args.result);
}

void
Init_gobject_value_array(void)
{
    /* ValueArray is treated as Array */
    rbgobj_register_g2r_func(G_TYPE_VALUE_ARRAY, value_array_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_VALUE_ARRAY, value_array_from_ruby);
}
