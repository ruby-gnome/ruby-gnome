
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
 * Class: Gda::Parameter
 *
 * Parameters are the way clients have to send an unlimited number of arguments
 * to the providers.
 */
VALUE cGdaParameter;

/*
 * Class method: new(name, value)
 * name: the name for the parameter being created.
 * value: a value for the parameter, as a Gda::Value object.
 *
 * Creates a new Gda::Parameter object, which is usually used with
 * Gda::ParameterList.
 *
 * Returns: a newly created Gda::Parameter object.
 */
static VALUE rb_gda_parameter_new(self, name, value)
    VALUE self, name, value;
{
    GdaParameter *param = gda_parameter_new_from_value(
        RVAL2CSTR(name),
        RGDA_VALUE(value, GDA_VALUE_TYPE_NULL));
    if (param != NULL) {
        G_INITIALIZE(self, param);
    }
    return Qnil;
}

/*
 * Method: name
 *
 * Returns: the name of the parameter.
 */
static VALUE rb_gda_parameter_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_parameter_get_name(RGDA_PARAMETER(self)));
}

/*
 * Method: set_name(name)
 * name: new name for the parameter.
 *
 * Sets the name of the parameter.
 *
 * Returns: self.
 */
static VALUE rb_gda_parameter_set_name(self, name)
    VALUE self, name;
{
    gda_parameter_set_name(RGDA_PARAMETER(self),
                           RVAL2CSTR(name));
    return self;
}

/*
 * Method: value
 *
 * Returns: the value of the parameter, as a Gda::Value object.
 */
static VALUE rb_gda_parameter_get_value(self)
    VALUE self;
{
    return RGDA_VALUE_NEW(gda_parameter_get_value(RGDA_PARAMETER(self)));
}

/*
 * Method: set_value(value)
 * value: new value for the parameter, as a Gda::Value object.
 *
 * Sets the value of the parameter.
 *
 * Returns: self.
 */
static VALUE rb_gda_parameter_set_value(self, value)
    VALUE self, value;
{
    gda_parameter_set_value(RGDA_PARAMETER(self),
                            RGDA_VALUE(value, GDA_VALUE_TYPE_NULL));
    return self;
}

void Init_gda_parameter(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_PARAMETER, "Parameter", mGda);

    rb_define_method(c, "initialize", rb_gda_parameter_new, 2);

    rb_define_method(c, "name",      rb_gda_parameter_get_name,  0);
    rb_define_method(c, "set_name",  rb_gda_parameter_set_name,  1);
    rb_define_method(c, "value",     rb_gda_parameter_get_value, 0);
    rb_define_method(c, "set_value", rb_gda_parameter_set_value, 1);

    G_DEF_SETTERS(c);

    cGdaParameter = c;
}

