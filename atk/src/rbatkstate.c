/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkstate.c -

  $Author: mutoh $
  $Date: 2003/12/08 16:32:09 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_STATE(RVAL2GOBJ(s)))

static VALUE
rbatkstate_s_type_register(self, name)
    VALUE self, name;
{
    return GENUM2RVAL(atk_state_type_register(RVAL2CSTR(name)), ATK_TYPE_STATE_TYPE);
}

/* We don't need them
G_CONST_RETURN gchar* atk_state_type_get_name
                                            (AtkStateType type);
AtkStateType atk_state_type_for_name  (const gchar *name);
*/

void
Init_atk_state()
{
    VALUE state = G_DEF_CLASS(ATK_TYPE_STATE_TYPE, "StateType", mAtk);
    rb_define_singleton_method(state, "type_register", rbatkstate_s_type_register, 1);
    G_DEF_CONSTANTS(mAtk, ATK_TYPE_STATE_TYPE, "ATK_");
}
