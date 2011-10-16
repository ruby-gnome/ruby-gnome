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

#define _SELF(s) (ATK_GOBJECT_ACCESSIBLE(RVAL2GOBJ(s)))

static VALUE
rbatk_gobjectaccessible_s_for_object(VALUE self, VALUE obj)
{
    return GOBJ2RVAL(atk_gobject_accessible_for_object(RVAL2GOBJ(obj)));
}

static VALUE
rbatk_gobjectaccessible_get_object(VALUE self)
{
    return GOBJ2RVAL(atk_gobject_accessible_get_object(_SELF(self)));
}

void
Init_atk_gobjectaccessible()
{
    VALUE macc = G_DEF_CLASS(ATK_TYPE_GOBJECT_ACCESSIBLE, "GObjectAccessible", mAtk);

    rb_define_singleton_method(macc, "for_object", rbatk_gobjectaccessible_s_for_object, 1);
    rb_define_method(macc, "object", rbatk_gobjectaccessible_get_object, 0);
}
