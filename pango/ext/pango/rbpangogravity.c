/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbpangoprivate.h"

#if PANGO_CHECK_VERSION(1,16,0)

#define RG_TARGET_NAMESPACE mGravity

static VALUE
rg_s_to_rotation(G_GNUC_UNUSED VALUE self, VALUE gravity)
{
    return rb_float_new(pango_gravity_to_rotation(RVAL2PANGOGRAVITY(gravity)));
}

static VALUE
rg_s_vertical_p(G_GNUC_UNUSED VALUE self, VALUE gravity)
{
    return CBOOL2RVAL(PANGO_GRAVITY_IS_VERTICAL(RVAL2PANGOGRAVITY(gravity)));
}
#endif

void
Init_pango_gravity(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,16,0)
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mPango, "Gravity");

    RG_DEF_SMETHOD(to_rotation, 1);
    RG_DEF_SMETHOD_P(vertical, 1);

    G_DEF_CLASS(PANGO_TYPE_GRAVITY, "Gravity", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_GRAVITY, "PANGO_GRAVITY_");
    G_DEF_CLASS(PANGO_TYPE_GRAVITY_HINT, "Hint", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_GRAVITY_HINT, "PANGO_GRAVITY_");
#endif
}
