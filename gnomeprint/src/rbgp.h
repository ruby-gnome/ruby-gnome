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

#ifndef RBGP_H
#define RBGP_H

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include "rbgp-utils.h"

#define GP_JOB(o) (GNOME_PRINT_JOB(RVAL2GOBJ(o)))
#define GP_CONFIG(o) (GNOME_PRINT_CONFIG(RVAL2GOBJ(o)))
#define GP_CONTEXT(o) (GNOME_PRINT_CONTEXT(RVAL2GOBJ(o)))

#define GP_GPA_PRINTER(o) (GPA_PRINTER(RVAL2GOBJ(o)))
#define GP_GPA_SETTINGS(o) (GPA_SETTINGS(RVAL2GOBJ(o)))

extern GType gnome_print_paper_get_type(void);

#define GNOME_TYPE_PRINT_PRINT_PAPER (gnome_print_paper_get_type())

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

#endif
