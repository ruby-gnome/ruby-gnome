
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

VALUE cGdaValue;

GType gda_value_get_type_bis(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaValue",
            (GBoxedCopyFunc)gda_value_copy,
            (GBoxedFreeFunc)gda_value_free);
    }
    return our_type;
}

static VALUE rb_gda_value_to_s(self)
    VALUE self;
{
    return CSTR2RVAL(gda_value_stringify(RGDA_VALUE(self)));
}

void Init_gda_value(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_VALUE, "Value", mGda);
    rb_define_method(c, "to_s", rb_gda_value_to_s, 0);
    cGdaValue = c;
}

