/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:55:26 $

  Copyright (C) 2002,2003  KUBO Takehiro <kubo@jiubao.org>

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
