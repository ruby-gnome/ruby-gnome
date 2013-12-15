/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2013  Ruby-GNOME2 Project Team
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

#ifndef __RBPOPPLER_H__
#define __RBPOPPLER_H__

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <poppler.h>

#ifndef POPPLER_MAJOR_VERSION
#  include "rbpopplerversion.h"
#endif

#include "rbpopplerconversions.h"

#ifndef POPPLER_TYPE_INDEX_ITER
#  define POPPLER_TYPE_INDEX_ITER (poppler_index_iter_get_type ())
#endif
#ifndef POPPLER_TYPE_FONTS_ITER
#  define POPPLER_TYPE_FONTS_ITER (poppler_fonts_iter_get_type ())
#endif

#ifndef POPPLER_TYPE_DEST
extern GType poppler_dest_get_type (void) G_GNUC_CONST;
#  define POPPLER_TYPE_DEST (poppler_dest_get_type ())
#  define RB_POPPLER_TYPE_DEST_NOT_DEFINED
#endif

#include <rb_cairo.h>

#ifdef POPPLER_TYPE_COLOR
extern PopplerColor *rb_poppler_ruby_object_to_color(VALUE color);
extern VALUE rb_poppler_ruby_object_from_color_with_free(PopplerColor *color);
#endif

extern VALUE rb_poppler_ruby_object_from_form_field(PopplerFormField *field);
extern VALUE rb_poppler_ruby_object_from_action(PopplerAction *action);
extern PopplerAction *rb_poppler_action_from_ruby_object(VALUE action);

#endif
