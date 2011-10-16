/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbatk.h"

#define _SELF(s) (ATK_NOOPOBJECT(RVAL2GOBJ(s)))

static VALUE
rbatk_no_op_object_initialize(VALUE self, VALUE gobj)
{
    G_INITIALIZE(self, atk_no_op_object_new(RVAL2GOBJ(gobj)));
    return Qnil;
}

void
Init_atk_noopobject()
{
    VALUE noop = G_DEF_CLASS(ATK_TYPE_NO_OP_OBJECT, "NoOpObject", mAtk);

    rb_define_method(noop, "initialize", rbatk_no_op_object_initialize, 1);
}
