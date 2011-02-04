/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatknoopobjectfactory.c -

  $Author: mutoh $
  $Date: 2003/12/07 17:18:16 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_NO_OP_OBJECT_FACTORY(RVAL2GOBJ(s)))

static VALUE
rbatk_no_op_object_factory_initialize(VALUE self)
{
    G_INITIALIZE(self, atk_no_op_object_factory_new());
    return Qnil;
}

void
Init_atk_noopobjectfactory()
{
    VALUE noop = G_DEF_CLASS(ATK_TYPE_NO_OP_OBJECT_FACTORY, "NoOpObjectFactory", mAtk);

    rb_define_method(noop, "initialize", rbatk_no_op_object_factory_initialize, 0);
}
