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

void
Init_gnomeprint2(void) {
  VALUE mGnome = rb_define_module("Gnome");

  Init_gnome_print(mGnome);
  Init_gnome_print_job(mGnome);
  Init_gnome_print_config(mGnome);
  Init_gnome_print_paper(mGnome);
  Init_gnome_print_unit(mGnome);

  Init_gnome_print_gpa_root(mGnome);
  Init_gnome_print_gpa_node(mGnome);
  Init_gnome_print_gpa_printer(mGnome);
  Init_gnome_print_gpa_settings(mGnome);
  Init_gnome_print_gpa_model(mGnome);

  Init_rbgp_utils(mGnome);
}
