/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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

#include "rbatkprivate.h"

#define _SELF(s) (ATK_TABLE(RVAL2GOBJ(s)))

static VALUE
rbatk_table_ref_at(VALUE self, VALUE row, VALUE column)
{
    return GOBJ2RVAL(atk_table_ref_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_index_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_index_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_column_at_index(VALUE self, VALUE index_)
{
    return INT2NUM(atk_table_get_column_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rbatk_table_get_row_at_index(VALUE self, VALUE index_)
{
    return INT2NUM(atk_table_get_row_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rbatk_table_get_n_columns(VALUE self)
{
    return INT2NUM(atk_table_get_n_columns(_SELF(self)));
}

static VALUE
rbatk_table_get_n_rows(VALUE self)
{
    return INT2NUM(atk_table_get_n_rows(_SELF(self)));
}

static VALUE
rbatk_table_get_column_extent_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_column_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_extent_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_row_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_caption(VALUE self)
{
    return GOBJ2RVAL(atk_table_get_caption(_SELF(self)));
}

static VALUE
rbatk_table_get_column_description(VALUE self, VALUE column)
{
    return CSTR2RVAL(atk_table_get_column_description(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_description(VALUE self, VALUE row)
{
    return CSTR2RVAL(atk_table_get_row_description(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_get_column_header(VALUE self, VALUE column)
{
    return GOBJ2RVAL(atk_table_get_column_header(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_header(VALUE self, VALUE row)
{
    return GOBJ2RVAL(atk_table_get_row_header(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_get_summary(VALUE self)
{
    return GOBJ2RVAL(atk_table_get_summary(_SELF(self)));
}

static VALUE
rbatk_table_set_caption(VALUE self, VALUE caption)
{
    atk_table_set_caption(_SELF(self), ATK_OBJECT(RVAL2GOBJ(caption)));
    return self;
}

static VALUE
rbatk_table_set_row_description(VALUE self, VALUE row, VALUE description)
{
    atk_table_set_row_description(_SELF(self), NUM2INT(row), RVAL2CSTR(description));
    return self;
}

static VALUE
rbatk_table_set_column_description(VALUE self, VALUE column, VALUE description)
{
    atk_table_set_column_description(_SELF(self), NUM2INT(column), RVAL2CSTR(description));
    return self;
}

static VALUE
rbatk_table_set_row_header(VALUE self, VALUE row, VALUE header)
{
    atk_table_set_row_header(_SELF(self), NUM2INT(row), ATK_OBJECT(RVAL2GOBJ(header)));
    return self;
}

static VALUE
rbatk_table_set_column_header(VALUE self, VALUE column, VALUE header)
{
    atk_table_set_column_header(_SELF(self), NUM2INT(column), ATK_OBJECT(RVAL2GOBJ(header)));
    return self;
}

static VALUE
rbatk_table_set_summary(VALUE self, VALUE accessible)
{
    atk_table_set_summary(_SELF(self), ATK_OBJECT(RVAL2GOBJ(accessible)));
    return self;
}

static VALUE
rbatk_table_get_selected_columns(VALUE self)
{
    gint ret;
    gint* selected;
    VALUE result = Qnil;
    ret = atk_table_get_selected_columns(_SELF(self), &selected);
    if (ret > 0){
        gint i;
        result = rb_ary_new2(ret);
        for (i = 0; i < ret; i++){
            rb_ary_push(result, INT2NUM(selected[i]));
        }
    }
    return result;
}

static VALUE
rbatk_table_get_selected_rows(VALUE self)
{
    gint ret;
    gint* selected;
    VALUE result = Qnil;
    ret = atk_table_get_selected_rows(_SELF(self), &selected);
    if (ret > 0){
        gint i;
        result = rb_ary_new2(ret);
        for (i = 0; i < ret; i++){
            rb_ary_push(result, INT2NUM(selected[i]));
        }
    }
    return result;
}

static VALUE
rbatk_table_is_column_selected(VALUE self, VALUE column)
{
    return CBOOL2RVAL(atk_table_is_column_selected(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_is_row_selected(VALUE self, VALUE row)
{
    return CBOOL2RVAL(atk_table_is_row_selected(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_is_selected(VALUE self, VALUE row, VALUE column)
{
    return CBOOL2RVAL(atk_table_is_selected(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_add_column_selection(VALUE self, VALUE column)
{
    gboolean ret = atk_table_add_column_selection(_SELF(self), NUM2INT(column));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rbatk_table_add_row_selection(VALUE self, VALUE row)
{
    gboolean ret = atk_table_add_row_selection(_SELF(self), NUM2INT(row));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rbatk_table_remove_column_selection(VALUE self, VALUE column)
{
    gboolean ret = atk_table_remove_column_selection(_SELF(self), NUM2INT(column));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rbatk_table_remove_row_selection(VALUE self, VALUE row)
{
    gboolean ret = atk_table_remove_row_selection(_SELF(self), NUM2INT(row));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

void
Init_atk_table(void)
{
    VALUE mTable = G_DEF_INTERFACE(ATK_TYPE_TABLE, "Table", mAtk);

    rb_define_method(mTable, "ref_at", rbatk_table_ref_at, 2);
    rb_define_method(mTable, "get_index_at", rbatk_table_get_index_at, 2);
    rb_define_method(mTable, "get_column_at_index", rbatk_table_get_column_at_index, 1);
    rb_define_method(mTable, "get_row_at_index", rbatk_table_get_row_at_index, 1);
    rb_define_method(mTable, "n_columns", rbatk_table_get_n_columns, 0);
    rb_define_method(mTable, "n_rows", rbatk_table_get_n_rows, 0);
    rb_define_method(mTable, "get_column_extent_at", rbatk_table_get_column_extent_at, 2);
    rb_define_method(mTable, "get_row_extent_at", rbatk_table_get_row_extent_at, 2);
    rb_define_method(mTable, "caption", rbatk_table_get_caption, 0);
    rb_define_method(mTable, "get_column_description", rbatk_table_get_column_description, 1);
    rb_define_method(mTable, "get_row_description", rbatk_table_get_row_description, 1);
    rb_define_method(mTable, "get_column_header", rbatk_table_get_column_header, 1);
    rb_define_method(mTable, "get_row_header", rbatk_table_get_row_header, 1);
    rb_define_method(mTable, "summary", rbatk_table_get_summary, 0);
    rb_define_method(mTable, "set_caption", rbatk_table_set_caption, 1);
    rb_define_method(mTable, "set_row_description", rbatk_table_set_row_description, 2);
    rb_define_method(mTable, "set_column_description", rbatk_table_set_column_description, 2);
    rb_define_method(mTable, "set_row_header", rbatk_table_set_row_header, 2);
    rb_define_method(mTable, "set_column_header", rbatk_table_set_column_header, 2);
    rb_define_method(mTable, "set_summary", rbatk_table_set_summary, 1);
    rb_define_method(mTable, "selected_columns", rbatk_table_get_selected_columns, 0);
    rb_define_method(mTable, "selected_rows", rbatk_table_get_selected_rows, 0);
    rb_define_method(mTable, "column_selected?", rbatk_table_is_column_selected, 1);
    rb_define_method(mTable, "row_selected?", rbatk_table_is_row_selected, 1);
    rb_define_method(mTable, "selected?", rbatk_table_is_selected, 2);
    rb_define_method(mTable, "add_column_selection", rbatk_table_add_column_selection, 1);
    rb_define_method(mTable, "add_row_selection", rbatk_table_add_row_selection, 1);
    rb_define_method(mTable, "remove_column_selection", rbatk_table_remove_column_selection, 1);
    rb_define_method(mTable, "remove_row_selection", rbatk_table_remove_row_selection, 1);

    G_DEF_SETTERS(mTable);
}
