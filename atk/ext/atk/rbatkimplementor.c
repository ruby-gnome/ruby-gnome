/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkimplementor.c -

  $Author: mutoh $
  $Date: 2003/12/07 17:18:16 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_IMPLEMENTOR(RVAL2GOBJ(s)))

static VALUE
rbatkimpl_ref_accessible(VALUE self)
{
    return GOBJ2RVAL(atk_implementor_ref_accessible(_SELF(self)));
}

void
Init_atk_implementor()
{
    VALUE impl = G_DEF_INTERFACE(ATK_TYPE_IMPLEMENTOR, "Implementor", mAtk);
    rb_define_method(impl, "ref_accessible", rbatkimpl_ref_accessible, 0);
}
