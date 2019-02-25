/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2019  Ruby-GNOME2 Project Team
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

struct list2rval_args {
    gpointer list;
    RBGRValueFunc conv;
    GFreeFunc free_list;
    GFreeFunc free_elem;
};

struct list2rval_with_type_args {
    struct list2rval_args args;
    RBGRValueFuncWithType conv_with_type;
    GType gtype;
};

static VALUE
glist2rval_body(VALUE data)
{
    struct list2rval_args *args = (struct list2rval_args *)data;
    RBGRValueFunc conv = args->conv;
    GList *node;
    VALUE ary;

    ary = rb_ary_new();
    if (conv)
        for (node = args->list; node; node = g_list_next(node))
            rb_ary_push(ary, conv(node->data));

    return ary;
}

static VALUE
gslist2rval_body(VALUE data)
{
    struct list2rval_args *args = (struct list2rval_args *)data;
    RBGRValueFunc conv = args->conv;
    GSList *node;
    VALUE ary;

    ary = rb_ary_new();
    if (conv)
        for (node = args->list; node; node = g_slist_next(node))
            rb_ary_push(ary, conv(node->data));

    return ary;
}

static VALUE
glist2rval_with_type_body(VALUE data)
{
    struct list2rval_with_type_args *args_with_type = (struct list2rval_with_type_args *)data;
    struct list2rval_args *args = (struct list2rval_args *)data;
    RBGRValueFuncWithType conv = args_with_type->conv_with_type;
    GType gtype = args_with_type->gtype;
    GList *node;
    VALUE ary;

    ary = rb_ary_new();
    if (conv)
        for (node = args->list; node; node = g_list_next(node))
            rb_ary_push(ary, conv(node->data, gtype));

    return ary;
}

static VALUE
gslist2rval_with_type_body(VALUE data)
{
    struct list2rval_with_type_args *args_with_type = (struct list2rval_with_type_args *)data;
    struct list2rval_args *args = (struct list2rval_args *)data;
    RBGRValueFuncWithType conv = args_with_type->conv_with_type;
    GType gtype = args_with_type->gtype;
    GSList *node;
    VALUE ary;

    ary = rb_ary_new();
    if (conv)
        for (node = args->list; node; node = g_slist_next(node))
            rb_ary_push(ary, conv(node->data, gtype));

    return ary;
}

static VALUE
glist2rval_ensure(VALUE data)
{
    struct list2rval_args *args = (struct list2rval_args *)data;
    GList *node;

    if (args->free_elem)
        for (node = args->list; node; node = g_list_next(node))
            args->free_elem(node->data);
    if (args->free_list)
        args->free_list(args->list);

    return Qnil;
}

static VALUE
gslist2rval_ensure(VALUE data)
{
    struct list2rval_args *args = (struct list2rval_args *)data;
    GSList *node;

    if (args->free_elem)
        for (node = args->list; node; node = g_slist_next(node))
            args->free_elem(node->data);
    if (args->free_list)
        args->free_list(args->list);

    return Qnil;
}

VALUE
rbg_glist2rval(GList *const list, RBGRValueFunc conv,
               GFreeFunc free_list, GFreeFunc free_elem)
{
    struct list2rval_args args = {list, conv, free_list, free_elem};

    return rb_ensure(glist2rval_body, (VALUE)&args,
                     glist2rval_ensure, (VALUE)&args);
}

VALUE
rbg_gslist2rval(GSList *const list, RBGRValueFunc conv,
                GFreeFunc free_list, GFreeFunc free_elem)
{
    struct list2rval_args args = {list, conv, free_list, free_elem};

    return rb_ensure(gslist2rval_body, (VALUE)&args,
                     gslist2rval_ensure, (VALUE)&args);
}

VALUE
rbg_glist2rval_with_type(GList *const list, RBGRValueFuncWithType conv, GType gtype,
                         GFreeFunc free_list, GFreeFunc free_elem)
{
    struct list2rval_with_type_args args = {{list, NULL, free_list, free_elem}, conv, gtype};

    return rb_ensure(glist2rval_with_type_body, (VALUE)&args,
                     glist2rval_ensure, (VALUE)&args);
}

VALUE
rbg_gslist2rval_with_type(GSList *const list, RBGRValueFuncWithType conv, GType gtype,
                          GFreeFunc free_list, GFreeFunc free_elem)
{
    struct list2rval_with_type_args args = {{list, NULL, free_list, free_elem}, conv, gtype};

    return rb_ensure(gslist2rval_with_type_body, (VALUE)&args,
                     gslist2rval_ensure, (VALUE)&args);
}

struct rval2glist_args {
    GList *list;
    VALUE rb_array;
};

static VALUE
rval2glist_body(VALUE data)
{
    struct rval2glist_args *args = (struct rval2glist_args *)data;
    VALUE rb_array;
    long i, n;

    rb_array = rbg_to_array(args->rb_array);
    n = RARRAY_LEN(rb_array);
    for (i = 0; i < n; i++) {
        VALUE rb_element = RARRAY_CONST_PTR(rb_array)[i];
        args->list = g_list_append(args->list, RVAL2GOBJ(rb_element));
    }

    return Qnil;
}

static VALUE
rval2glist_rescue(VALUE data, VALUE e)
{
    struct rval2glist_args *args = (struct rval2glist_args *)data;

    g_list_free(args->list);
    args->list = NULL;

    rb_exc_raise(e);

    return Qnil;
}

GList *
rbg_rval2glist(VALUE rb_array)
{
    struct rval2glist_args args;
    args.list = NULL;
    args.rb_array = rb_array;

    rb_rescue(rval2glist_body, (VALUE)&args,
              rval2glist_rescue, (VALUE)&args);

    return args.list;
}

struct rval2gslist_args {
    GSList *list;
    VALUE rb_array;
};

static VALUE
rval2gslist_body(VALUE data)
{
    struct rval2gslist_args *args = (struct rval2gslist_args *)data;
    VALUE rb_array;
    long i, n;

    rb_array = rbg_to_array(args->rb_array);
    n = RARRAY_LEN(rb_array);
    for (i = 0; i < n; i++) {
        VALUE rb_element = RARRAY_CONST_PTR(rb_array)[i];
        args->list = g_slist_append(args->list, RVAL2GOBJ(rb_element));
    }

    return Qnil;
}

static VALUE
rval2gslist_rescue(VALUE data, VALUE e)
{
    struct rval2gslist_args *args = (struct rval2gslist_args *)data;

    g_slist_free(args->list);
    args->list = NULL;

    rb_exc_raise(e);

    return Qnil;
}

GSList *
rbg_rval2gslist(VALUE rb_array)
{
    struct rval2gslist_args args;
    args.list = NULL;
    args.rb_array = rb_array;

    rb_rescue(rval2gslist_body, (VALUE)&args,
              rval2gslist_rescue, (VALUE)&args);

    return args.list;
}

