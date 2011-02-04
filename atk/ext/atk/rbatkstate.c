/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkstate.c -

  $Author: mutoh $
  $Date: 2004/10/17 23:06:07 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_STATE(RVAL2GOBJ(s)))

static VALUE
rbatkstate_s_type_register(VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_state_type_register(RVAL2CSTR(name)), ATK_TYPE_STATE_TYPE);
}

/* We don't need this.
G_CONST_RETURN gchar* atk_state_type_get_name
                                            (AtkStateType type);
*/

static VALUE
rbatkstate_s_for_name(VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_state_type_for_name(RVAL2CSTR(name)), ATK_TYPE_STATE_TYPE);
}

void
Init_atk_state()
{
    VALUE state = G_DEF_CLASS(ATK_TYPE_STATE_TYPE, "State", mAtk);
    rb_define_singleton_method(state, "type_register", rbatkstate_s_type_register, 1);
    rb_define_singleton_method(state, "for_name", rbatkstate_s_for_name, 1);
    G_DEF_CONSTANTS(mAtk, ATK_TYPE_STATE_TYPE, "ATK_");
}
