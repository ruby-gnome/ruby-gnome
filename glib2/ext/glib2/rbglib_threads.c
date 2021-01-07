/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cThread

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_s_init(VALUE self)
{
    return self;
}

static VALUE
rg_s_supported_p(G_GNUC_UNUSED VALUE self)
{
    return Qtrue;

}

void
Init_glib_threads(void)
{
    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Thread", rb_cObject);

    RG_DEF_SMETHOD(init, 0);
    RG_DEF_SMETHOD_P(supported, 0);
}
