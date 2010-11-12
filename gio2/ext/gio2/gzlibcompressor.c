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

#define _SELF(value) G_ZLIB_COMPRESSOR(RVAL2GOBJ(value))

#define RVAL2ZLIBCOMPRESSORLEVELDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), FIX2INT, -1)

static VALUE
zlibcompressor_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE format, level;

        rb_scan_args(argc, argv, "02", &format, &level);
        G_INITIALIZE(self,
                     g_zlib_compressor_new(RVAL2GZLIBCOMPRESSORFORMATDEFAULT(format),
                                           RVAL2GZLIBCOMPRESSORFORMATDEFAULT(level)));

        return Qnil;
}

void
Init_gzlibcompressor(VALUE glib)
{
        VALUE zlibcompressor = G_DEF_CLASS(G_TYPE_ZLIB_COMPRESSOR, "ZlibCompressor", glib);

        G_DEF_CLASS(G_TYPE_ZLIB_COMPRESSOR_FORMAT, "Format", zlibcompressor);
        G_DEF_CONSTANTS(zlibcompressor, G_TYPE_ZLIB_COMPRESSOR_FORMAT, "G_ZLIB_");

        rb_define_method(zlibcompressor, "initialize", zlibcompressor_initialize, -1);
}
