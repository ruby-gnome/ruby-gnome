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

#define RG_TARGET_NAMESPACE mLoadableIcon
#define _SELF(value) RVAL2GLOADABLEICON(value)

static VALUE
rg_load(int argc, VALUE *argv, VALUE self)
{
        VALUE size, cancellable;
        char *type;
        GError *error = NULL;
        GInputStream *stream;

        rb_scan_args(argc, argv, "11", &size, &cancellable);
        stream = g_loadable_icon_load(_SELF(self),
                                      NUM2INT(size),
                                      &type,
                                      RVAL2GCANCELLABLE(cancellable),
                                      &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(stream), CSTR2RVAL_FREE(type));
}

static VALUE
rg_load_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbsize, rbcancellable, block;
        int size;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "11&", &rbsize, &rbcancellable, &block);
        size = NUM2INT(rbsize);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_loadable_icon_load_async(_SELF(self),
                                   size,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

static VALUE
rg_load_finish(VALUE self, VALUE result)
{
        char *type;
        GError *error = NULL;
        GInputStream *stream;

        stream = g_loadable_icon_load_finish(_SELF(self),
                                             RVAL2GASYNCRESULT(result),
                                             &type,
                                             &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(stream), CSTR2RVAL_FREE(type));
}

void
Init_gloadableicon(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_LOADABLE_ICON, "LoadableIcon", mGio);

        RG_DEF_METHOD(load, -1);
        RG_DEF_METHOD(load_async, -1);
        RG_DEF_METHOD(load_finish, 1);
}
