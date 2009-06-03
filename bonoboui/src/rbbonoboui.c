/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonoboui.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonoboui.h"

extern void Init_bonoboui_inits();

VALUE mBonobo;
VALUE mBonoboUI;

void
Init_bonoboui2()
{
    mBonobo = rb_define_module("Bonobo");
    mBonoboUI = rb_define_module_under(mBonobo, "UI");
   
    rb_define_const(mBonoboUI, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(BONOBOUI_MAJOR_VERSION), 
                                INT2FIX(BONOBOUI_MINOR_VERSION), 
                                INT2FIX(BONOBOUI_MICRO_VERSION)));


    Init_bonoboui_inits();
}
