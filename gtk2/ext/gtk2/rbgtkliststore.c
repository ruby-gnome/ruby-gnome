/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "global.h"

#define RG_TARGET_NAMESPACE cListStore
#define _SELF(s) (GTK_LIST_STORE(RVAL2GOBJ(s)))

static ID id_to_a, id_size;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    gint cnt;
    GtkListStore* store;
    GType* buf;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");

    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    store = gtk_list_store_newv(argc, buf);

    G_INITIALIZE(self, store);

    return Qnil;
}

static VALUE
rg_set_column_types(int argc, VALUE *argv, VALUE self)
{
    gint cnt;
    GType* buf;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");
    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    gtk_list_store_set_column_types(_SELF(self), argc, buf);

    return self;
}

static VALUE
rg_set_value(VALUE self, VALUE iter, VALUE column, VALUE value)
{
    GType gtype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(RVAL2GOBJ(self)), NUM2INT(column));
    GValue gval = G_VALUE_INIT;
    g_value_init(&gval, gtype);

    rbgobj_rvalue_to_gvalue(value, &gval);

    G_CHILD_ADD(iter, value);

    gtk_list_store_set_value(_SELF(self), RVAL2GTKTREEITER(iter), NUM2INT(column), &gval);

    g_value_unset(&gval);
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
typedef struct _ValuesInfo
{
    gint i;
    VALUE iter;
    GtkTreeModel *model;
    gint *g_columns;
    GValue *g_values;
} ValuesInfo;

static int
collect_value(VALUE key, VALUE value, ValuesInfo *info)
{
    GType g_type;
    gint column;

    column = NUM2INT(key);
    info->g_columns[info->i] = column;
    g_type = gtk_tree_model_get_column_type(info->model, column);
    g_value_init(&(info->g_values[info->i]), g_type);
    rbgobj_rvalue_to_gvalue(value, &(info->g_values[info->i]));
    G_CHILD_ADD(info->iter, value);
    info->i++;

    return ST_CONTINUE;
}

static void
hash_to_values(VALUE hash, VALUE iter, GtkTreeModel *model,
               gint *g_columns, GValue *g_values, G_GNUC_UNUSED gint length)
{
    ValuesInfo info;

    info.i = 0;
    info.iter = iter;
    info.model = model;
    info.g_columns = g_columns;
    info.g_values = g_values;
    rb_hash_foreach(hash, collect_value, (VALUE)(&info));
}

static void
array_to_values(VALUE array, VALUE iter, GtkTreeModel *model,
                gint *g_columns, GValue *g_values, gint length)
{
    gint i;

    for (i = 0; i < length; i++) {
        GType g_type;

        g_columns[i] = i;
        g_type = gtk_tree_model_get_column_type(model, i);
        g_value_init(&g_values[i], g_type);
        rbgobj_rvalue_to_gvalue(RARRAY_PTR(array)[i], &g_values[i]);
        G_CHILD_ADD(iter, RARRAY_PTR(array)[i]);
    }
}

static VALUE
rg_set_values(VALUE self, VALUE iter, VALUE values)
{
    GValue *g_values;
    GtkListStore *store;
    GtkTreeModel *model;
    gint *g_columns;
    gint i, length;

    length = NUM2INT(rb_funcall(values, id_size, 0));
    if (length == 0)
        return self;

    g_columns = ALLOCA_N(gint, length);
    g_values = ALLOCA_N(GValue, length);
    MEMZERO(g_values, GValue, length);

    store = _SELF(self);
    model = GTK_TREE_MODEL(store);
    if (RVAL2CBOOL(rb_obj_is_kind_of(values, rb_cHash))) {
        hash_to_values(values, iter, model, g_columns, g_values, length);
    }
    else if (RVAL2CBOOL(rb_obj_is_kind_of(values, rb_cArray))) {
        array_to_values(values, iter, model, g_columns, g_values, length);
    }
    else {
        rb_raise(rb_eArgError, "must be array or hash of values");
    }

    gtk_list_store_set_valuesv(store, RVAL2GTKTREEITER(iter),
                               g_columns, g_values, length);

    for (i = 0; i < length; i++)
        g_value_unset(&g_values[i]);

    return self;
}
#endif

/*
  void        gtk_tree_store_set (GtkTreeStore *tree_store,
  GtkTreeIter *iter,
  ...);

  void        gtk_tree_store_set_valist       (GtkTreeStore *tree_store,
  GtkTreeIter *iter,
  va_list var_args);
*/

static VALUE
rg_remove(VALUE self, VALUE iter)
{
    G_CHILD_REMOVE(self, iter);
#if GTK_CHECK_VERSION(2,2,0)
    return CBOOL2RVAL(gtk_list_store_remove(_SELF(self), RVAL2GTKTREEITER(iter)));
#else
    gtk_list_store_remove(_SELF(self), RVAL2GTKTREEITER(iter));
    return Qtrue;
#endif
}

struct lstore_insert_args {
    GtkListStore *store;
    GtkTreeIter iter;
    gint position;
    VALUE ary;
    long i;
    long n;
    gint *columns;
    GValue *values;
};

