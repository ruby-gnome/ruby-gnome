/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatk.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:41:18 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "rbatk.h"

VALUE mAtk;

void
Init_atk()
{
    mAtk = rb_define_module("Atk");

    rb_define_const(mAtk, "VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(ATK_MAJOR_VERSION), 
                                INT2FIX(ATK_MINOR_VERSION), 
                                INT2FIX(ATK_MICRO_VERSION)));
    rb_define_const(mAtk, "MAJOR_VERSION", INT2FIX(ATK_MAJOR_VERSION));
    rb_define_const(mAtk, "MINOR_VERSION", INT2FIX(ATK_MINOR_VERSION));
    rb_define_const(mAtk, "MICRO_VERSION", INT2FIX(ATK_MICRO_VERSION));

    Init_atk_inits();
}
