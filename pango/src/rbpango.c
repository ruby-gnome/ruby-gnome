/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

VALUE mPango;

void
Init_pango()
{
    mPango = rb_define_module("Pango");
    Init_pango_inits();
}
