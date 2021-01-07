/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cBinding

#define _SELF(object) G_BINDING(RVAL2GOBJ(self))

static VALUE
rg_unbind(VALUE self)
{
    GBinding *binding = _SELF(self);
    GObject *source;
    VALUE rb_source;

    source = g_binding_get_source(binding);
    rb_source = GOBJ2RVAL(source);
    rbgobj_object_remove_relative(rb_source, self);
    g_binding_unbind(binding);

    return self;
}

void
Init_gobject_gbinding(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BINDING, "Binding", mGLib);

    RG_DEF_METHOD(unbind, 0);
}
