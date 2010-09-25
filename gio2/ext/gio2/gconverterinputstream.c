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

#define _SELF(value) G_CONVERTER_INPUT_STREAM(RVAL2GOBJ(value))

static VALUE
converterinputstream_initialize(VALUE self, VALUE base_stream, VALUE converter)
{
        G_INITIALIZE(self,
                     g_converter_input_stream_new(RVAL2GINPUTSTREAM(base_stream),
                                                  RVAL2GCONVERTER(converter)));

        return Qnil;
}

static VALUE
converterinputstream_get_converter(VALUE self)
{
        return GOBJ2RVAL(g_converter_input_stream_get_converter(_SELF(self)));
}

void
Init_gconverterinputstream(VALUE glib)
{
        VALUE converterinputstream = G_DEF_CLASS(G_TYPE_CONVERTER_INPUT_STREAM, "ConverterInputStream", glib);

        rb_define_method(converterinputstream, "initialize", converterinputstream_initialize, 2);
        rb_define_method(converterinputstream, "converter", converterinputstream_get_converter, 0);
}
