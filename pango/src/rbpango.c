/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.c -

  $Author: mutoh $
  $Date: 2005/01/28 09:24:45 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

VALUE mPango;

void
Init_pango()
{
    mPango = rb_define_module("Pango");

    rb_define_const(mPango, "VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(PANGO_MAJOR_VERSION), 
                                INT2FIX(PANGO_MINOR_VERSION), 
                                INT2FIX(PANGO_MICRO_VERSION)));
    rb_define_const(mPango, "MAJOR_VERSION", INT2FIX(PANGO_MAJOR_VERSION));
    rb_define_const(mPango, "MINOR_VERSION", INT2FIX(PANGO_MINOR_VERSION));
    rb_define_const(mPango, "MICRO_VERSION", INT2FIX(PANGO_MICRO_VERSION));

    Init_pango_inits();
}
