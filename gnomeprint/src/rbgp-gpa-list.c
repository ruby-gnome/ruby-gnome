/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-list.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-list.h>

static VALUE
gp_gpa_list_get_default(VALUE self)
{
  return GOBJ2RVAL(gpa_list_get_default(RVAL2GOBJ(self)));
}

static VALUE
gp_gpa_list_set_default(VALUE self, VALUE def)
{
  return CBOOL2RVAL(gpa_list_set_default(RVAL2GOBJ(self), RVAL2GOBJ(def)));
}


void
Init_gnome_print_gpa_list(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GPA_TYPE_LIST, "GPAList", mGnome);

  rb_define_method(c, "default", gp_gpa_list_get_default, 0);
  rb_define_method(c, "set_default", gp_gpa_list_set_default, 1);

  G_DEF_SETTERS(c);
}
