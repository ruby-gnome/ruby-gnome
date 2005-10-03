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

#include "rbgp-version.h"

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <libgnomeprint/gnome-print-unit.h>
#include <libgnomeprint/libgnomeprint-enum-types.h>

VALUE rbgp_check_return_code(gint code);

#define GP_JOB(o) (GNOME_PRINT_JOB(RVAL2GOBJ(o)))
#define GP_CONFIG(o) (GNOME_PRINT_CONFIG(RVAL2GOBJ(o)))
#define GP_CONTEXT(o) (GNOME_PRINT_CONTEXT(RVAL2GOBJ(o)))

#define GP_GPA_PRINTER(o) (GPA_PRINTER(RVAL2GOBJ(o)))
#define GP_GPA_SETTINGS(o) (GPA_SETTINGS(RVAL2GOBJ(o)))

#define RVAL2GPU(obj) ((GnomePrintUnit *)RVAL2BOXED(obj, GNOME_TYPE_PRINT_UNIT))
#define GPU2RVAL(obj) (BOXED2RVAL(obj, GNOME_TYPE_PRINT_UNIT))
#define CONST_GPU2RVAL(obj) (GPU2RVAL((GnomePrintUnit *)obj))

#define RVAL2GPUB(obj) (RVAL2GFLAGS(obj, GNOME_TYPE_PRINT_PRINT_UNIT_BASE))
#define GPUB2RVAL(obj) (GFLAGS2RVAL(obj, GNOME_TYPE_PRINT_PRINT_UNIT_BASE))

extern GType gnome_print_paper_get_type(void);

#define GNOME_TYPE_PRINT_PRINT_PAPER (gnome_print_paper_get_type())

#endif
