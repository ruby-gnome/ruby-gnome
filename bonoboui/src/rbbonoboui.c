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

   Init_bonoboui_inits();
}
