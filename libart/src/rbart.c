/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: tkubo $
  $Date: 2002/09/28 14:27:08 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

void
Init_libart2()
{
    VALUE mArt = rb_define_module("Art");

    Init_art_affine(mArt);
}
