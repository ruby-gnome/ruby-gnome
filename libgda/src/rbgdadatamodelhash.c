
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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

static VALUE rb_gda_datamodel_hash_new(self, n_columns)
    VALUE self, n_columns;
{
    GdaDataModel *data = gda_data_model_hash_new(FIX2INT(n_columns));
    if (data != NULL) {
        G_INITIALIZE(self, data);
    }
    return Qnil;
}

static VALUE rb_gda_datamodel_hash_set_n_columns(self, n_columns)
    VALUE self, n_columns;
{
    gda_data_model_hash_set_n_columns(RGDA_DATAMODEL_HASH(self),
                                      FIX2INT(n_columns));
    return self;
}

static VALUE rb_gda_datamodel_hash_clear(self)
    VALUE self;
{
    gda_data_model_hash_clear(RGDA_DATAMODEL_HASH(self));
    return self;
}

static VALUE rb_gda_datamodel_hash_get_value_at(self, colnum, rownum)
    VALUE self, colnum, rownum;
{
    const GdaValue *val = gda_data_model_hash_get_value_at(RGDA_DATAMODEL(self),
                                                           FIX2INT(colnum),
                                                           FIX2INT(rownum));
    return val != NULL  
        ? RGDA_VALUE_NEW(val)
        : Qnil;
}

static VALUE rb_gda_datamodel_hash_insert_row(self, rownum, row)
    VALUE self, rownum, row;
{
    gda_data_model_hash_insert_row(RGDA_DATAMODEL_HASH(self), 
                                   FIX2INT(rownum),
                                   RGDA_ROW(row));
    return self; 
}

static VALUE rb_gda_datamodel_hash_get_row(self, rownum)
    VALUE self, rownum;
{
    const GdaRow *row = gda_data_model_hash_get_row(RGDA_DATAMODEL(self),
                                                    FIX2INT(rownum));
    return row != NULL  
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

void Init_gda_datamodel_hash(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL_HASH, "DataModelHash", mGda);
    
    rb_define_method(c, "initialize",    rb_gda_datamodel_hash_new,           1);
    rb_define_method(c, "set_n_columns", rb_gda_datamodel_hash_set_n_columns, 1);
    rb_define_method(c, "clear",         rb_gda_datamodel_hash_clear,         0);
    rb_define_method(c, "get_value_at",  rb_gda_datamodel_hash_get_value_at,  2);
    rb_define_method(c, "insert_row",    rb_gda_datamodel_hash_insert_row,    2);
    rb_define_method(c, "get_row",       rb_gda_datamodel_hash_get_row,       1);
    G_DEF_SETTER(c, "n_columns"); 
}

