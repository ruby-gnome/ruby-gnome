/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.h - header file for Art module of ruby

  $Author: tkubo $
  $Date: 2002/09/28 14:27:08 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#ifndef _RBART_H_included
#define _RBART_H_included
#include <libart_lgpl/libart.h>
#include <ruby.h>

extern VALUE mArt;
extern VALUE artAffine;

double *get_art_affine(VALUE);
VALUE make_art_affine(double[6]);

void Init_art_affine(VALUE);

#endif /* ! _RBART_H_included */
