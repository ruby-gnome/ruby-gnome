/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2002/07/26 14:31:33 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "global.h"

void Init_glib2()
{
    mGLib = rb_define_module("GLib");
    Init_gobject();
}
