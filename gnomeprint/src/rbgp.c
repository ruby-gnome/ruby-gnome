#include "rbgp.h"

void
Init_gnomeprint2(void) {
  VALUE mGnome = rb_define_module("Gnome");
  VALUE mGP = rb_define_module_under(mGnome, "Print");

  Init_gnome_print(mGnome, mGP);
  Init_gnome_print_job(mGnome, mGP);
  Init_gnome_print_config(mGnome, mGP);
  Init_gnome_print_paper(mGnome, mGP);
  Init_gnome_print_unit(mGnome, mGP);

  Init_gnome_print_gpa_root(mGnome, mGP);
}
