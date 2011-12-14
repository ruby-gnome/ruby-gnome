/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

