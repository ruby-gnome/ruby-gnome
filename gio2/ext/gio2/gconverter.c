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

#define _SELF(value) G_CONVERTER(RVAL2GOBJ(value))

#define RVAL2GCONVERTERFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_CONVERTER_FLAGS)

#define RVAL2GCONVERTERFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GCONVERTERFLAGS, \
                               G_CONVERTER_NO_FLAGS)

#define GCONVERTERRESULT2RVAL(value) \
        GFLAGS2RVAL((value), G_TYPE_CONVERTER_RESULT)

/* TODO: Look at gconverteroutputstream for implementing this. */
/* TODO: Actually, is there any point in implementing this? */
static VALUE
converter_convert(int argc, VALUE *argv, VALUE self)
{
        VALUE input, rbflags;
        GConverter *converter;
        gsize input_remaining, output_size, output_remaining, bytes_read, bytes_written;
        GConverterFlags flags;
        /* TODO: Use a Ruby string instead. */
        char *output_base, *output;
        GError *error = NULL;
        GConverterResult result;

        rb_scan_args(argc, argv, "11", &input, &rbflags);
        converter = _SELF(self);
        StringValue(input);
        input_remaining = RSTRING_LEN(input);
        flags = RVAL2GCONVERTERFLAGSDEFAULT(rbflags);
        output_remaining = output_size = BUFSIZ;
        output_base = output = ALLOCA_N(char, output_size);
        do {
                result = g_converter_convert(converter,
                                             RSTRING_PTR(input),
                                             input_remaining,
                                             output,
                                             output_remaining,
                                             flags,
                                             &bytes_read,
                                             &bytes_written,
                                             &error);
                switch (result) {
                case G_CONVERTER_ERROR:
                        switch (error->code) {
                        case G_IO_ERROR_NO_SPACE:
                                /* TODO: Expand the buffer? */
                                break;
                        case G_IO_ERROR_PARTIAL_INPUT:
                                /* TODO: How do we request/tell the user that
                                 * they need to provide more data? */
                                break;
                        default:
                                break;
                        }
                        rbgio_raise_error(error);
                        break;
                case G_CONVERTER_CONVERTED:
                case G_CONVERTER_FLUSHED:
                        input_remaining -= bytes_read;
                        input += bytes_read;
                        output_remaining -= bytes_written;
                        output += bytes_written;
                        /* TODO: Do we need to do anything else? */
                        break;
                case G_CONVERTER_FINISHED:
                        /* TODO: Do we simply break, or do we need to consider
                         * something else? */
                        break;
                default:
                        break;
                }
        } while (input_remaining > 0);

        return CSTR2RVAL(output_base);
}

static VALUE
converter_reset(VALUE self)
{
        g_converter_reset(_SELF(self));

        return self;
}

void
Init_gconverter(VALUE glib)
{
        VALUE converter = G_DEF_INTERFACE(G_TYPE_CONVERTER, "Converter", glib);

        rb_define_method(converter, "convert", converter_convert, -1);
        rb_define_method(converter, "reset", converter_reset, 0);
}
