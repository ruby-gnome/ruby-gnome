/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cPageTransition

#define RVAL2TRANS(obj) (RVAL2POPPLERPAGETRANSITION(obj))

#define TT2RVAL(obj) (POPPLERPAGETRANSITIONTYPE2RVAL(obj))
#define RVAL2TT(obj) (RVAL2POPPLERPAGETRANSITIONTYPE(obj))
#define TA2RVAL(obj) (POPPLERPAGETRANSITIONALIGNMENT2RVAL(obj))
#define RVAL2TA(obj) (RVAL2POPPLERPAGETRANSITIONALIGNMENT(obj))
#define TD2RVAL(obj) (POPPLERPAGETRANSITIONDIRECTION2RVAL(obj))
#define RVAL2TD(obj) (RVAL2POPPLERPAGETRANSITIONDIRECTION(obj))

DEF_ACCESSOR(page_trans, type, RVAL2TRANS, RVAL2TT, TT2RVAL)
DEF_ACCESSOR(page_trans, alignment, RVAL2TRANS, RVAL2TA, TA2RVAL)
DEF_ACCESSOR(page_trans, direction, RVAL2TRANS, RVAL2TD, TD2RVAL)
DEF_ACCESSOR(page_trans, duration, RVAL2TRANS, NUM2INT, INT2NUM)
DEF_ACCESSOR(page_trans, angle, RVAL2TRANS, NUM2INT, INT2NUM)
DEF_ACCESSOR(page_trans, scale, RVAL2TRANS, NUM2DBL, rb_float_new)
DEF_ACCESSOR(page_trans, rectangular, RVAL2TRANS, RVAL2CBOOL, CBOOL2RVAL)

void
Init_poppler_pagetransition(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION,
                                  "PageTransition", mPoppler);

    rb_define_method(RG_TARGET_NAMESPACE, "type", page_trans_get_type, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "alignment", page_trans_get_alignment, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "direction", page_trans_get_direction, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "duration", page_trans_get_duration, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "angle", page_trans_get_angle, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "scale", page_trans_get_scale, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "rectangular",
                     page_trans_get_rectangular, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "set_type", page_trans_set_type, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_alignment",
                     page_trans_set_alignment, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_direction",
                     page_trans_set_direction, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_duration",
                     page_trans_set_duration, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_angle", page_trans_set_angle, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_scale", page_trans_set_scale, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_rectangular",
                     page_trans_set_rectangular, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
