/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cCharsetConverter
#define _SELF(value) G_CHARSET_CONVERTER(RVAL2GOBJ(value))

static VALUE
rg_initialize(VALUE self, VALUE to, VALUE from)
{
        GError *error = NULL;
        GCharsetConverter *converter;

        converter = g_charset_converter_new(RVAL2CSTR(to),
                                            RVAL2CSTR(from),
                                            &error);
        if (converter == NULL)
                rbgio_raise_error(error);

        G_INITIALIZE(self, converter);

        return Qnil;
}

static VALUE
rg_num_fallbacks(VALUE self)
{
        return GUINT2RVAL(g_charset_converter_get_num_fallbacks(_SELF(self)));
}

void
Init_gcharsetconverter(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_CHARSET_CONVERTER, "CharsetConverter", mGio);

        RG_DEF_METHOD(initialize, 2);
        RG_DEF_METHOD(num_fallbacks, 0);
}
