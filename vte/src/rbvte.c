/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbvte.c -

  $Author: ktou $
  $Date: 2006/05/17 12:40:47 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbvte.h"

void
Init_vte(void)
{
    VALUE mVte;

    mVte = rb_define_module("Vte");

    rb_define_const(mVte, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(VTE_MAJOR_VERSION),
                                INT2FIX(VTE_MINOR_VERSION),
                                INT2FIX(VTE_MICRO_VERSION)));

    Init_vte_access(mVte);
    Init_vte_reaper(mVte);
    Init_vte_terminal(mVte);
}
