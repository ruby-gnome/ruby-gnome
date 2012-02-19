/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTabArray
#define _SELF(self) (RVAL2PANGOTABARRAY(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE size, positions_in_pixels, attr_ary;
    PangoTabArray *array;
    int i;

    rb_scan_args(argc, argv, "2*", &size, &positions_in_pixels, &attr_ary);

    array = pango_tab_array_new(NUM2INT(size), 
                                RVAL2CBOOL(positions_in_pixels));
    G_INITIALIZE(self, array);

    if (! NIL_P(attr_ary)){
        for (i = 0; i < RARRAY_LEN(attr_ary); i++) {
            pango_tab_array_set_tab(array, i, 
                                    RVAL2PANGOTABALIGN(RARRAY_PTR(RARRAY_PTR(attr_ary)[i])[0]),
                                    FIX2INT(RARRAY_PTR(RARRAY_PTR(attr_ary)[i])[1]));
        }
    }

    return Qnil;
}
/* This is implemented in rtab_initialize.
PangoTabArray* pango_tab_array_new_with_positions
                                            (gint size,
                                             gboolean positions_in_pixels,
                                             PangoTabAlign first_alignment,
                                             gint first_position,
                                             ...);
*/
static VALUE
rg_size(VALUE self)
{
    return INT2NUM(pango_tab_array_get_size(_SELF(self)));
}

static VALUE
rg_resize(VALUE self, VALUE size)
{
    pango_tab_array_resize(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
rg_set_tab(VALUE self, VALUE tab_index, VALUE align, VALUE location)
{
    pango_tab_array_set_tab(_SELF(self), NUM2INT(tab_index), RVAL2PANGOTABALIGN(align),
                            NUM2INT(location));
    return self;
}

static VALUE
rg_get_tab(VALUE self, VALUE tab_index)
{
    PangoTabAlign align;
    gint location;
    pango_tab_array_get_tab(_SELF(self), NUM2INT(tab_index),
                            &align, &location);
    return rb_ary_new3(2, PANGOTABALIGN2RVAL(align), INT2NUM(location));
}

static VALUE
rg_tabs(VALUE self)
{
    PangoTabAlign* aligns;
    gint* locations;
    VALUE ary = rb_ary_new();
    PangoTabArray* tab_array = _SELF(self);
    gint i;

    pango_tab_array_get_tabs(tab_array, &aligns, &locations);

    for (i = 0; i < pango_tab_array_get_size(tab_array); i++){
        rb_ary_push(ary, rb_ary_new3(2, PANGOTABALIGN2RVAL(aligns[i]), 
                                     INT2NUM(locations[i])));
    }
    return ary;
}

static VALUE
rg_positions_in_pixels_p(VALUE self)
{
    return CBOOL2RVAL(pango_tab_array_get_positions_in_pixels(_SELF(self)));
}

void
Init_pango_array(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_TAB_ARRAY, "TabArray", mPango);
    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(resize, 1);
    RG_DEF_METHOD(set_tab, 3);
    RG_DEF_METHOD(get_tab, 1);
    RG_DEF_METHOD(tabs, 0);
    RG_DEF_METHOD_P(positions_in_pixels, 0);

    /* PangoTabAlign */
    G_DEF_CLASS(PANGO_TYPE_TAB_ALIGN, "TabAlign", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_TAB_ALIGN, "PANGO_");    
}
