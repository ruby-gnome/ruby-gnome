/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.c -

  $Author: mutoh $
  $Date: 2002/09/12 16:38:16 $

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

    rb_define_const(mGLib, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GLIB_MAJOR_VERSION),
                                INT2FIX(GLIB_MINOR_VERSION),
                                INT2FIX(GLIB_MICRO_VERSION)));
    rb_define_const(mGLib, "MAJOR_VERSION", INT2FIX(GLIB_MAJOR_VERSION));
    rb_define_const(mGLib, "MINOR_VERSION", INT2FIX(GLIB_MINOR_VERSION));
    rb_define_const(mGLib, "MICRO_VERSION", INT2FIX(GLIB_MICRO_VERSION));

    Init_utils_int64();
    Init_gobject();
}
