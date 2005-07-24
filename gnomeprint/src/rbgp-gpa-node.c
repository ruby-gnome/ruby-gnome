/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
