
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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
 * Class: Gda::Table
 * An in-memory representation of a database table
 */
VALUE cGdaTable;

/*
 * Class method: new(name, model=nil, add_data=nil)
 * name: name for the new table.
 * model: a Gda::DataModel to create the table from.
 * add_data: whether to add model's data or not.
 *
 * Creates a new Gda::Table object, which is an in-memory representation of an
 * entire table. It is mainly used by the Gda::XmlDatabase class, but you can
 * also use it in your applications for whatever you may need it.
 *
 * You can optionally pass a Gda::DataModel object to create the table from.
 *
 * Returns: a newly created Gda::Table object.
 */
static VALUE rb_gda_table_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE name, datamodel, add_data;
    GdaTable *table;

    rb_scan_args(argc, argv, "12", &name, &datamodel, &add_data);

    if (NIL_P(datamodel) || NIL_P(add_data)) {
        table = gda_table_new(RVAL2CSTR(name));
    }
    else {
        table = gda_table_new_from_model(RVAL2CSTR(name),
                                         RGDA_DATAMODEL(datamodel),
                                         RVAL2CBOOL(add_data));
    }
    if (table != NULL) {
        G_INITIALIZE(self, table);
    }
    return Qnil;
}

/*
 * Method: name
 *
 * Returns: the name of the table.
 */
static VALUE rb_gda_table_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_table_get_name(RGDA_TABLE(self)));
}

/*
 * Method: set_name(name)
 * name: new name for the table.
 *
 * Sets the name of the table.
 *
 * Returns: self.
 */
static VALUE rb_gda_table_set_name(self, name)
    VALUE self, name;
{
    gda_table_set_name(RGDA_TABLE(self),
                       RVAL2CSTR(name));
    return self;
}

/*
 * Method: add_field(field)
 * field: a Gda::FieldAttributes object.
 *
 * Adds a field to the table.
 *
 * Returns: self.
 */
static VALUE rb_gda_table_add_field(self, field)
    VALUE self, field;
{
    gda_table_add_field(RGDA_TABLE(self),
                        RGDA_FIELD_ATTRIBUTES(field));
    return self;
}

/*
 * Method: add_data_from_model(model)
 * model: the Gda::DataModel from which to add the data.
 *
 * Adds data from the given model to the table.
 *
 * Returns: self.
 */
static VALUE rb_gda_table_add_data_from_model(self, model)
    VALUE self, model;
{
    gda_table_add_data_from_model(RGDA_TABLE(self),
                                  RGDA_DATAMODEL(model));
    return self;
}

void Init_gda_table(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_TABLE, "Table", mGda);

    rb_define_method(c, "initialize", rb_gda_table_new,      -1);
    rb_define_method(c, "name",       rb_gda_table_get_name,  0);
    rb_define_method(c, "set_name",   rb_gda_table_set_name,  1);
    rb_define_method(c, "add_field",  rb_gda_table_add_field, 1);
    rb_define_method(c, "add_data_from_model",  
                     rb_gda_table_add_data_from_model, 1);
    G_DEF_SETTER(c, "name");

    cGdaTable = c;
}

