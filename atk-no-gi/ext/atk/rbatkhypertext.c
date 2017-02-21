/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE mHypertext
#define _SELF(s) (RVAL2ATKHYPERTEXT(s))

static VALUE
rg_get_link(VALUE self, VALUE link_index)
{
    return GOBJ2RVAL(atk_hypertext_get_link(_SELF(self), NUM2INT(link_index)));
}

static VALUE
rg_n_links(VALUE self)
{
    return INT2NUM(atk_hypertext_get_n_links(_SELF(self)));
}

static VALUE
rg_link_index(VALUE self, VALUE char_index)
{
    return INT2NUM(atk_hypertext_get_link_index(_SELF(self), NUM2INT(char_index)));
}

void
Init_atk_hypertext(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_HYPERTEXT, "Hypertext", mAtk);

    RG_DEF_METHOD(get_link, 1);
    RG_DEF_METHOD(n_links, 0);
    RG_DEF_METHOD(link_index, 1);
}
