/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatknoopobject.c -

  $Author: mutoh $
  $Date: 2003/12/04 18:06:17 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_NOOPOBJECT(RVAL2GOBJ(s)))

static VALUE
rbatk_no_op_object_initialize(self, gobj)
    VALUE self, gobj;
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
