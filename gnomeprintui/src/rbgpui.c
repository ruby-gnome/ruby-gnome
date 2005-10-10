/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui.c -

  $Author: ktou $
  $Date: 2005/10/10 01:42:03 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

extern void Init_gnome_print_dialog(VALUE mGnome);
extern void Init_gnome_print_config_dialog(VALUE mGnome);
extern void Init_gnome_print_job_preview(VALUE mGnome);
extern void Init_gnome_print_preview(VALUE mGnome);
extern void Init_gnome_print_unit_selector(VALUE mGnome);
extern void Init_gnome_print_content_selector(VALUE mGnome);

extern void Init_gnome_font_dialog(VALUE mGnome);

extern void Init_gnome_paper_selector(VALUE mGnome);

void
Init_gnomeprintui2(void) {
  VALUE mGnome,  mGnomePrintUI;

  mGnome = rb_define_module("Gnome");
  mGnomePrintUI = rb_define_module_under(mGnome, "PrintUI");

  rb_define_const(mGnomePrintUI, "BUILD_VERSION",
                  rb_ary_new3(3,
                              INT2FIX(LIBGNOMEPRINTUI_MAJOR_VERSION),
                              INT2FIX(LIBGNOMEPRINTUI_MINOR_VERSION),
                              INT2FIX(LIBGNOMEPRINTUI_MICRO_VERSION)));
  
  Init_gnome_print_dialog(mGnome);
  Init_gnome_print_config_dialog(mGnome);
  Init_gnome_print_job_preview(mGnome);
  Init_gnome_print_preview(mGnome);
  Init_gnome_print_unit_selector(mGnome);
  Init_gnome_print_content_selector(mGnome);

  Init_gnome_font_dialog(mGnome);
  
  Init_gnome_paper_selector(mGnome);
}
