/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbvte-reaper.c -

  $Author: ktou $
  $Date: 2006/05/17 12:40:47 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbvte.h"

static VALUE
reaper_get(VALUE self)
{
    return GOBJ2RVAL(vte_reaper_get());
}

void
Init_vte_reaper(VALUE mVte)
{
    VALUE cReaper;

    cReaper = G_DEF_CLASS(VTE_TYPE_REAPER, "Reaper", mVte);

    rb_define_module_function(cReaper, "get", reaper_get, 0);

    G_DEF_SETTERS(cReaper);
}
