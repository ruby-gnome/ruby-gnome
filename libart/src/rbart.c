/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: tkubo $
  $Date: 2002/09/20 14:51:20 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

VALUE mArt;

void
Init_libart()
{
    mArt = rb_define_module("Art");

    Init_art_affine();
}
