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
#include <libgnomeprint/private/gpa-root.h>

static VALUE cPrinter;
static ID s_name, s_description;

static VALUE
gp_gpa_get_printers(VALUE self)
{
  VALUE array = rb_ary_new();
  GPANode *printers = GPA_NODE(gpa_get_printers());
  GPANode *printer;

  printer = gpa_node_get_child (printers, NULL);
  while (printer) {
    const guchar *id = gpa_node_id(printer);
    guchar *name = gpa_node_get_value(printer);
    rb_ary_push(array,
                rb_funcall(cPrinter,
                           rb_intern("new"),
                           2,
                           CSTR2RVAL(id),
                           CSTR2RVAL2(name)));
    printer = gpa_node_get_child(printers, printer);
  }
  gpa_node_unref(printers);
  
  return array;
}


static VALUE
printer_initialize(VALUE self, VALUE name, VALUE description)
{
  rb_ivar_set(self, s_name, name);
  rb_ivar_set(self, s_description, description);
  return self;
}


void
Init_gnome_print_gpa_root(VALUE mGnome, VALUE mGP)
{
  VALUE mGPA = rb_define_module_under(mGnome, "PrintGPA");
  
  gpa_init();

  rb_define_module_function(mGPA, "printers", gp_gpa_get_printers, 0);
  

  cPrinter = rb_define_class_under(mGnome, "PrintPrinter", rb_cObject);

  rb_define_method(cPrinter, "initialize", printer_initialize, 2);

  s_name = rb_intern("@name");
  s_description = rb_intern("@description");
  
  rb_define_attr(cPrinter, "name", 1, 0);
  rb_define_attr(cPrinter, "description", 1, 0);
}
