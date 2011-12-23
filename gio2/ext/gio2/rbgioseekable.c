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

#define RG_TARGET_NAMESPACE mSeekable
#define _SELF(value) G_SEEKABLE(RVAL2GOBJ(value))

#define RVAL2GSEEKTYPEDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, G_SEEK_CUR)

static VALUE
rg_tell(VALUE self)
{
        return GOFFSET2RVAL(g_seekable_tell(_SELF(self)));
}

static VALUE
rg_can_seek_p(VALUE self)
{
        return CBOOL2RVAL(g_seekable_can_seek(_SELF(self)));
}

static VALUE
rg_seek(int argc, VALUE *argv, VALUE self)
{
        VALUE offset, type, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "12", &offset, &type, &cancellable);
        if (!g_seekable_seek(_SELF(self),
                             RVAL2GOFFSET(offset),
                             RVAL2GSEEKTYPEDEFAULT(type),
                             RVAL2GCANCELLABLE(cancellable),
                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_can_truncate_p(VALUE self)
{
        return CBOOL2RVAL(g_seekable_can_truncate(_SELF(self)));
}

static VALUE
rg_truncate(int argc, VALUE *argv, VALUE self)
{
        VALUE offset, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &offset, &cancellable);
        if (!g_seekable_truncate(_SELF(self),
                                 RVAL2GOFFSET(offset),
                                 RVAL2GCANCELLABLE(cancellable),
                                 &error))
                rbgio_raise_error(error);

        return self;
}

void
Init_gseekable(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_SEEKABLE, "Seekable", mGio);

        RG_DEF_METHOD(tell, 0);
        RG_DEF_METHOD_P(can_seek, 0);
        RG_DEF_METHOD(seek, -1);
        RG_DEF_METHOD_P(can_truncate, 0);
        RG_DEF_METHOD(truncate, -1);
}
