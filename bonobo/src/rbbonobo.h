/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo.h -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>

  This file is devided from rbgnome.c.
  rbgnome.c -
  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
  Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
************************************************/

#ifndef _RBBONOBO_H_included
#define _RBBONOBO_H_included

#define ENABLE_NLS 1

#include <bonobo.h>
#include "rbgtk.h"

extern VALUE mBonobo;
extern void Init_bonobo_inits();

#define CBOOL2RVAL(b)   ((b) ? Qtrue : Qfalse)

#endif /* ! _RBBONOBO_H_included */
