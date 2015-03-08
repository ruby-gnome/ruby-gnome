/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Kouhei Sutou <kou@cozmixng.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#ifndef RSVG2_H
#define RSVG2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <librsvg/rsvg.h>

#include <rb_cairo.h>

#define LIBRSVG_CHECK_VERSION(major, minor, micro)                            \
    (LIBRSVG_MAJOR_VERSION > (major) ||                                       \
     (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION > (minor)) || \
     (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION == (minor) && \
      LIBRSVG_MICRO_VERSION >= (micro)))

#if !LIBRSVG_CHECK_VERSION(2, 36, 2)
#  include <librsvg/librsvg-features.h>
#  include <librsvg/rsvg-cairo.h>
#  ifdef HAVE_LIBRSVG_LIBRSVG_ENUM_TYPES_H
#    include <librsvg/librsvg-enum-types.h>
#  else
#    include "librsvg-enum-types.h"
#  endif
#endif


G_GNUC_INTERNAL void Init_rsvg_handle(VALUE mRSVG);
G_GNUC_INTERNAL void Init_rsvg_dimensiondata(VALUE mRSVG);

#ifdef __cplusplus
}
#endif

#endif
