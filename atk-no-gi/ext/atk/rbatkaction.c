/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE mAction
#define _SELF(s) (RVAL2ATKACTION(s))

static VALUE
rg_do_action(VALUE self, VALUE i)
{
    return CBOOL2RVAL(atk_action_do_action(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_n_actions(VALUE self)
{
    return INT2NUM(atk_action_get_n_actions(_SELF(self)));
}

static VALUE
rg_get_description(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_action_get_description(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_get_name(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_action_get_name(_SELF(self), NUM2INT(i)));
}

#ifdef HAVE_ATK_ACTION_GET_LOCALIZED_NAME
static VALUE
rg_get_localized_name(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_action_get_localized_name(_SELF(self), NUM2INT(i)));
}
#endif

static VALUE
rg_get_keybinding(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_action_get_keybinding(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_set_description(VALUE self, VALUE i, VALUE desc)
{
    return CBOOL2RVAL(atk_action_set_description(_SELF(self), NUM2INT(i), RVAL2CSTR(desc)));
}

void
Init_atk_action(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_ACTION, "Action", mAtk);

    RG_DEF_METHOD(do_action, 1);
    RG_DEF_METHOD(n_actions, 0);
    RG_DEF_METHOD(get_description, 1);
    RG_DEF_METHOD(get_name, 1);
#ifdef HAVE_ATK_ACTION_GET_LOCALIZED_NAME
    RG_DEF_METHOD(get_localized_name, 1);
#endif
    RG_DEF_METHOD(get_keybinding, 1);
    RG_DEF_METHOD(set_description, 2);
}
