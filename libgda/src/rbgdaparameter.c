
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

static GdaParameter* parameter_copy(GdaParameter* parameter) {
    GdaValue *value = GDA_VALUE(gda_parameter_get_value(parameter));
    return gda_parameter_new_from_value(gda_parameter_get_name(parameter),
                                        value);
}

GType gda_parameter_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaParameter",
            (GBoxedCopyFunc)parameter_copy,
            (GBoxedFreeFunc)gda_parameter_free);
    }
    return our_type;
}

static VALUE rb_gda_parameter_new(self, name, value)
    VALUE self, name, value;
{
    GdaParameter *param = NULL;

    if (TYPE(value) == T_TRUE || TYPE(value) == T_FALSE) {
        param = gda_parameter_new_boolean(RVAL2CSTR(name),
                                          RVAL2CBOOL(value));
    }
    else if (TYPE(value) == T_STRING) {
        param = gda_parameter_new_string(RVAL2CSTR(name),
                                         RVAL2CSTR(value));
    }
    else if (TYPE(value) == T_FIXNUM
          || TYPE(value) == T_BIGNUM
          || TYPE(value) == T_FLOAT) {
        param = gda_parameter_new_double(RVAL2CSTR(name),
                                         NUM2DBL(value));   
    }
    else if (CLASS_OF(value) == cGdaValue) {
        param = gda_parameter_new_from_value(RVAL2CSTR(name),
                                             RGDA_VALUE(value));   
    }

    if (param != NULL) {
        G_INITIALIZE(self, param);
    }
    return Qnil;
}

static VALUE rb_gda_parameter_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_parameter_get_name(RGDA_PARAMETER(self)));
}

static VALUE rb_gda_parameter_set_name(self, name)
    VALUE self, name;
{
    gda_parameter_set_name(RGDA_PARAMETER(self),
                           RVAL2CSTR(name));
    return self;
}

static VALUE rb_gda_parameter_get_value(self)
    VALUE self;
{
    return RGDA_VALUE_NEW(gda_parameter_get_value(RGDA_PARAMETER(self)));
}

static VALUE rb_gda_parameter_set_value(self, value)
    VALUE self, value;
{
    gda_parameter_set_value(RGDA_PARAMETER(self),
                            RGDA_VALUE(value));
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
}

