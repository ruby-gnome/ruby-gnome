
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

/*
 * Class: Gda::Row
 * Accessing rows of Gda::DataModel.
 */
VALUE cGdaRow;

/*
 * Class method: new(model, *values)
 * model: the Gda::DataModel this row belongs to.
 * values: a list of Gda::Value object to compose the row.
 *
 * Creates a new row from a list of values.
 *
 * If you don't want to specify values, you can also build a row like this:
 *
 * 	Gda::Row.new(model, 5)      # the model will hold 5 values 
 *
 * Returns: a newly created Gda::Row object.
 */
static VALUE rb_gda_row_new(argc, argv, self)
    int argc;   
    VALUE *argv, self;
{
    VALUE model, values;
    GdaRow *row;

    rb_scan_args(argc, argv, "1*", &model, &values);

    /* Using gda_row_new() */
    if (RARRAY(values)->len == 1) {
        row = gda_row_new(RGDA_DATAMODEL(model),
                          INT2FIX(rb_ary_entry(values, 0)));
    }
    /* Using gda_row_new_from_list() */
    else {
        GList *vals = NULL;
        int i;

        for (i = 0; i < RARRAY(values)->len; i++) {
            vals = g_list_append(vals,
                                 RGDA_VALUE(rb_ary_entry(values, i),
                                            GDA_VALUE_TYPE_NULL));
        }
        row = gda_row_new_from_list(RGDA_DATAMODEL(model), vals);
    }
    
    if (row != NULL) { 
        G_INITIALIZE(self, row); 
    }
    return Qnil;
}

/*
 * Method: model
 *
 * Gets the model this row belongs to.
 *
 * Returns: a Gda::DataModel object.
 */
static VALUE rb_gda_row_get_model(self)
    VALUE self;
{
    return RGDA_DATAMODEL_NEW(gda_row_get_model(RGDA_ROW(self)));
}

/*
 * Method: number
 *
 * Gets the number of the given row, that is, its position in its containing
 * data model.
 *
 * Returns: the row number, or -1 if there was an error.
 */
static VALUE rb_gda_row_get_number(self)
    VALUE self;
{
    return INT2FIX(gda_row_get_number(RGDA_ROW(self)));
}

/*
 * Method: unique_id
 *
 * Returns the unique identifier for this row. This identifier is assigned by
 * the different providers, to uniquely identify rows returned to clients.
 * If there is no ID, this means that the row has not been created by a
 * provider, or that it the provider cannot identify it (ie, modifications to
 * it won't take place into the database).
 *
 * Returns: the unique identifier for this row.
 */
static VALUE rb_gda_row_get_unique_id(self)
    VALUE self;
{
    return CSTR2RVAL(gda_row_get_id(RGDA_ROW(self)));
}

/*
 * Method: set_number(number)
 * number: the new row number.
 *
 * Sets the row number of the row.
 *
 * Returns: self.
 */
static VALUE rb_gda_row_set_number(self, number)
    VALUE self, number;
{
    gda_row_set_number(RGDA_ROW(self), FIX2INT(number));
    return self;
}

/*
 * Method: set_unique_id(unique_id)
 * unique_id: new identifier for the row.
 *
 * Assigns a new identifier to the given row. This method is usually called by
 * providers. 
 *
 * Returns: self.
 */
static VALUE rb_gda_row_set_unique_id(self, id)
    VALUE self, id;
{
    gda_row_set_id(RGDA_ROW(self), RVAL2CSTR(id));
    return self;
}

/*
 * Method: values
 *
 * Gets a list of all values this row has.
 *
 * Returns: an Array of Gda::Value objects.
 */
static VALUE rb_gda_row_get_values(self)
    VALUE self;
{
    GdaRow *row;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    row = RGDA_ROW(self);
    for (i = 0; i < gda_row_get_length(row); i++) {
        rb_ary_push(arr, RGDA_VALUE_NEW(gda_row_get_value(row, i)));
    }
    return arr;
}

/*
 * Method: each_value { |value| ... }
 *
 * Calls the block for each value this row has, passing a reference to a
 * Gda::Value object as parameter.
 *
 * Returns: always nil.
 */
static VALUE rb_gda_row_each_value(self)
    VALUE self;
{
    rb_ary_each(rb_gda_row_get_values(self));
    return Qnil;
}

/*
 * Method: length
 *
 * Returns: the number of column the row has.
 */
static VALUE rb_gda_row_get_length(self)
    VALUE self;
{
    return INT2FIX(gda_row_get_length(RGDA_ROW(self)));
}

/*
 * Method: get_value(colnum)
 * colnum: field index.
 *
 * Gets the value indexed in the row at the given position.
 *
 * Returns: a Gda::Value object if successful, or nil if there was an error.
 */
static VALUE rb_gda_row_get_value(self, num)
    VALUE self, num;
{
    const GdaValue *val = gda_row_get_value(RGDA_ROW(self),
                                            FIX2INT(num));
    return val != NULL
        ? RGDA_VALUE_NEW(val)
        : Qnil;
}

/*
 * Method: [](key)
 * key: field index or column name.
 *
 * Gets the value in the row indexed by the 'key' parameter.
 * If the 'key' parameter is a Fixnum, it will be used as an field index.
 * If the 'key' parameter is a String, it will be used as the column's name.
 * 
 * Returns: a Gda::Value object if successful, or nil if there was an error.
 */
static VALUE
rb_gda_row_index(self, key)
    VALUE self, key;
{
    const GdaValue *value;
    GdaRow *row;
    int pos;
   
    value = NULL;
    row = RGDA_ROW(self);
    pos = -1;
    
    if (TYPE(key) == T_STRING) {
        int i;

        for (i = 0; i < gda_row_get_length(RGDA_ROW(self)); i++) {
            GdaDataModel *model = gda_row_get_model(row);
            if (model != NULL && strcmp(gda_data_model_get_column_title(model, i),
                                        RVAL2CSTR(key)) == 0) {
                pos = i;
                break;
            }
        }
    }
    else if (TYPE(key) == T_FIXNUM
          || TYPE(key) == T_BIGNUM
          || TYPE(key) == T_FLOAT) {
        pos = FIX2INT(key);
    }

    if (pos != -1)
        value = gda_row_get_value(row, pos);

    return value != NULL
        ? RGDA_VALUE_NEW(value)
        : Qnil;
}

void Init_gda_row(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_ROW, "Row", mGda);
  
    rb_define_method(c, "initialize", rb_gda_row_new,      -1);
    rb_define_method(c, "model",      rb_gda_row_get_model, 0);
 
    rb_define_method(c, "set_number",    rb_gda_row_set_number,    1);
    rb_define_method(c, "number",        rb_gda_row_get_number,    0);
    rb_define_method(c, "set_unique_id", rb_gda_row_set_unique_id, 1);
    rb_define_method(c, "unique_id",     rb_gda_row_get_unique_id, 0);
 
    rb_define_method(c, "values",     rb_gda_row_get_values, 0); 
    rb_define_method(c, "each_value", rb_gda_row_each_value, 0); 
    rb_define_method(c, "length",     rb_gda_row_get_length, 0);
    rb_define_alias(c, "size", "length");

    rb_define_method(c, "get_value", rb_gda_row_get_value, 1);
    rb_define_method(c, "[]",        rb_gda_row_index,     1);

    G_DEF_SETTERS(c);

    cGdaRow = c;
}

