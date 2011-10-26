/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cState
#define _SELF(s) (ATK_STATE(RVAL2GOBJ(s)))

static VALUE
rbatkstate_s_type_register(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_state_type_register(RVAL2CSTR(name)), ATK_TYPE_STATE_TYPE);
}

/* We don't need this.
G_CONST_RETURN gchar* atk_state_type_get_name
                                            (AtkStateType type);
*/

static VALUE
rbatkstate_s_for_name(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_state_type_for_name(RVAL2CSTR(name)), ATK_TYPE_STATE_TYPE);
}

void
Init_atk_state(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_STATE_TYPE, "State", mAtk);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "type_register", rbatkstate_s_type_register, 1);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "for_name", rbatkstate_s_for_name, 1);
    G_DEF_CONSTANTS(mAtk, ATK_TYPE_STATE_TYPE, "ATK_");
}
