/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014  Ruby-GNOME2 Project Team
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

#include "rb-gio2.h"

#define RG_TARGET_NAMESPACE mPollableSource

#define _SELF(self) ((GSource *)RVAL2BOXED(self, G_TYPE_SOURCE))

static gboolean
source_func(GObject *pollable_stream, gpointer user_data)
{
    VALUE callback = (VALUE)user_data;
    ID id_call;
    id_call = rb_intern("call");
    return RVAL2CBOOL(rb_funcall(callback, id_call,
                                 1, GOBJ2RVAL(pollable_stream)));
}

static VALUE
rg_set_callback(VALUE self)
{
    VALUE callback;

    callback = rb_block_proc();
    G_RELATIVE(self, callback);
    g_source_set_callback(_SELF(self),
                          (GSourceFunc)source_func,
                          (gpointer)callback,
                          (GDestroyNotify)NULL);
    return self;
}

void
rb_gio2_init_pollable_source (VALUE mGio)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "PollableSource");

    RG_DEF_METHOD(set_callback, 0);
}
