/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkobjectfactory.c -

  $Author: mutoh $
  $Date: 2003/12/07 18:12:33 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

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
Init_atk_objectfactory()
{
    VALUE fact = G_DEF_CLASS(ATK_TYPE_OBJECT_FACTORY, "ObjectFactory", mAtk);
    rb_define_method(fact, "create_accessible", rbatkfact_create_accessible, 0);
    rb_define_method(fact, "accessible_type", rbatkfact_get_accessible_type, 0);
    rb_define_method(fact, "invalidate", rbatkfact_invalidate, 0);
}
