/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatk.c -

  $Author: mutoh $
  $Date: 2003/12/04 18:06:17 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

VALUE mAtk;

void
Init_atk()
{
    mAtk = rb_define_module("Atk");
    Init_atk_inits();
}
