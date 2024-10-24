/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2024  Ruby-GNOME Project Team
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

/* Deprecated. Use RB_LL2NUM() directly. */
VALUE
rbglib_int64_to_num(guint64 val)
{
    return RB_LL2NUM(val);
}

/* Deprecated. Use RB_ULL2NUM() directly. */
VALUE
rbglib_uint64_to_num(guint64 val)
{
    return RB_ULL2NUM(val);
}

/* Deprecated. Use RB_NUM2LL() directly. */
gint64
rbglib_num_to_int64(VALUE val)
{
    return RB_NUM2LL(val);
}

/* Deprecated. Use RB_NUM2ULL() directly. */
guint64
rbglib_num_to_uint64(VALUE val)
{
    return RB_NUM2ULL(val);
}
