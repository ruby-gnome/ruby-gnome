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

#define RG_TARGET_NAMESPACE mTable
#define _SELF(s) (RVAL2ATKTABLE(s))

static VALUE
rg_ref_at(VALUE self, VALUE row, VALUE column)
{
    return GOBJ2RVAL(atk_table_ref_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rg_get_index_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_index_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rg_get_column_at_index(VALUE self, VALUE index_)
{
    return INT2NUM(atk_table_get_column_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rg_get_row_at_index(VALUE self, VALUE index_)
{
    return INT2NUM(atk_table_get_row_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rg_n_columns(VALUE self)
{
    return INT2NUM(atk_table_get_n_columns(_SELF(self)));
}

static VALUE
rg_n_rows(VALUE self)
{
    return INT2NUM(atk_table_get_n_rows(_SELF(self)));
}

static VALUE
rg_get_column_extent_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_column_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rg_get_row_extent_at(VALUE self, VALUE row, VALUE column)
{
    return INT2NUM(atk_table_get_row_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rg_caption(VALUE self)
{
    return GOBJ2RVAL(atk_table_get_caption(_SELF(self)));
}

static VALUE
rg_get_column_description(VALUE self, VALUE column)
{
    return CSTR2RVAL(atk_table_get_column_description(_SELF(self), NUM2INT(column)));
}

static VALUE
rg_get_row_description(VALUE self, VALUE row)
{
    return CSTR2RVAL(atk_table_get_row_description(_SELF(self), NUM2INT(row)));
}

static VALUE
rg_get_column_header(VALUE self, VALUE column)
{
    return GOBJ2RVAL(atk_table_get_column_header(_SELF(self), NUM2INT(column)));
}

static VALUE
rg_get_row_header(VALUE self, VALUE row)
{
    return GOBJ2RVAL(atk_table_get_row_header(_SELF(self), NUM2INT(row)));
}

static VALUE
rg_summary(VALUE self)
{
    return GOBJ2RVAL(atk_table_get_summary(_SELF(self)));
}

static VALUE
rg_set_caption(VALUE self, VALUE caption)
{
    atk_table_set_caption(_SELF(self), RVAL2ATKOBJECT(caption));
    return self;
}

static VALUE
rg_set_row_description(VALUE self, VALUE row, VALUE description)
{
    atk_table_set_row_description(_SELF(self), NUM2INT(row), RVAL2CSTR(description));
    return self;
}

static VALUE
rg_set_column_description(VALUE self, VALUE column, VALUE description)
{
    atk_table_set_column_description(_SELF(self), NUM2INT(column), RVAL2CSTR(description));
    return self;
}

static VALUE
rg_set_row_header(VALUE self, VALUE row, VALUE header)
{
    atk_table_set_row_header(_SELF(self), NUM2INT(row), RVAL2ATKOBJECT(header));
    return self;
}

static VALUE
rg_set_column_header(VALUE self, VALUE column, VALUE header)
{
    atk_table_set_column_header(_SELF(self), NUM2INT(column), RVAL2ATKOBJECT(header));
    return self;
}

static VALUE
rg_set_summary(VALUE self, VALUE accessible)
{
    atk_table_set_summary(_SELF(self), RVAL2ATKOBJECT(accessible));
    return self;
}

static VALUE
rg_selected_columns(VALUE self)
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
rg_selected_rows(VALUE self)
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
rg_column_selected_p(VALUE self, VALUE column)
{
    return CBOOL2RVAL(atk_table_is_column_selected(_SELF(self), NUM2INT(column)));
}

static VALUE
rg_row_selected_p(VALUE self, VALUE row)
{
    return CBOOL2RVAL(atk_table_is_row_selected(_SELF(self), NUM2INT(row)));
}

static VALUE
rg_selected_p(VALUE self, VALUE row, VALUE column)
{
    return CBOOL2RVAL(atk_table_is_selected(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rg_add_column_selection(VALUE self, VALUE column)
{
    gboolean ret = atk_table_add_column_selection(_SELF(self), NUM2INT(column));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rg_add_row_selection(VALUE self, VALUE row)
{
    gboolean ret = atk_table_add_row_selection(_SELF(self), NUM2INT(row));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rg_remove_column_selection(VALUE self, VALUE column)
{
    gboolean ret = atk_table_remove_column_selection(_SELF(self), NUM2INT(column));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

static VALUE
rg_remove_row_selection(VALUE self, VALUE row)
{
    gboolean ret = atk_table_remove_row_selection(_SELF(self), NUM2INT(row));
    if (! ret) rb_raise(rb_eRuntimeError, "not implement this interface");
    return self;
}

void
Init_atk_table(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_TABLE, "Table", mAtk);

    RG_DEF_METHOD(ref_at, 2);
    RG_DEF_METHOD(get_index_at, 2);
    RG_DEF_METHOD(get_column_at_index, 1);
    RG_DEF_METHOD(get_row_at_index, 1);
    RG_DEF_METHOD(n_columns, 0);
    RG_DEF_METHOD(n_rows, 0);
    RG_DEF_METHOD(get_column_extent_at, 2);
    RG_DEF_METHOD(get_row_extent_at, 2);
    RG_DEF_METHOD(caption, 0);
    RG_DEF_METHOD(get_column_description, 1);
    RG_DEF_METHOD(get_row_description, 1);
    RG_DEF_METHOD(get_column_header, 1);
    RG_DEF_METHOD(get_row_header, 1);
    RG_DEF_METHOD(summary, 0);
    RG_DEF_METHOD(set_caption, 1);
    RG_DEF_METHOD(set_row_description, 2);
    RG_DEF_METHOD(set_column_description, 2);
    RG_DEF_METHOD(set_row_header, 2);
    RG_DEF_METHOD(set_column_header, 2);
    RG_DEF_METHOD(set_summary, 1);
    RG_DEF_METHOD(selected_columns, 0);
    RG_DEF_METHOD(selected_rows, 0);
    RG_DEF_METHOD_P(column_selected, 1);
    RG_DEF_METHOD_P(row_selected, 1);
    RG_DEF_METHOD_P(selected, 2);
    RG_DEF_METHOD(add_column_selection, 1);
    RG_DEF_METHOD(add_row_selection, 1);
    RG_DEF_METHOD(remove_column_selection, 1);
    RG_DEF_METHOD(remove_row_selection, 1);
}
