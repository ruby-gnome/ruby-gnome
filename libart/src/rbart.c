/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: mutoh $
  $Date: 2003/01/04 06:04:28 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

void
Init_libart2()
{
    VALUE mArt = rb_define_module("Art");

    Init_art_affine(mArt);
    Init_art_bpath(mArt);
    Init_art_canvas(mArt);
    Init_art_svp(mArt);
    Init_art_vpath(mArt);
}
