/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  This file is devided from rbgnome.c.
  rbgnome.c -
  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
  Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
************************************************/

#include "rbbonobo.h"

VALUE mBonobo;

void
Init_bonobo2()
{
   mBonobo = rb_define_module("Bonobo");
   
   Init_bonobo_inits();
}
