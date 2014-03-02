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

#define RG_TARGET_NAMESPACE mAsyncResult
#define _SELF(value) RVAL2GASYNCRESULT(value)

/* NOTE: g_async_result_get_user_data isn't of any use from Ruby. */

static VALUE
rg_source_object(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_async_result_get_source_object(_SELF(self)));
}

void
Init_gasyncresult(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_ASYNC_RESULT, "AsyncResult", mGio);

        RG_DEF_METHOD(source_object, 0);
}
