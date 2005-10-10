/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-root.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-root.h>

static VALUE
gp_gpa_get_printers(VALUE self)
{
  VALUE array = rb_ary_new();
  GPANode *printers = GPA_NODE(gpa_get_printers());
  GPANode *printer;

  printer = gpa_node_get_child (printers, NULL);
  while (printer) {
    rb_ary_push(array, GOBJ2RVAL(printer));
    printer = gpa_node_get_child(printers, printer);
  }
  gpa_node_unref(printers);
  
  return array;
}


void
Init_gnome_print_gpa_root(VALUE mGnome)
{
  VALUE mGPA = rb_define_module_under(mGnome, "GPARoot");
  
  gpa_init();

  rb_define_module_function(mGPA, "printers", gp_gpa_get_printers, 0);
}
