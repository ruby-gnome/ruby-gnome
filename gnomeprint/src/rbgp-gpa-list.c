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
