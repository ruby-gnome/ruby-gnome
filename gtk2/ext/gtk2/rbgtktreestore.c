/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTreeStore
#define _SELF(s) (GTK_TREE_STORE(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    gint cnt;
    GtkTreeStore* store;
    GType* buf;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");

    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    store = gtk_tree_store_newv(argc, buf);

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

    gtk_tree_store_set_column_types(_SELF(self), argc, buf);

    return self;
}

static VALUE
rg_set_value(VALUE self, VALUE iter, VALUE column, VALUE value)
{
    GType gtype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(RVAL2GOBJ(self)), NUM2INT(column));
    GValue gval = G_VALUE_INIT;
    g_value_init(&gval, gtype);

    rbgobj_rvalue_to_gvalue(value, &gval);

    G_CHILD_ADD(self, iter);
    G_CHILD_ADD(iter, value);

    gtk_tree_store_set_value(_SELF(self), RVAL2GTKTREEITER(iter), NUM2INT(column), &gval);

    g_value_unset(&gval);
    return self;
}

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
    return CBOOL2RVAL(gtk_tree_store_remove(_SELF(self), RVAL2GTKTREEITER(iter)));
#else
    gtk_tree_store_remove(_SELF(self), RVAL2GTKTREEITER(iter));
    return Qtrue;
#endif
}

static VALUE
rg_insert(int argc, VALUE *argv, VALUE self)
{
    VALUE parent, position, values, ret;
    GtkTreeIter iter;
    GtkTreeStore* model = _SELF(self);

    rb_scan_args(argc, argv, "21", &parent, &position, &values);

    if (NIL_P(values)){
        gtk_tree_store_insert(model, &iter, 
                              NIL_P(parent) ? NULL : RVAL2GTKTREEITER(parent), 
                              NUM2INT(position));
        iter.user_data3 = model;
        ret = GTKTREEITER2RVAL(&iter);
        G_CHILD_ADD(self, ret);
    } else {
#if GTK_CHECK_VERSION(2,10,0)
        gint *c_columns;
        GValue *c_values;
        long size, i;

        size = NUM2INT(rb_funcall(values, rb_intern("size"), 0));
        c_columns = ALLOCA_N(gint, size);
        c_values = ALLOCA_N(GValue, size);

        if(TYPE(values)==T_ARRAY) {
            for(i=0; i<size; i++) {
                GType gtype;
                GValue gval = G_VALUE_INIT;

                c_columns[i] = i;
                gtype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(RVAL2GOBJ(self)),
                                                       c_columns[i]);

                g_value_init(&gval, gtype);
                rbgobj_rvalue_to_gvalue(rb_ary_shift(values), &gval);
                c_values[i] = gval;
            }
        } else if(TYPE(values)==T_HASH) {
            VALUE r_columns;
            r_columns = rb_funcall(values, rb_intern("keys"), 0);

            for(i=0; i<size; i++) {
                GType gtype;
                GValue gval = G_VALUE_INIT;

                c_columns[i] = NUM2INT (rb_ary_entry(r_columns, i));
                gtype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(RVAL2GOBJ(self)),
                                                       c_columns[i]);

                g_value_init(&gval, gtype);
                rbgobj_rvalue_to_gvalue(rb_hash_aref(values, INT2NUM(c_columns[i])), &gval);
                c_values[i] = gval;
            }
        } else {
            rb_raise(rb_eArgError, "values must be of type Hash or Array");
        }

        gtk_tree_store_insert_with_valuesv(model, &iter,
                                           NIL_P(parent) ? NULL : RVAL2GTKTREEITER(parent),
                                           NUM2INT(position),
                                           c_columns,
                                           c_values,
                                           size);
        iter.user_data3 = model;

        ret = GTKTREEITER2RVAL(&iter);
        G_CHILD_ADD(self, ret);

        for(i=0; i<size; i++) {
            G_CHILD_ADD(ret, rbgobj_gvalue_to_rvalue(&(c_values[i])));
            g_value_unset(&(c_values[i]));
        }
#else
        rb_warn("Gtk::TreeStore#insert(parent, position, values) requires GTK+-2.10.0 or later");
        gtk_tree_store_insert(model, &iter, 
                              NIL_P(parent) ? NULL : RVAL2GTKTREEITER(parent), 
                              NUM2INT(position));
        iter.user_data3 = model;
        ret = GTKTREEITER2RVAL(&iter);
        G_CHILD_ADD(self, ret);
#endif
    }

    return ret;
}

static VALUE
rg_insert_before(VALUE self, VALUE parent, VALUE sibling)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkTreeStore* model = _SELF(self);
    gtk_tree_store_insert_before(model, &iter, 
                                 NIL_P(parent) ? NULL : RVAL2GTKTREEITER(parent), 
                                 NIL_P(sibling) ? NULL : RVAL2GTKTREEITER(sibling));
    iter.user_data3 = model;
    ret = GTKTREEITER2RVAL(&iter);
    G_CHILD_ADD(self, ret);
    return ret;
}