static VALUE
lstore_insert_body(VALUE value)
{
    struct lstore_insert_args *args = (struct lstore_insert_args *)value;
    GtkTreeModel *model = GTK_TREE_MODEL(args->store);

    for (args->i = 0; args->i < args->n; args->i++) {
        VALUE ary = rb_ary_to_ary(RARRAY_PTR(args->ary)[args->i]);

        args->columns[args->i] = NUM2INT(RARRAY_PTR(ary)[1]);
        g_value_init(&args->values[args->i],
                     gtk_tree_model_get_column_type(model, args->columns[args->i]));
        rbgobj_rvalue_to_gvalue(RARRAY_PTR(ary)[0], &args->values[args->i]);
    }

    gtk_list_store_insert_with_valuesv(args->store,
                                       &args->iter,
                                       args->position,
                                       args->columns,
                                       args->values,
                                       args->n);

    return Qnil;
}

static VALUE
lstore_insert_ensure(VALUE value)
{
    long i;
    struct lstore_insert_args *args = (struct lstore_insert_args *)value;

    for (i = 0; i < args->i; i++)
        g_value_unset(&args->values[i]);

    g_free(args->values);
    g_free(args->columns);

    return Qnil;
}

/*
  Gtk::ListStore#insert(pos, val1 => 0, val2 => 2, ... )
 */
static VALUE
rg_insert(int argc, VALUE *argv, VALUE self)
{
    VALUE position, values, result;
    struct lstore_insert_args args;
    args.store = _SELF(self);

    rb_scan_args(argc, argv, "11", &position, &values);
    args.position = NUM2INT(position);

    if (NIL_P(values)){
        gtk_list_store_insert(args.store, &args.iter, args.position);
    } else {
#if GTK_CHECK_VERSION(2,6,0)
        args.ary = rb_funcall(values, id_to_a, 0);
        args.n = RARRAY_LEN(args.ary);
        args.columns = g_new(gint, args.n);
        args.values = g_new0(GValue, args.n);

        rb_ensure(lstore_insert_body, (VALUE)&args,
                  lstore_insert_ensure, (VALUE)&args);
#else
        gtk_list_store_insert(args.store, &args.iter, args.position);
        rb_warn("Ignored 2nd argument under this environment, as it has been supported since GTK+-2.6.");
#endif
    }

    args.iter.user_data3 = args.store;

    result = GTKTREEITER2RVAL(&args.iter);

    return result;
}

static VALUE
rg_insert_before(VALUE self, VALUE sibling)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_insert_before(model, &iter, NIL_P(sibling) ? NULL : RVAL2GTKTREEITER(sibling));
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    return ret;
}

static VALUE
rg_insert_after(VALUE self, VALUE sibling)
{ 
    VALUE ret;
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_insert_after(model, &iter, NIL_P(sibling) ? NULL : RVAL2GTKTREEITER(sibling));
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    return ret;
}

static VALUE
rg_prepend(VALUE self)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_prepend(model, &iter);
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    return ret;
}

static VALUE
rg_append(VALUE self)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_append(model, &iter);
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    return ret;
}

static VALUE
rg_clear(VALUE self)
{
    G_CHILD_REMOVE_ALL(self);
    gtk_list_store_clear(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_iter_is_valid_p(VALUE self, VALUE iter)
{
    return (NIL_P(iter)) ? Qfalse :
        CBOOL2RVAL(gtk_list_store_iter_is_valid(_SELF(self), RVAL2GTKTREEITER(iter)));
}

static VALUE
rg_reorder(VALUE self, VALUE rbnew_order)
{
    GtkListStore *store = _SELF(self);
    long n;
    gint *new_order = RVAL2GINTS(rbnew_order, n);

    gtk_list_store_reorder(store, new_order);

    g_free(new_order);

    return self;
}
static VALUE
rg_swap(VALUE self, VALUE iter1, VALUE iter2)
{
    gtk_list_store_swap(_SELF(self), RVAL2GTKTREEITER(iter1), RVAL2GTKTREEITER(iter2));
    return self;
}
static VALUE
rg_move_before(VALUE self, VALUE iter, VALUE position)
{
    gtk_list_store_move_before(_SELF(self), RVAL2GTKTREEITER(iter), 
                               NIL_P(position) ? NULL : RVAL2GTKTREEITER(position));
    return self;
}
static VALUE
rg_move_after(VALUE self, VALUE iter, VALUE position)
{
    gtk_list_store_move_after(_SELF(self), RVAL2GTKTREEITER(iter), 
                               NIL_P(position) ? NULL : RVAL2GTKTREEITER(position));
    return self;
}
#endif

void
Init_gtk_list_store(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_LIST_STORE, "ListStore", mGtk);

    id_to_a = rb_intern("to_a");
    id_size = rb_intern("size");

    rbgtk_register_treeiter_set_value_func(GTK_TYPE_LIST_STORE, 
                                           (rbgtkiter_set_value_func)&gtk_list_store_set_value);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_column_types, -1);
    RG_DEF_METHOD(set_value, 3);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(insert, -1);
    RG_DEF_METHOD(insert_before, 1);
    RG_DEF_METHOD(insert_after, 1);
    RG_DEF_METHOD(prepend, 0);
    RG_DEF_METHOD(append, 0);
    RG_DEF_METHOD(clear, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD_P(iter_is_valid, 1);
    RG_DEF_METHOD(reorder, 1);
    RG_DEF_METHOD(swap, 2);
    RG_DEF_METHOD(move_before, 2);
    RG_DEF_METHOD(move_after, 2);
#endif
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(set_values, 2);
#endif

}
