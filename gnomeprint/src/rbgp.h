#ifndef RBGP_H
#define RBGP_H

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#define GP_JOB(o) (GNOME_PRINT_JOB(RVAL2GOBJ(o)))
#define GP_CONFIG(o) (GNOME_PRINT_CONFIG(RVAL2GOBJ(o)))
#define GP_CONTEXT(o) (GNOME_PRINT_CONTEXT(RVAL2GOBJ(o)))

extern void Init_gnome_print(VALUE mGnome, VALUE mGP);
extern void Init_gnome_print_job(VALUE mGnome, VALUE mGP);
extern void Init_gnome_print_config(VALUE mGnome, VALUE mGP);
extern void Init_gnome_print_paper(VALUE mGnome, VALUE mGP);
extern void Init_gnome_print_unit(VALUE mGnome, VALUE mGP);

extern void Init_gnome_print_gpa_root(VALUE mGnome, VALUE mGP);

#endif
