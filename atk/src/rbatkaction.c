/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkaction.c -

  $Author: mutoh $
  $Date: 2003/12/04 18:06:17 $

  Copyright (C) 2003 Masao Mutoh 
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_ACTION(RVAL2GOBJ(s)))

static VALUE
rbatk_action_do_action(self, i)
    VALUE self, i;
{
    return CBOOL2RVAL(atk_action_do_action(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_action_get_n_actions(self)
    VALUE self;
{
    return INT2NUM(atk_action_get_n_actions(_SELF(self)));
}

static VALUE
rbatk_action_get_description(self, i)
    VALUE self, i;
{
    return CSTR2RVAL(atk_action_get_description(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_action_get_name(self, i)
    VALUE self, i;
{
    return CSTR2RVAL(atk_action_get_name(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_action_get_localized_name(self, i)
    VALUE self, i;
{
    return CSTR2RVAL(atk_action_get_localized_name(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_action_get_keybinding(self, i)
    VALUE self, i;
{
    return CSTR2RVAL(atk_action_get_keybinding(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_action_set_description(self, i, desc)
    VALUE self, i, desc;
{
    return CBOOL2RVAL(atk_action_set_description(_SELF(self), NUM2INT(i), RVAL2CSTR(desc)));
}

void
Init_atk_action()
{
    VALUE mAction = G_DEF_INTERFACE(ATK_TYPE_ACTION, "Action", mAtk);

    rb_define_method(mAction, "do_action", rbatk_action_do_action, 1);
    rb_define_method(mAction, "n_actions", rbatk_action_get_n_actions, 0);
    rb_define_method(mAction, "get_description", rbatk_action_get_description, 1);
    rb_define_method(mAction, "get_name", rbatk_action_get_name, 1);
    rb_define_method(mAction, "get_localized_name", rbatk_action_get_localized_name, 1);
    rb_define_method(mAction, "get_keybinding", rbatk_action_get_keybinding, 1);
    rb_define_method(mAction, "set_description", rbatk_action_set_description, 2);
}
