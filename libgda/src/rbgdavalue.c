
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
 * Class: Gda::Value
 * Data values.
 */
VALUE cGdaValue;

/*
 * Method: to_s
 *
 * Converts a Gda::Value to its string representation.
 *
 * Returns: a formatted string.
 */
static VALUE rb_gda_value_to_s(self)
    VALUE self;
{
    return CSTR2RVAL(gda_value_stringify(RGDA_VALUE(self)));
}

/*
 * Method: <=>(an_other_value)
 * an_other_value: the other value to be compared to.
 *
 * Compares two Gda::Value objects of the same type.
 *
 * Returns: if both values have the same type, returns 0 if both contains the
 * same value, an integer less than 0 if the value is less than the other or an
 * integer greater than 0 if the value is greater than the other.
 */
static VALUE rb_gda_value_cmp(self, other_value)
    VALUE self;
{
    return INT2FIX(gda_value_compare(RGDA_VALUE(self),
                                     RGDA_VALUE(other_value)));
}

void Init_gda_value(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_VALUE, "Value", mGda);
    rb_define_method(c, "to_s", rb_gda_value_to_s, 0);
    rb_define_method(c, "<=>",  rb_gda_value_cmp,  1);
    cGdaValue = c;
}

