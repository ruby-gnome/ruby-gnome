/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkvalue.c -

  $Author: mutoh $
  $Date: 2003/12/23 15:59:28 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_VALUE(RVAL2GOBJ(s)))

static VALUE
rbatk_value_get_current_value(VALUE self)
{
    GValue gval = {0,};
    atk_value_get_current_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}


static VALUE
rbatk_value_get_maximum_value(VALUE self)
{
    GValue gval = {0,};
    atk_value_get_maximum_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}

static VALUE
rbatk_value_get_minimum_value(VALUE self)
{
    GValue gval = {0,};
    atk_value_get_minimum_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}

static VALUE
rbatk_value_set_current_value(VALUE self, VALUE value)
{
    GValue gval = {0,};
    g_value_init(&gval, RVAL2GTYPE(value));

    rbgobj_rvalue_to_gvalue(value, &gval);

    if (! atk_value_set_current_value(_SELF(self), &gval)){
        rb_raise(rb_eRuntimeError, "Can't set the current value.");
    }

    return self;
}

void
Init_atk_value()
{
    VALUE mValue = G_DEF_INTERFACE(ATK_TYPE_VALUE, "Value", mAtk);

    rb_define_method(mValue, "current", rbatk_value_get_current_value, 0);
    rb_define_method(mValue, "max", rbatk_value_get_maximum_value, 0);
    rb_define_method(mValue, "min", rbatk_value_get_minimum_value, 0);
    rb_define_method(mValue, "set_current", rbatk_value_set_current_value, 1);

    G_DEF_SETTERS(mValue);
}
