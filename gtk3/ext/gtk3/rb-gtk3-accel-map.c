/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2022  Ruby-GNOME Project Team
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

#include "rb-gtk3-private.h"

static void
accel_map_foreach_func(gpointer data,
                       const gchar *path,
                       guint key,
                       GdkModifierType accel_mods,
                       gboolean changed)
{
    VALUE block = (VALUE)data;
    ID id_call;
    CONST_ID(id_call, "call");
    rb_funcall(block,
               id_call,
               4,
               CSTR2RVAL(path),
               UINT2NUM(key),
               GFLAGS2RVAL(accel_mods, GDK_TYPE_MODIFIER_TYPE),
               CBOOL2RVAL(changed));
}

static VALUE
rg_s_each(int argc, VALUE *args, VALUE self)
{
    RETURN_ENUMERATOR(self, argc, args);

    VALUE options = Qnil;
    VALUE block = Qnil;
    rb_scan_args(argc, args, ":&", &options, &block);

    ID keywords[1];
    VALUE values[1];
    CONST_ID(keywords[0], "filter");
    rb_get_kwargs(options, keywords, 0, 1, values);

    VALUE filter = values[0];
    if (filter == RUBY_Qundef || RB_NIL_P(filter)) {
        filter = RUBY_Qtrue;
    }
    if (RB_TEST(filter)) {
        gtk_accel_map_foreach((gpointer)block, accel_map_foreach_func);
    } else {
        gtk_accel_map_foreach_unfiltered((gpointer)block,
                                         accel_map_foreach_func);
    }
    return self;
}

void
rbgtk3_accel_map_init(void)
{
    VALUE mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    VALUE RG_TARGET_NAMESPACE = rb_const_get(mGtk, rb_intern("AccelMap"));

    RG_DEF_SMETHOD(each, -1);
}
