/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"
#include <gdk/gdkkeysyms.h>

#define RG_TARGET_NAMESPACE mGdkKeyval

static VALUE
rg_m_to_name(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    gchar* name = gdk_keyval_name(NUM2UINT(keyval));
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
rg_m_from_name(G_GNUC_UNUSED VALUE self, VALUE keyval_name)
{
    return UINT2NUM(gdk_keyval_from_name(RVAL2CSTR(keyval_name)));
}

static VALUE
rg_m_upper_p(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    return CBOOL2RVAL(gdk_keyval_is_upper(NUM2UINT(keyval)));
}

static VALUE
rg_m_lower_p(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    return CBOOL2RVAL(gdk_keyval_is_lower(NUM2UINT(keyval)));
}

static VALUE
rg_m_convert_case(G_GNUC_UNUSED VALUE self, VALUE symbol)
{
    guint upper, lower;
    gdk_keyval_convert_case(NUM2UINT(symbol), &lower, &upper);
    return rb_ary_new3(2, UINT2NUM(lower), UINT2NUM(upper));
}

static VALUE
rg_m_to_upper(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    return INT2NUM(gdk_keyval_to_upper(NUM2UINT(keyval)));
}

static VALUE
rg_m_to_lower(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    return INT2NUM(gdk_keyval_to_lower(NUM2UINT(keyval)));
}

static VALUE
rg_m_to_unicode(G_GNUC_UNUSED VALUE self, VALUE keyval)
{
    return UINT2NUM(gdk_keyval_to_unicode(NUM2UINT(keyval)));
}

static VALUE
rg_m_from_unicode(G_GNUC_UNUSED VALUE self, VALUE wc)
{
    VALUE unicode;
    if (TYPE(wc) == T_STRING) {
        unicode = NUM2UINT(rb_funcall(wc, rb_intern("[]"), 1, INT2FIX(0)));
    } else {
        unicode = NUM2UINT(wc);
    }
    return UINT2NUM(gdk_unicode_to_keyval(unicode));
}

void
Init_gtk_gdk_keyval(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Keyval");
    RG_DEF_MODFUNC(to_name, 1);
    RG_DEF_MODFUNC(from_name, 1);
    RG_DEF_MODFUNC_P(upper, 1);
    RG_DEF_MODFUNC_P(lower, 1);
    RG_DEF_MODFUNC(convert_case, 1);
    RG_DEF_MODFUNC(to_upper, 1);
    RG_DEF_MODFUNC(to_lower, 1);
    RG_DEF_MODFUNC(to_unicode, 1);
    RG_DEF_MODFUNC(from_unicode, 1);

#include "rbgdkkeysyms.h"
}
