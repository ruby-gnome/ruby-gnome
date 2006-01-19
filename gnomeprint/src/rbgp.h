/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp.h -

  $Author: ktou $
  $Date: 2006/01/19 03:07:06 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#ifndef RBGP_H
#define RBGP_H

#include "rblibgnomeprintversion.h"

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <libgnomeprint/gnome-print-unit.h>
#include <libgnomeprint/libgnomeprint-enum-types.h>

#define RBGP_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBGP_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBGP_MICRO_VERSION RBGLIB_MICRO_VERSION

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
