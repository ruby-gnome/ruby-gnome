/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatktable.c -

  $Author: mutoh $
  $Date: 2003/12/25 17:46:49 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_TABLE(RVAL2GOBJ(s)))

static VALUE
rbatk_table_ref_at(self, row, column)
    VALUE self, row, column;
{
    return GOBJ2RVAL(atk_table_ref_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_index_at(self, row, column)
    VALUE self, row, column;
{
    return INT2NUM(atk_table_get_index_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_column_at_index(self, index_)
    VALUE self, index_;
{
    return INT2NUM(atk_table_get_column_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rbatk_table_get_row_at_index(self, index_)
    VALUE self, index_;
{
    return INT2NUM(atk_table_get_row_at_index(_SELF(self), NUM2INT(index_)));
}

static VALUE
rbatk_table_get_n_columns(self)
    VALUE self;
{
    return INT2NUM(atk_table_get_n_columns(_SELF(self)));
}

static VALUE
rbatk_table_get_n_rows(self)
    VALUE self;
{
    return INT2NUM(atk_table_get_n_rows(_SELF(self)));
}

static VALUE
rbatk_table_get_column_extent_at(self, row, column)
    VALUE self, row, column;
{
    return INT2NUM(atk_table_get_column_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_extent_at(self, row, column)
    VALUE self, row, column;
{
    return INT2NUM(atk_table_get_row_extent_at(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_get_caption(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_table_get_caption(_SELF(self)));
}

static VALUE
rbatk_table_get_column_description(self, column)
    VALUE self, column;
{
    return CSTR2RVAL(atk_table_get_column_description(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_description(self, row)
    VALUE self, row;
{
    return CSTR2RVAL(atk_table_get_row_description(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_get_column_header(self, column)
    VALUE self, column;
{
    return GOBJ2RVAL(atk_table_get_column_header(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_get_row_header(self, row)
    VALUE self, row;
{
    return GOBJ2RVAL(atk_table_get_row_header(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_get_summary(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_table_get_summary(_SELF(self)));
}

static VALUE
rbatk_table_set_caption(self, caption)
    VALUE self, caption;
{
    atk_table_set_caption(_SELF(self), ATK_OBJECT(RVAL2GOBJ(caption)));
    return self;
}

static VALUE
rbatk_table_set_row_description(self, row, description)
    VALUE self, row, description;
{
    atk_table_set_row_description(_SELF(self), NUM2INT(row), RVAL2CSTR(description));
    return self;
}

static VALUE
rbatk_table_set_column_description(self, column, description)
    VALUE self, column, description;
{
    atk_table_set_column_description(_SELF(self), NUM2INT(column), RVAL2CSTR(description));
    return self;
}

static VALUE
rbatk_table_set_row_header(self, row, header)
    VALUE self, row, header;
{
    atk_table_set_row_header(_SELF(self), NUM2INT(row), ATK_OBJECT(RVAL2GOBJ(header)));
    return self;
}

static VALUE
rbatk_table_set_column_header(self, column, header)
    VALUE self, column, header;
{
    atk_table_set_column_header(_SELF(self), NUM2INT(column), ATK_OBJECT(RVAL2GOBJ(header)));
    return self;
}

static VALUE
rbatk_table_set_summary(self, accessible)
    VALUE self, accessible;
{
    atk_table_set_summary(_SELF(self), ATK_OBJECT(RVAL2GOBJ(accessible)));
    return self;
}

static VALUE
rbatk_table_get_selected_columns(self)
    VALUE self;
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
rbatk_table_get_selected_rows(self)
    VALUE self;
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
rbatk_table_is_column_selected(self, column)
    VALUE self, column;
{
    return CBOOL2RVAL(atk_table_is_column_selected(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_is_row_selected(self, row)
    VALUE self, row;
{
    return CBOOL2RVAL(atk_table_is_row_selected(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_is_selected(self, row, column)
    VALUE self, row, column;
{
    return CBOOL2RVAL(atk_table_is_selected(_SELF(self), NUM2INT(row), NUM2INT(column)));
}

static VALUE
rbatk_table_add_column_selection(self, column)
    VALUE self, column;
{
    return CBOOL2RVAL(atk_table_add_column_selection(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_add_row_selection(self, row)
    VALUE self, row;
{
    return CBOOL2RVAL(atk_table_add_row_selection(_SELF(self), NUM2INT(row)));
}

static VALUE
rbatk_table_remove_column_selection(self, column)
    VALUE self, column;
{
    return CBOOL2RVAL(atk_table_remove_column_selection(_SELF(self), NUM2INT(column)));
}

static VALUE
rbatk_table_remove_row_selection(self, row)
    VALUE self, row;
{
    return CBOOL2RVAL(atk_table_remove_row_selection(_SELF(self), NUM2INT(row)));
}

void
Init_atk_table()
{
    VALUE mTable = G_DEF_INTERFACE(ATK_TYPE_TABLE, "Table", mAtk);

    rb_define_method(mTable, "set_caption", rbatk_table_set_caption, 1);
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