static VALUE
rg_insert_after(VALUE self, VALUE parent, VALUE sibling)
{ 
    VALUE ret;
    GtkTreeIter iter;
    GtkTreeStore* model = _SELF(self);
    gtk_tree_store_insert_after(model, &iter, 
                                NIL_P(parent) ? NULL : RVAL2GTKTREEITER(parent), 
                                NIL_P(sibling) ? NULL : RVAL2GTKTREEITER(sibling));
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    G_CHILD_ADD(self, ret);
    return ret;
}

/* The 2 following functions where implemented as TreeStore#insert
   which will use a Hash instead of pairs of parameters

void        gtk_tree_store_insert_with_values
                                            (GtkTreeStore *tree_store,
                                             GtkTreeIter *iter,
                                             GtkTreeIter *parent,
                                             gint position,
                                             ...);

void        gtk_tree_store_insert_with_valuesv
                                            (GtkTreeStore *tree_store,
                                             GtkTreeIter *iter,
                                             GtkTreeIter *parent,
                                             gint position,
                                             gint *columns,
                                             GValue *values,
                                             gint n_values);

*/

static VALUE
rg_prepend(VALUE self, VALUE parent)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkTreeStore* model = _SELF(self);
    gtk_tree_store_prepend(model, &iter, 
                           NIL_P(parent)?NULL:RVAL2GTKTREEITER(parent));
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    G_CHILD_ADD(self, ret);
    return ret;
}

static VALUE
rg_append(VALUE self, VALUE parent)
{
    VALUE ret;
    GtkTreeIter iter;
    GtkTreeStore* model = _SELF(self);
    gtk_tree_store_append(model, &iter, 
                          NIL_P(parent)?NULL:RVAL2GTKTREEITER(parent));
    iter.user_data3 = model;

    ret = GTKTREEITER2RVAL(&iter);
    G_CHILD_ADD(self, ret);
    return ret;
}

static VALUE
rg_ancestor_p(VALUE self, VALUE iter, VALUE descendant)
{
    return gtk_tree_store_is_ancestor(_SELF(self), RVAL2GTKTREEITER(iter), 
                                      RVAL2GTKTREEITER(descendant));
}

static VALUE
rg_iter_depth(VALUE self, VALUE iter)
{
    return INT2NUM(gtk_tree_store_iter_depth(_SELF(self), RVAL2GTKTREEITER(iter)));
}

static VALUE
rg_clear(VALUE self)
{
    G_CHILD_REMOVE_ALL(self);
    gtk_tree_store_clear(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_iter_is_valid_p(VALUE self, VALUE iter)
{
    return (NIL_P(iter)) ? Qfalse : 
        CBOOL2RVAL(gtk_tree_store_iter_is_valid(_SELF(self), RVAL2GTKTREEITER(iter)));
}

static VALUE
rg_reorder(VALUE self, VALUE rbparent, VALUE rbnew_order)
{
    GtkTreeStore *store = _SELF(self);
    GtkTreeIter *parent = RVAL2GTKTREEITER(rbparent);
    gint n_children = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(store),
                                                     parent);
    long n;
    gint *new_order = RVAL2GINTS(rbnew_order, n);

    if (n != n_children) {
        g_free(new_order);

        rb_raise(rb_eArgError,
                 "new order array must contain the same number of elements as the number of children in the parent: %ld != %d",
                 n, n_children);
    }

    gtk_tree_store_reorder(store, parent, new_order);

    g_free(new_order);

    return self;
}

static VALUE
rg_swap(VALUE self, VALUE iter1, VALUE iter2)
{
    gtk_tree_store_swap(_SELF(self), RVAL2GTKTREEITER(iter1), RVAL2GTKTREEITER(iter2));
    return self;
}

static VALUE
rg_move_before(VALUE self, VALUE iter, VALUE position)
{
    gtk_tree_store_move_before(_SELF(self), RVAL2GTKTREEITER(iter), 
                               NIL_P(position) ? NULL : RVAL2GTKTREEITER(position));
    return self;
}

static VALUE
rg_move_after(VALUE self, VALUE iter, VALUE position)
{
    gtk_tree_store_move_after(_SELF(self), RVAL2GTKTREEITER(iter), 
                               NIL_P(position) ? NULL : RVAL2GTKTREEITER(position));
    return self;
}
#endif

void
Init_gtk_tree_store(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_STORE, "TreeStore", mGtk);

    rbgtk_register_treeiter_set_value_func(GTK_TYPE_TREE_STORE, 
                                           (rbgtkiter_set_value_func)&gtk_tree_store_set_value);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_column_types, -1);
    RG_DEF_METHOD(set_value, 3);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(insert, -1);
    RG_DEF_METHOD(insert_before, 2);
    RG_DEF_METHOD(insert_after, 2);
    RG_DEF_METHOD(prepend, 1);
    RG_DEF_METHOD(append, 1);
    RG_DEF_METHOD_P(ancestor, 2);
    RG_DEF_METHOD(iter_depth, 1);
    RG_DEF_METHOD(clear, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD_P(iter_is_valid, 1);
    RG_DEF_METHOD(reorder, 2);
    RG_DEF_METHOD(swap, 2);
    RG_DEF_METHOD(move_before, 2);
    RG_DEF_METHOD(move_after, 2);
#endif

}
