
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

static VALUE rb_gda_table_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_table_get_name(RGDA_TABLE(self)));
}

static VALUE rb_gda_table_set_name(self, name)
    VALUE self, name;
{
    gda_table_set_name(RGDA_TABLE(self),
                       RVAL2CSTR(name));
    return name;
}

static VALUE rb_gda_table_add_field(self, field)
    VALUE self, field;
{
    gda_table_add_field(RGDA_TABLE(self),
                        RGDA_FIELD_ATTRIBUTES(field));
    return self;
}

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
}

