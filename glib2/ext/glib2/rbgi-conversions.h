/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

#define RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags) \
    (RVAL2GFLAGS(rb_flags, GI_TYPE_REPOSITORY_LOAD_FLAGS))

GIBaseInfo *rbgi_base_info_from_ruby(VALUE rb_info);
VALUE rbgi_base_info_to_ruby(GIBaseInfo *info);
VALUE rbgi_base_info_to_ruby_take(GIBaseInfo *info);

#define RVAL2GI_BASE_INFO(rb_info)   rbgi_base_info_from_ruby(rb_info)
#define GI_BASE_INFO2RVAL(info)      rbgi_base_info_to_ruby(info)
#define GI_BASE_INFO2RVAL_TAKE(info) rbgi_base_info_to_ruby_take(info)
