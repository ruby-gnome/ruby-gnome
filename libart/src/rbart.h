/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.h - header file for Art module of ruby

  $Author: mutoh $
  $Date: 2003/02/08 18:51:44 $

  Copyright (C) 2002,2003  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#ifndef _RBART_H_included
#define _RBART_H_included
#include <libart_lgpl/libart.h>
#include <ruby.h>

extern VALUE artAffine;
extern VALUE artSVP;
extern VALUE artBpath;
extern VALUE artVpath;
extern VALUE artVpathDash;
extern VALUE artCanvas;

extern double *rbart_get_art_affine(VALUE);
extern VALUE rbart_make_art_affine(double[6]);

extern void Init_art_affine(VALUE);

extern ArtSVP *get_art_svp(VALUE);
extern VALUE make_art_svp(ArtSVP *);

extern void Init_art_svp();

extern ArtBpath *get_art_bpath(VALUE);
extern VALUE make_art_bpath(ArtBpath *);

extern void Init_art_bpath();

extern ArtVpath *get_art_vpath(VALUE);
extern VALUE make_art_vpath(ArtVpath *);

extern ArtVpathDash *get_art_vpath_dash(VALUE);
extern VALUE make_art_vpath_dash(ArtVpathDash *);

extern void Init_art_vpath();
extern void Init_art_canvas();


#endif /* ! _RBART_H_included */
