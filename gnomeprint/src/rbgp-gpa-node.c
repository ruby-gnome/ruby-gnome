/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-node.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-node.h>

static VALUE
gp_gpa_node_id(VALUE self)
{
  return CSTR2RVAL((const char*)gpa_node_id(RVAL2GOBJ(self)));
}

static VALUE
gp_gpa_node_get_value(VALUE self)
{
  return CSTR2RVAL((const char*)gpa_node_get_value(RVAL2GOBJ(self)));
}

static VALUE
gp_gpa_node_set_value(VALUE self, VALUE value)
{
  return CBOOL2RVAL(gpa_node_set_value(RVAL2GOBJ(self), (const guchar*)RVAL2CSTR(value)));
}


void
Init_gnome_print_gpa_node(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GPA_TYPE_NODE, "GPANode", mGnome);

  rb_define_method(c, "id", gp_gpa_node_id, 0);
  rb_define_method(c, "value", gp_gpa_node_get_value, 0);
  rb_define_method(c, "set_value", gp_gpa_node_set_value, 1);

  G_DEF_SETTERS(c);
}
