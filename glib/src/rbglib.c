/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2002/08/10 16:07:09 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "global.h"

VALUE mGLib;

extern void Init_utils_int64();
extern void Init_gobject();

void Init_glib2()
{
    mGLib = rb_define_module("GLib");
    Init_utils_int64();
    Init_gobject();
}
