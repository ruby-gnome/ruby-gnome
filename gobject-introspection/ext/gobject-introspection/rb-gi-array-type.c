/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME Project Team
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

#include "rb-gi-private.h"

VALUE
rb_gi_array_type_to_ruby(GIArrayType type)
{
    if (type == (GIArrayType)-1) {
        return Qnil;
    } else {
        return GENUM2RVAL(type, G_TYPE_I_ARRAY_TYPE);
    }
}

const gchar *
rb_gi_array_type_to_string(GIArrayType type)
{
    switch (type) {
      case GI_ARRAY_TYPE_C:
        return "C";
      case GI_ARRAY_TYPE_ARRAY:
        return "GArray";
      case GI_ARRAY_TYPE_PTR_ARRAY:
        return "GPtrArray";
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        return "GByteArray";
      default:
        return "unknown";
    }
}

void
rb_gi_array_type_init(VALUE rb_mGI)
{
    G_DEF_CLASS(G_TYPE_I_ARRAY_TYPE, "ArrayType", rb_mGI);
}
