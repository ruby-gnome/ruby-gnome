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
#include <libgnomeprint/private/gpa-printer.h>

#define _SELF(self) (GP_GPA_PRINTER(self))

static VALUE
gp_gpa_printer_initialize(VALUE self, VALUE id, VALUE name, VALUE model, VALUE settings)
{
  G_INITIALIZE(self, gpa_printer_new(RVAL2CSTR(id),
                                     RVAL2CSTR(name),
                                     RVAL2GOBJ(model),
                                     RVAL2GOBJ(settings)));
  return Qnil;
}

static VALUE
gp_gpa_printer_default(VALUE self)
{
  return GOBJ2RVAL(gpa_printer_get_default());
}

static VALUE
gp_gpa_printer_get_by_id(VALUE self, VALUE id)
{
  return GOBJ2RVAL(gpa_printer_get_by_id(RVAL2CSTR(id)));
}

/* static VALUE */
/* gp_gpa_printer_model(VALUE self) */
/* { */
/*   return GOBJ2RVAL((_SELF(self))->model); */
/* } */

static VALUE
gp_gpa_printer_set_polling(VALUE self, VALUE polling)
{
  gpa_printer_set_polling(_SELF(self), RVAL2CBOOL(polling));
  return Qnil;
}

static VALUE
gp_gpa_printer_default_settings(VALUE self)
{
  return GOBJ2RVAL(gpa_printer_get_default_settings(_SELF(self)));
}

static VALUE
gp_gpa_printer_settings_by_id(VALUE self, VALUE id)
{
  return GOBJ2RVAL(gpa_printer_get_settings_by_id(_SELF(self), RVAL2CSTR(id)));
}

static VALUE
gp_gpa_printer_settings_generic(int argc, VALUE *argv, VALUE self)
{
  VALUE id;
  rb_scan_args(argc, argv, "01", &id);

  if (NIL_P(id)) {
    return gp_gpa_printer_default_settings(self);
  } else {
    return gp_gpa_printer_settings_by_id(self, id);
  }
}


static VALUE
gp_gpa_printer_state(VALUE self)
{
  return GOBJ2RVAL(gpa_printer_get_state(_SELF(self)));
}

static VALUE
gp_gpa_printer_state_by_id(VALUE self, VALUE id)
{
  return GOBJ2RVAL(gpa_printer_get_state_by_id(_SELF(self), RVAL2CSTR(id)));
}

static VALUE
gp_gpa_printer_state_generic(int argc, VALUE *argv, VALUE self)
{
  VALUE id;
  rb_scan_args(argc, argv, "01", &id);

  if (NIL_P(id)) {
    return gp_gpa_printer_state(self);
  } else {
    return gp_gpa_printer_state_by_id(self, id);
  }
}


void
Init_gnome_print_gpa_printer(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GPA_TYPE_PRINTER, "GPAPrinter", mGnome);

  rb_define_module_function(c, "default", gp_gpa_printer_default, 0);
  rb_define_module_function(c, "get", gp_gpa_printer_get_by_id, 1);
  
  rb_define_method(c, "initialize", gp_gpa_printer_initialize, 4);

  rb_define_method(c, "set_polling", gp_gpa_printer_set_polling, 1);
/*   rb_define_method(c, "model", gp_gpa_printer_model, 0); */
  rb_define_method(c, "settings", gp_gpa_printer_settings_generic, -1);
  rb_define_method(c, "state", gp_gpa_printer_state_generic, -1);
  
  G_DEF_SETTERS(c);
}
