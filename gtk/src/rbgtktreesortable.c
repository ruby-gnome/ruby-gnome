/*****************************************************************************
 *
 * rbgtktreesortable.c: GtkTreeSortable interface.
 *
 * Copyright (C) 2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2003-03-27
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "global.h"

/* Defines *******************************************************************/

#define _SELF(s)	(GTK_TREE_SORTABLE(RVAL2GOBJ(s)))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static VALUE
treesortable_sort_column_changed(self)
	VALUE self;
{
	gtk_tree_sortable_sort_column_changed(_SELF(self));
	return self;
}

static VALUE
treesortable_get_sort_column_id(self)
	VALUE self;
{
	gint sort_column_id;
	GtkSortType order;

	if (gtk_tree_sortable_get_sort_column_id(_SELF(self), &sort_column_id,
						 &order)) {
		VALUE ary;

		ary = rb_ary_new2(2);
		rb_ary_push(ary, INT2NUM(sort_column_id));
		rb_ary_push(ary, INT2NUM(order));
		return ary;
	} else {
		return Qnil;	/* XXX: or something else? */
	}
}

static VALUE
treesortable_set_sort_column_id(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	gint sort_column_id;
	GtkSortType order;

	if (argc == 1 || argc == 2) {
		sort_column_id = NUM2INT(argv[0]);
		order = (argc == 2) ? NUM2INT(argv[1]) : GTK_SORT_ASCENDING;
	} else {
		rb_raise(rb_eArgError, "need 1 or 2 arguments.");
	}

	gtk_tree_sortable_set_sort_column_id(_SELF(self), sort_column_id,
					     order);
	return self;
}

/* XXX: this is stolen from rbgtktreeiter.c: what to do?  export it there? */
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
static gint
sort_func(model, a, b, func)
	GtkTreeModel *model;
	GtkTreeIter *a, *b;
	gpointer func;
{
	a->user_data3 = model;
	b->user_data3 = model;
	return NUM2INT(rb_funcall((VALUE)func, id_call, 2, ITR2RVAL(a),
				 ITR2RVAL(b)));
}

static VALUE
treesortable_set_sort_func(self, sort_column_id)
	VALUE self, sort_column_id;
{
	volatile VALUE func = rb_f_lambda();
	G_RELATIVE(self, func);
	gtk_tree_sortable_set_sort_func(_SELF(self), NUM2INT(sort_column_id),
					(GtkTreeIterCompareFunc)sort_func,
					(gpointer)func, NULL);
	return self;
}

static VALUE
treesortable_set_default_sort_func(self)
	VALUE self;
{
	volatile VALUE func = rb_f_lambda();
	G_RELATIVE(self, func);
	gtk_tree_sortable_set_default_sort_func(_SELF(self),
					(GtkTreeIterCompareFunc)sort_func,
					(gpointer)func, NULL);
	return self;
}

static VALUE
treesortable_has_default_sort_func(self)
	VALUE self;
{
	return RTEST(gtk_tree_sortable_has_default_sort_func(_SELF(self)));
}

void
Init_gtk_treesortable()
{
	VALUE mts = G_DEF_INTERFACE(GTK_TYPE_TREE_SORTABLE, "TreeSortable",
				    mGtk);

	rb_define_method(mts, "sort_column_changed",
			 treesortable_sort_column_changed, 0);
	rb_define_method(mts, "sort_column_id",
			 treesortable_get_sort_column_id, 0);
	rb_define_method(mts, "set_sort_column_id",
			 treesortable_set_sort_column_id, -1);
	rb_define_method(mts, "set_sort_func",
			 treesortable_set_sort_func, 1);
	rb_define_method(mts, "set_default_sort_func",
			 treesortable_set_default_sort_func, 0);
	rb_define_method(mts, "has_default_sort_func?",
			 treesortable_has_default_sort_func, 0);

	G_DEF_SETTERS(mts);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
