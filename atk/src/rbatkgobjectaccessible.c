/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkgobjectaccessible.c -

  $Author: mutoh $
  $Date: 2003/12/04 18:06:17 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_GOBJECT_ACCESSIBLE(RVAL2GOBJ(s)))

static VALUE
rbatk_gobjectaccessible_s_for_object(self, obj)
    VALUE self, obj;
{
    return GOBJ2RVAL(atk_gobject_accessible_for_object(RVAL2GOBJ(obj)));
}

static VALUE
rbatk_gobjectaccessible_get_object(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_gobject_accessible_get_object(_SELF(self)));
}

void
Init_atk_gobjectaccessible()
{
    VALUE macc = G_DEF_INTERFACE(ATK_TYPE_GOBJECT_ACCESSIBLE, "GObjectAccessible", mAtk);

    rb_define_singleton_method(macc, "for_object", rbatk_gobjectaccessible_s_for_object, 1);
    rb_define_method(macc, "object", rbatk_gobjectaccessible_get_object, 0);
}
