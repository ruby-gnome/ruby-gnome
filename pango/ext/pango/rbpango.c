/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.c -

  $Author: mutoh $
  $Date: 2005/09/28 18:04:21 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

VALUE mPango;

void
Init_pango()
{
    mPango = rb_define_module("Pango");

    rb_define_const(mPango, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(PANGO_MAJOR_VERSION), 
                                INT2FIX(PANGO_MINOR_VERSION), 
                                INT2FIX(PANGO_MICRO_VERSION)));
    Init_pango_inits();
}
