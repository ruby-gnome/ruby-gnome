/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatk.c -

  $Author: mutoh $
  $Date: 2005/09/28 17:38:25 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "rbatk.h"

VALUE mAtk;

void
Init_atk()
{
    mAtk = rb_define_module("Atk");

    rb_define_const(mAtk, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(ATK_MAJOR_VERSION), 
                                INT2FIX(ATK_MINOR_VERSION), 
                                INT2FIX(ATK_MICRO_VERSION)));
    Init_atk_inits();
}
