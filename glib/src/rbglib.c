/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2002/06/21 18:26:11 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "global.h"

void Init_glib2()
{
    mGLib = rb_define_module("GLib");
    rb_ivar_set(mGLib, id_relatives, Qnil);
    rb_ivar_set(mGLib, id_relative_callbacks, Qnil);

    Init_gobject();
}
