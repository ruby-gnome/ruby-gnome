/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkstateset.c -

  $Author: mutoh $
  $Date: 2003/12/23 15:59:28 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_STATE_SET(RVAL2GOBJ(s)))

static VALUE
rbatkstateset_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, atk_state_set_new());
    return Qnil;
}

static VALUE
rbatkstateset_is_empty(self)
    VALUE self;
{
    return CBOOL2RVAL(atk_state_set_is_empty(_SELF(self)));
}

static VALUE
rbatkstateset_add_state(self, type)
    VALUE self, type;
{
    return CBOOL2RVAL(atk_state_set_add_state(_SELF(self), 
                                              RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

static VALUE
rbatkstateset_add_states(self, types)
    VALUE self, types;
{
    gint i;
    gint n_types = RARRAY(types)->len;
    AtkStateType* atypes = g_new(AtkStateType, n_types);

    Check_Type(types, T_ARRAY);

    for (i = 0; i < n_types; i++) {
        atypes[i] = RVAL2GENUM(RARRAY(types)->ptr[i], ATK_TYPE_STATE_TYPE);
    }

    atk_state_set_add_states(_SELF(self), atypes, n_types);
    g_free(atypes);
    return self;
}

static VALUE
rbatkstateset_clear_states(self)
    VALUE self;
{
    atk_state_set_clear_states(_SELF(self));
    return self;
}

static VALUE
rbatkstateset_contains_state(self, type)
    VALUE self;
{
    return CBOOL2RVAL(atk_state_set_contains_state(_SELF(self), 
                                                   RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

static VALUE
rbatkstateset_contains_states(self, types)
    VALUE self, types;
{
    gint i;
    gboolean ret;
    gint n_types = RARRAY(types)->len;
    AtkStateType* atypes = g_new(AtkStateType, n_types);

    Check_Type(types, T_ARRAY);

    for (i = 0; i < n_types; i++) {
        atypes[i] = RVAL2GENUM(RARRAY(types)->ptr[i], ATK_TYPE_STATE_TYPE);
    }
    
    ret = CBOOL2RVAL(atk_state_set_contains_states(_SELF(self), atypes, n_types));

    g_free(atypes);
    return ret;
}

static VALUE
rbatkstateset_remove_state(self, type)
    VALUE self, type;
{
    return CBOOL2RVAL(atk_state_set_remove_state(_SELF(self), 
                                                 RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

static VALUE
rbatkstateset_and_sets(self, compare_set)
    VALUE self, compare_set;
{
    return GOBJ2RVAL(atk_state_set_and_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rbatkstateset_or_sets(self, compare_set)
    VALUE self, compare_set;
{
    return GOBJ2RVAL(atk_state_set_or_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rbatkstateset_xor_sets(self, compare_set)
    VALUE self, compare_set;
{
    return GOBJ2RVAL(atk_state_set_xor_sets(_SELF(self), _SELF(compare_set)));
}

void
Init_atk_state_set()
{
    VALUE stateset = G_DEF_CLASS(ATK_TYPE_STATE_SET, "StateSet", mAtk);

    rb_define_method(stateset, "initialize", rbatkstateset_initialize, 0);
    rb_define_method(stateset, "empty?", rbatkstateset_is_empty, 0);
    rb_define_method(stateset, "add_state", rbatkstateset_add_state, 1);
    rb_define_method(stateset, "add_states", rbatkstateset_add_states, 1);
    rb_define_method(stateset, "clear_states", rbatkstateset_clear_states, 0);
    rb_define_method(stateset, "contains_state", rbatkstateset_contains_state, 1);
    rb_define_method(stateset, "contains_states", rbatkstateset_contains_states, 1);
    rb_define_method(stateset, "remove_state", rbatkstateset_remove_state, 1);
    rb_define_method(stateset, "and", rbatkstateset_and_sets, 1);
    rb_define_alias(stateset, "&", "and");
    rb_define_method(stateset, "or", rbatkstateset_or_sets, 1);
    rb_define_alias(stateset, "|", "or");
    rb_define_method(stateset, "xor", rbatkstateset_xor_sets, 1);
    rb_define_alias(stateset, "^", "xor");
}
