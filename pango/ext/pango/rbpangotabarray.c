/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoarray.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoTabArray*)RVAL2BOXED(self, PANGO_TYPE_TAB_ARRAY))

static VALUE
rtab_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
                                    RVAL2GENUM(RARRAY_PTR(RARRAY_PTR(attr_ary)[i])[0], PANGO_TYPE_TAB_ALIGN),
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
rtab_get_size(self)
    VALUE self;
{
    return INT2NUM(pango_tab_array_get_size(_SELF(self)));
}

static VALUE
rtab_resize(self, size)
    VALUE self, size;
{
    pango_tab_array_resize(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
rtab_set_tab(self, tab_index, align, location)
    VALUE self, tab_index, align, location;
{
    pango_tab_array_set_tab(_SELF(self), NUM2INT(tab_index), RVAL2GENUM(align, PANGO_TYPE_TAB_ALIGN),
                            NUM2INT(location));
    return self;
}

static VALUE
rtab_get_tab(self, tab_index)
    VALUE self, tab_index;
{
    PangoTabAlign align;
    gint location;
    pango_tab_array_get_tab(_SELF(self), NUM2INT(tab_index),
                            &align, &location);
    return rb_ary_new3(2, GENUM2RVAL(align, PANGO_TYPE_TAB_ALIGN), INT2NUM(location));
}

static VALUE
rtab_get_tabs(self)
    VALUE self;
{
    PangoTabAlign* aligns;
    gint* locations;
    VALUE ary = rb_ary_new();
    PangoTabArray* tab_array = _SELF(self);
    gint i;

    pango_tab_array_get_tabs(tab_array, &aligns, &locations);

    for (i = 0; i < pango_tab_array_get_size(tab_array); i++){
        rb_ary_push(ary, rb_ary_new3(2, GENUM2RVAL(aligns[i], PANGO_TYPE_TAB_ALIGN), 
                                     INT2NUM(locations[i])));
    }
    return ary;
}

static VALUE
rtab_get_positions_in_pixels(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_tab_array_get_positions_in_pixels(_SELF(self)));
}

void
Init_pango_array()
{
    VALUE pTabArray = G_DEF_CLASS(PANGO_TYPE_TAB_ARRAY, "TabArray", mPango);
    rb_define_method(pTabArray, "initialize", rtab_initialize, -1);
    rb_define_method(pTabArray, "size", rtab_get_size, 0);
    rb_define_method(pTabArray, "resize", rtab_resize, 1);
    rb_define_method(pTabArray, "set_tab", rtab_set_tab, 3);
    rb_define_method(pTabArray, "get_tab", rtab_get_tab, 1);
    rb_define_method(pTabArray, "tabs", rtab_get_tabs, 0);
    rb_define_method(pTabArray, "positions_in_pixels?", rtab_get_positions_in_pixels, 0);

    G_DEF_SETTERS(pTabArray);

    /* PangoTabAlign */
    G_DEF_CLASS(PANGO_TYPE_TAB_ALIGN, "TabAlign", pTabArray);
    G_DEF_CONSTANTS(pTabArray, PANGO_TYPE_TAB_ALIGN, "PANGO_");    
}
