/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cApplication

static VALUE
rg_open_signal_func(G_GNUC_UNUSED guint num, const GValue *values)
{
    VALUE rb_files;
    GFile **files;
    gint i, n_files;

    files = g_value_get_pointer(&(values[1]));
    n_files = g_value_get_int(&(values[2]));
    rb_files = rb_ary_new2(n_files);
    for (i = 0; i < n_files; i++) {
        GFile *file = files[i];
        rb_ary_push(rb_files, GOBJ2RVAL(file));
    }

    return rb_ary_new3(3,
                       GVAL2RVAL(&values[0]),
                       rb_files,
                       GVAL2RVAL(&values[3]));
}

void
rb_gio2_init_application (VALUE mGio)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_const_get(mGio, rb_intern("Application"));

    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "open", rg_open_signal_func);
}
