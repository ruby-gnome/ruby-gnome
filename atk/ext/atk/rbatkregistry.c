/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkregistry.c -

  $Author: mutoh $
  $Date: 2003/12/07 18:12:33 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
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
