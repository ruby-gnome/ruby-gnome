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

#define _SELF(s) (ATK_HYPERLINK(RVAL2GOBJ(s)))

static VALUE
rbatk_hl_get_uri(VALUE self, VALUE i)
{
    return CSTR2RVAL(atk_hyperlink_get_uri(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_hl_get_object(VALUE self, VALUE i)
{
    return GOBJ2RVAL(atk_hyperlink_get_object(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatk_hl_is_valid(VALUE self)
{
    return CBOOL2RVAL(atk_hyperlink_is_valid(_SELF(self)));
}

#ifdef HAVE_ATK_HYPERLINK_IS_INLINE
static VALUE
rbatk_hl_is_inline(VALUE self)
{
    return CBOOL2RVAL(atk_hyperlink_is_inline(_SELF(self)));
}
#endif

static VALUE
rbatk_hl_get_n_anchors(VALUE self)
{
    return INT2NUM(atk_hyperlink_get_n_anchors(_SELF(self)));
}

void
Init_atk_hyperlink(void)
{
    VALUE hl = G_DEF_CLASS(ATK_TYPE_HYPERLINK, "Hyperlink", mAtk);

    rb_define_method(hl, "get_uri", rbatk_hl_get_uri, 1);
    rb_define_method(hl, "get_object", rbatk_hl_get_object, 1);
    rb_define_method(hl, "valid?", rbatk_hl_is_valid, 0);
#ifdef HAVE_ATK_HYPERLINK_IS_INLINE
    rb_define_method(hl, "inline?", rbatk_hl_is_inline, 0);
#endif
    rb_define_method(hl, "n_anchors", rbatk_hl_get_n_anchors, 0);
}
