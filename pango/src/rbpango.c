/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.c -

  $Author: mutoh $
  $Date: 2003/01/10 19:22:13 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

VALUE mPango;

void
Init_pango()
{
    mPango = rb_define_module("Pango");
    Init_pango_inits();
}
