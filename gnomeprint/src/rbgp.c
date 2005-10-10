/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp.c -

  $Author: ktou $
  $Date: 2005/10/10 02:07:41 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

extern void Init_gnome_print(VALUE mGnome);
extern void Init_gnome_print_job(VALUE mGnome);
extern void Init_gnome_print_config(VALUE mGnome);
extern void Init_gnome_print_paper(VALUE mGnome);
extern void Init_gnome_print_unit(VALUE mGnome);

extern void Init_gnome_print_gpa_root(VALUE mGnome);
extern void Init_gnome_print_gpa_node(VALUE mGnome);
extern void Init_gnome_print_gpa_list(VALUE mGnome);
extern void Init_gnome_print_gpa_printer(VALUE mGnome);
extern void Init_gnome_print_gpa_settings(VALUE mGnome);
extern void Init_gnome_print_gpa_state(VALUE mGnome);
extern void Init_gnome_print_gpa_model(VALUE mGnome);

extern void Init_rbgp_utils(VALUE mGnome);

void
Init_gnomeprint2(void) {
    VALUE mGnome, mGnomePrint;

    mGnome = rb_define_module("Gnome");
    mGnomePrint = rb_define_module_under(mGnome, "Print");

    rb_define_const(mGnomePrint, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(LIBGNOMEPRINT_MAJOR_VERSION),
                                INT2FIX(LIBGNOMEPRINT_MINOR_VERSION),
                                INT2FIX(LIBGNOMEPRINT_MICRO_VERSION)));
  
    Init_gnome_print(mGnome);
    Init_gnome_print_job(mGnome);
    Init_gnome_print_config(mGnome);
    Init_gnome_print_paper(mGnome);
    Init_gnome_print_unit(mGnome);

    Init_gnome_print_gpa_root(mGnome);
    Init_gnome_print_gpa_node(mGnome);
    Init_gnome_print_gpa_list(mGnome);
    Init_gnome_print_gpa_printer(mGnome);
    Init_gnome_print_gpa_settings(mGnome);
    Init_gnome_print_gpa_state(mGnome);
    Init_gnome_print_gpa_model(mGnome);

    Init_rbgp_utils(mGnome);
}
