
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
 * Class: Gda::DataModelArray
 * An implementation of Gda::DataModel based on an array.
 */
VALUE cGdaDataModelArray;

/*
 * Class method: new(n_columns)
 * n_columns: number of columns for rows of this data model.
 *
 * Returns: a reference to a newly created Gda::DataModelArray object.
 */
static VALUE rb_gda_datamodel_array_new(self, n_columns)
    VALUE self, n_columns;
{
    GdaDataModel *data = gda_data_model_array_new(FIX2INT(n_columns));
    if (data != NULL) {
        G_INITIALIZE(self, data);
    }
    return Qnil;
}

/*
 * Method: set_n_columns(n_columns)
 * n_columns: number of columns for rows this data model should use (must be
 * greated than or equal to 0). 
 *
 * Sets the number of columns for rows inserted in this model.
 *
 * Returns: self.
 */
static VALUE rb_gda_datamodel_array_set_n_columns(self, n_columns)
    VALUE self, n_columns;
{
    gda_data_model_array_set_n_columns(RGDA_DATAMODEL_ARRAY(self),
                                       FIX2INT(n_columns));
    return self;
}

/*
 * Method: clear
 *
 * Frees all the rows inserted in this model.
 *
 * Returns: self.
 */
static VALUE rb_gda_datamodel_array_clear(self)
    VALUE self;
{
    gda_data_model_array_clear(RGDA_DATAMODEL_ARRAY(self));
    return self;
}

void Init_gda_datamodel_array(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL_ARRAY, "DataModelArray", mGda);
    
    rb_define_method(c, "initialize",    rb_gda_datamodel_array_new,           1);
    rb_define_method(c, "set_n_columns", rb_gda_datamodel_array_set_n_columns, 1);
    rb_define_method(c, "clear",         rb_gda_datamodel_array_clear,         0);
    G_DEF_SETTER(c, "n_columns"); 

    cGdaDataModelArray = c;
}

