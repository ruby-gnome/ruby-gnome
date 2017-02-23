/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE cGObjectAccessible
#define _SELF(s) (RVAL2ATKGOBJECTACCESSIBLE(s))

static VALUE
rg_s_for_object(G_GNUC_UNUSED VALUE self, VALUE obj)
{
    return GOBJ2RVAL(atk_gobject_accessible_for_object(RVAL2GOBJ(obj)));
}

static VALUE
rg_object(VALUE self)
{
    return GOBJ2RVAL(atk_gobject_accessible_get_object(_SELF(self)));
}

void
Init_atk_gobjectaccessible(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_GOBJECT_ACCESSIBLE, "GObjectAccessible", mAtk);

    RG_DEF_SMETHOD(for_object, 1);
    RG_DEF_METHOD(object, 0);
}
