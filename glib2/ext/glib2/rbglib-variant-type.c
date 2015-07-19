/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rbgprivate.h"
#include "rbglib.h"

#define RG_TARGET_NAMESPACE mVariantType

static VALUE
rg_s_valid_p(G_GNUC_UNUSED VALUE klass, VALUE string)
{
    gboolean is_valid;

    StringValueCStr(string);
    is_valid = g_variant_type_string_is_valid(StringValueCStr(string));

    return CBOOL2RVAL(is_valid);
}

void
Init_glib_variant_type(void)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(G_TYPE_VARIANT_TYPE, "VariantType", mGLib);

    RG_DEF_SMETHOD_P(valid, 1);
}
