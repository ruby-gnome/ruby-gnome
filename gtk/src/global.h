/* -*- c-file-style: "ruby" -*- */
/************************************************

  global.h -

  $Author: sakai $
  $Date: 2003/02/17 11:29:19 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef _RBGTK_GLOBAL_H
#define _RBGTK_GLOBAL_H

#include "rbgtk.h"

extern void Init_gtk_gdk();
extern void Init_gtk_gtk();
extern void Init_gtk_inits();

extern ID id_relative_callbacks;
extern ID id_call;

#endif /* _RBGTK_GLOBAL_H */
