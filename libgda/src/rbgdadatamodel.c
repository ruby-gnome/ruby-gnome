
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/Gda.
 *
 * Ruby/Gda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Gda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Gda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

static VALUE rb_gda_datamodel_get_columns(self)
    VALUE self;
{
    GdaDataModel *datamod;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    datamod = RGDA_DATAMODEL(self);
    for (i = 0; i < gda_data_model_get_n_columns(datamod); i++) {
        rb_ary_push(arr, CSTR2RVAL(gda_data_model_get_column_title(datamod, 
                                                                   i))); 
    }
    return arr;
}

static VALUE rb_gda_datamodel_each_column(self)
    VALUE self;
{
    rb_ary_each(rb_gda_datamodel_get_columns(self));
    return Qnil;
}

static VALUE rb_gda_datamodel_get_n_columns(self)
    VALUE self;
{
    return INT2FIX(gda_data_model_get_n_columns(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_get_column(self, column_number)
    VALUE self, column_number;
{
    const gchar *column = gda_data_model_get_column_title(RGDA_DATAMODEL(self),  
                                                          FIX2INT(column_number));
    return column != NULL
        ? CSTR2RVAL(column) 
        : Qnil;
}

static VALUE rb_gda_datamodel_get_column_pos(self, column)
    VALUE self, column;
{
    return INT2FIX(gda_data_model_get_column_position(RGDA_DATAMODEL(self),
                                                      RVAL2CSTR(column)));
}

static VALUE rb_gda_datamodel_get_rows(self)
    VALUE self;
{
    GdaDataModel *datamod;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    datamod = RGDA_DATAMODEL(self);
    for (i = 0; i < gda_data_model_get_n_rows(datamod); i++) {
        rb_ary_push(arr, RGDA_ROW_NEW(gda_data_model_get_row(datamod, 
                                                             i))); 
    }
    return arr;
}

static VALUE rb_gda_datamodel_each_row(self)
    VALUE self;
{
    rb_ary_each(rb_gda_datamodel_get_rows(self));
    return Qnil;
}

static VALUE rb_gda_datamodel_get_n_rows(self)
    VALUE self;
{
    return INT2FIX(gda_data_model_get_n_rows(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_get_row(self, row_number)
    VALUE self, row_number;
{
    const GdaRow *row = gda_data_model_get_row(RGDA_DATAMODEL(self),  
                                               FIX2INT(row_number));
    return row != NULL
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

void Init_gda_datamodel(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL, "DataModel", mGda);

    rb_define_method(c, "columns",          rb_gda_datamodel_get_columns,   0);
    rb_define_method(c, "each_column",      rb_gda_datamodel_each_column,   0);
    rb_define_method(c, "n_columns",        rb_gda_datamodel_get_n_columns, 0);
    rb_define_method(c, "get_column_title", rb_gda_datamodel_get_column,    1);

    rb_define_method(c, "get_column_position", rb_gda_datamodel_get_column_pos, 1);

    rb_define_method(c, "rows",     rb_gda_datamodel_get_rows,   0);
    rb_define_method(c, "each_row", rb_gda_datamodel_each_row,   0);
    rb_define_method(c, "n_rows",   rb_gda_datamodel_get_n_rows, 0);
    rb_define_method(c, "get_row",  rb_gda_datamodel_get_row,    1);
}

