/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf.h -

  $Author: mutoh $
  $Date: 2003/01/17 19:20:40 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include "rbgobject.h"

extern void Init_gdk_pixbuf_animation(VALUE mGLib);
extern void Init_gdk_pixdata(VALUE mGLib);
extern void Init_gdk_pixbuf_loader(VALUE mGLib);
