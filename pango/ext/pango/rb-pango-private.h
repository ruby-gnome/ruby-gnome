/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017-2022  Ruby-GNOME Project Team
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

#pragma once

#include "rb-pango.h"

#ifndef PANGO_CHECK_VERSION
#  define PANGO_CHECK_VERSION(major, minor, micro)                      \
    (PANGO_VERSION_MAJOR > (major) ||                                   \
     (PANGO_VERSION_MAJOR == (major) && PANGO_VERSION_MINOR > (minor)) || \
     (PANGO_VERSION_MAJOR == (major) && PANGO_VERSION_MINOR == (minor) && \
      PANGO_VERSION_MICRO >= (micro)))
#endif

G_GNUC_INTERNAL void rbpango_attribute_init(VALUE mPango);
G_GNUC_INTERNAL void rbpango_attr_iterator_init(VALUE mPango);
G_GNUC_INTERNAL void rbpango_attr_list_init(VALUE mPango);
G_GNUC_INTERNAL void rbpango_scale_init(VALUE mPango);
