/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: tkubo $
  $Date: 2002/09/20 15:29:53 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

VALUE mArt;

void
Init_libart2()
{
    mArt = rb_define_module("Art");

    Init_art_affine();
}
