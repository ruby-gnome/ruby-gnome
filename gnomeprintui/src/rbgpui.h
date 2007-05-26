/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui.h -

  $Author: ktou $
  $Date: 2007/05/26 02:23:08 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#ifndef RBGPUI_H
#define RBGPUI_H

#include "rblibgnomeprintuiversion.h"

#ifdef HAVE_LIBGNOMEPRINTUI_LIBGNOMEPRINTUI_ENUM_TYPES_H
#  include <libgnomeprintui/libgnomeprintui-enum-types.h>
#else
#  include "libgnomeprintui-enum-types.h"
#endif

#include <ruby.h>

#include <rbgtk.h>

#define RBGPUI_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBGPUI_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBGPUI_MICRO_VERSION RBGLIB_MICRO_VERSION

#endif
