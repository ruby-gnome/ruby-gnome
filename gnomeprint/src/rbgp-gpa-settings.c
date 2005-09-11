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
#include <libgnomeprint/private/gpa-settings.h>

#define _SELF(self) (GP_GPA_SETTINGS(self))

static VALUE
gp_gpa_settings_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE model, name, id;
  rb_scan_args(argc, argv, "21", &model, &name, &id);

  /*
  if (NIL_P(model)) {
    model = name;
    name = id;
    G_INITIALIZE(self, gpa_settings_new_empty(RVAL2CSTR(name), RVAL2CSTR(id)));
  } else */ if (NIL_P(id)) {
    VALUE tree = id;
    G_INITIALIZE(self, gpa_settings_new_from_model_and_tree(RVAL2GOBJ(model),
                                                            RVAL2GOBJ(tree)));
  } else {
    G_INITIALIZE(self, gpa_settings_new(RVAL2GOBJ(model),
                                        (const guchar *)RVAL2CSTR(name),
                                        (const guchar *)RVAL2CSTR(id)));
  }
  return Qnil;
}

void
Init_gnome_print_gpa_settings(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GPA_TYPE_SETTINGS, "GPASettings", mGnome);
  
  rb_define_method(c, "initialize", gp_gpa_settings_initialize, -1);
}
