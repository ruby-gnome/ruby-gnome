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

#define RG_TARGET_NAMESPACE cRole

static VALUE
rg_localized_name(G_GNUC_UNUSED VALUE self)
{
#ifdef HAVE_ATK_ROLE_GET_LOCALIZED_NAME
    return CSTR2RVAL(atk_role_get_localized_name(RVAL2ATKROLE(self)));
#else
    rb_warning("not supported in this version of ATK.");
    return Qnil;
#endif
}

static VALUE
rg_s_for_name(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return ATKROLE2RVAL(atk_role_for_name(RVAL2CSTR(name)));
}

void
Init_atk_object_role(VALUE cObject)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_ROLE, "Role", cObject);
    RG_DEF_METHOD(localized_name, 0);
    RG_DEF_SMETHOD(for_name, 1);
    G_DEF_CONSTANTS(cObject, ATK_TYPE_ROLE, "ATK_");
}
