#ifndef RBGPUI_H
#define RBGPUI_H

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

extern void Init_gnome_print_dialog(VALUE mGnome);
extern void Init_gnome_print_config_dialog(VALUE mGnome);
extern void Init_gnome_print_job_preview(VALUE mGnome);
extern void Init_gnome_print_preview(VALUE mGnome);
extern void Init_gnome_print_unit_selector(VALUE mGnome);

extern void Init_gnome_paper_selector(VALUE mGnome);

#endif
