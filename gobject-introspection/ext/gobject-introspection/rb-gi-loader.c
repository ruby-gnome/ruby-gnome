/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGILoader

static VALUE
rg_s_define_class(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_gtype, rb_name, rb_module;
    VALUE rb_options, rb_parent;
    GType gtype;

    rb_scan_args(argc, argv, "31", &rb_gtype, &rb_name, &rb_module, &rb_options);
    rbg_scan_options(rb_options,
                     "parent", &rb_parent,
                     NULL);

    gtype = NUM2ULONG(rb_to_int(rb_gtype));
    return G_DEF_CLASS_WITH_PARENT(gtype, RVAL2CSTR(rb_name),
                                   rb_module, rb_parent);
}

void
rb_gi_loader_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_class_under(rb_mGI, "Loader", rb_cObject);

    RG_DEF_SMETHOD(define_class, -1);
}
