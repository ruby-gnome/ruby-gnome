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

#define _SELF(s) (ATK_OBJECT_FACTORY(RVAL2GOBJ(s)))

static VALUE
rbatkfact_create_accessible(VALUE self, VALUE obj)
{
    return GOBJ2RVAL(atk_object_factory_create_accessible(_SELF(self), RVAL2GOBJ(obj)));
}

static VALUE
rbatkfact_get_accessible_type(VALUE self)
{
    return GTYPE2CLASS(atk_object_factory_get_accessible_type(_SELF(self)));
}

static VALUE
rbatkfact_invalidate(VALUE self)
{
    atk_object_factory_invalidate(_SELF(self));
    return self;
}

void
Init_atk_objectfactory(void)
{
    VALUE fact = G_DEF_CLASS(ATK_TYPE_OBJECT_FACTORY, "ObjectFactory", mAtk);
    rb_define_method(fact, "create_accessible", rbatkfact_create_accessible, 0);
    rb_define_method(fact, "accessible_type", rbatkfact_get_accessible_type, 0);
    rb_define_method(fact, "invalidate", rbatkfact_invalidate, 0);
}
