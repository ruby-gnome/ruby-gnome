/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2002/06/10 18:51:43 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "global.h"

VALUE gError;

void Init_glib2()
{
    mGLib = rb_define_module("GLib");
    rb_ivar_set(mGLib, id_relatives, Qnil);
    rb_ivar_set(mGLib, id_relative_callbacks, Qnil);
    gError = rb_define_class_under(mGLib, "Error", rb_eRuntimeError);

    Init_gobject();
}
