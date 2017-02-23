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

#define RG_TARGET_NAMESPACE mSelection
#define _SELF(s) (RVAL2ATKSELECTION(s))

static VALUE
rg_add_selection(VALUE self, VALUE i)
{
    gboolean ret = atk_selection_add_selection(_SELF(self), NUM2INT(i));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't add selection");
    return self;
}

static VALUE
rg_clear_selection(VALUE self)
{
    gboolean ret = atk_selection_clear_selection(_SELF(self));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't clear selection");
    return self;
}

static VALUE
rg_ref_selection(VALUE self, VALUE i)
{
    return GOBJ2RVAL(atk_selection_ref_selection(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_selection_count(VALUE self)
{
    return INT2NUM(atk_selection_get_selection_count(_SELF(self)));
}

static VALUE
rg_child_selected_p(VALUE self, VALUE i)
{
    return CBOOL2RVAL(atk_selection_is_child_selected(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_remove_selection(VALUE self, VALUE i)
{
    gboolean ret = atk_selection_remove_selection(_SELF(self), NUM2INT(i));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't remove selection");
    return self;
}

static VALUE
rg_select_all_selection(VALUE self)
{
    return CBOOL2RVAL(atk_selection_select_all_selection(_SELF(self)));
}

void
Init_atk_selection(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_SELECTION, "Selection", mAtk);

    RG_DEF_METHOD(add_selection, 1);
    RG_DEF_METHOD(clear_selection, 0);
    RG_DEF_METHOD(ref_selection, 1);
    RG_DEF_METHOD(selection_count, 0);
    RG_DEF_METHOD_P(child_selected, 1);
    RG_DEF_METHOD(remove_selection, 1);
    RG_DEF_METHOD(select_all_selection, 0);
}
