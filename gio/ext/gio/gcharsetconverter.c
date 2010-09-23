/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) G_CHARSET_CONVERTER(RVAL2GOBJ(value))

static VALUE
charsetconverter_initialize(VALUE self, VALUE to, VALUE from)
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
charsetconverter_set_use_fallback(VALUE self, VALUE use_fallback)
{
        g_charset_converter_set_use_fallback(_SELF(self), RVAL2CBOOL(use_fallback));

        return self;
}

static VALUE
charsetconverter_get_use_fallback(VALUE self)
{
        return CBOOL2RVAL(g_charset_converter_get_use_fallback(_SELF(self)));
}

static VALUE
charsetconverter_get_num_fallbacks(VALUE self)
{
        return GUINT2RVAL(g_charset_converter_get_num_fallbacks(_SELF(self)));
}

void
Init_gcharsetconverter(VALUE glib)
{
        VALUE charsetconverter = G_DEF_CLASS(G_TYPE_CHARSET_CONVERTER, "CharsetConverter", glib);

        rb_define_method(charsetconverter, "initialize", charsetconverter_initialize, 2);
        rb_define_method(charsetconverter, "set_use_fallback", charsetconverter_set_use_fallback, 1);
        G_DEF_SETTER(charsetconverter, "use_fallback");
        rb_define_method(charsetconverter, "use_fallback?", charsetconverter_get_use_fallback, 0);
        rb_define_method(charsetconverter, "num_fallbacks", charsetconverter_get_num_fallbacks, 0);
}
