/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.h - header file for Art module of ruby

  $Author: tkubo $
  $Date: 2002/09/28 19:13:35 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#ifndef _RBART_H_included
#define _RBART_H_included
#include <libart_lgpl/libart.h>
#include <ruby.h>

extern VALUE mArt;
extern VALUE artAffine;

double *rbart_get_art_affine(VALUE);
VALUE rbart_make_art_affine(double[6]);

void Init_art_affine(VALUE);

#endif /* ! _RBART_H_included */
