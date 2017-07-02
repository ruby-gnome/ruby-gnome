/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017  Ruby-GNOME2 Project Team
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

#include "rb-pango-private.h"

#define RG_TARGET_NAMESPACE mScale

void
rbpango_scale_init(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module_under(mPango, "Scale");

    rb_define_const(RG_TARGET_NAMESPACE,
                    "XX_SMALL",
                    rb_float_new(PANGO_SCALE_XX_SMALL));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "X_SMALL",
                    rb_float_new(PANGO_SCALE_X_SMALL));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "SMALL",
                    rb_float_new(PANGO_SCALE_SMALL));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "MEDIUM",
                    rb_float_new(PANGO_SCALE_MEDIUM));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "LARGE",
                    rb_float_new(PANGO_SCALE_LARGE));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "X_LARGE",
                    rb_float_new(PANGO_SCALE_X_LARGE));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "XX_LARGE",
                    rb_float_new(PANGO_SCALE_XX_LARGE));
}
