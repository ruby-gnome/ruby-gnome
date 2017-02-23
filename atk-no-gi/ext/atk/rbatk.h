/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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
 
#include "ruby.h"
#include <atk/atk.h>
#include <atk/atk-enum-types.h>
#include <atk/atknoopobject.h>
#include <atk/atknoopobjectfactory.h>
#include "rbgobject.h"
#include "rbatkversion.h"
#include "rbatkconversions.h"

#if defined(G_PLATFORM_WIN32) && !defined(RUBY_ATK_STATIC_COMPILATION)
#  ifdef RUBY_ATK_COMPILATION
#    define RUBY_ATK_VAR __declspec(dllexport)
#  else
#    define RUBY_ATK_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_ATK_VAR extern
#endif

extern GType atk_text_rectangle_get_type(void);
extern GType atk_text_range_get_type(void);

RUBY_ATK_VAR VALUE mAtk;

#define ATK_TYPE_TEXT_RECTANGLE (atk_text_rectangle_get_type())
#define ATK_TYPE_TEXT_RANGE (atk_text_range_get_type())
