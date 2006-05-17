/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbvte-access.c -

  $Author: ktou $
  $Date: 2006/05/17 12:40:47 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbvte.h"

static VALUE
ta_initialize(VALUE self, VALUE terminal)
{
    G_INITIALIZE(self, vte_terminal_accessible_new(RVAL2TERM(terminal)));
    return Qnil;
}

void
Init_vte_access(VALUE mVte)
{
    VALUE cTerminalAccessible;

    cTerminalAccessible = G_DEF_CLASS(VTE_TYPE_TERMINAL_ACCESSIBLE,
                                      "TerminalAccessible", mVte);

    rb_define_method(cTerminalAccessible, "initialize", ta_initialize, 1);

    G_DEF_SETTERS(cTerminalAccessible);
}
