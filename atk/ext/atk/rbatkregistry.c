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

#define _SELF(s) (ATK_REGISTRY(RVAL2GOBJ(s)))

static VALUE
rbatkregistry_set_factory_type(VALUE self, VALUE type, VALUE factory_type)
{
    atk_registry_set_factory_type(_SELF(self),
                                  CLASS2GTYPE(type),
                                  CLASS2GTYPE(factory_type));
    return self;
}

static VALUE
rbatkregistry_get_factory_type(VALUE self, VALUE type)
{
    return GTYPE2CLASS(atk_registry_get_factory_type(_SELF(self), CLASS2GTYPE(type)));
}

static VALUE
rbatkregistry_get_factory(VALUE self, VALUE type)
{
    return GOBJ2RVAL(atk_registry_get_factory(_SELF(self),
                                              CLASS2GTYPE(type)));
}

static VALUE
rbatkregistry_s_get_default_registry(VALUE self)
{
    return GOBJ2RVAL(atk_get_default_registry());
}

void
Init_atk_registry()
{
    VALUE registry = G_DEF_CLASS(ATK_TYPE_REGISTRY, "Registry", mAtk);
    rb_define_method(registry, "set_factory_type", rbatkregistry_set_factory_type, 2);
    rb_define_method(registry, "get_factory_type", rbatkregistry_get_factory_type, 1);
    rb_define_method(registry, "get_factory", rbatkregistry_get_factory, 1);
    rb_define_singleton_method(registry, "default_registry", rbatkregistry_s_get_default_registry, 0);
}
