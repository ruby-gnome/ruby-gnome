/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-state.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-model.h>
#include <libgnomeprint/private/gpa-state.h>

static VALUE
gp_gpa_state_initialize(VALUE self, VALUE id)
{
  G_INITIALIZE(self, gpa_state_new(RVAL2CSTR(id)));
  return Qnil;
}

void
Init_gnome_print_gpa_state(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GPA_TYPE_STATE, "GPAState", mGnome);
  
  rb_define_method(c, "initialize", gp_gpa_state_initialize, 1);
}
