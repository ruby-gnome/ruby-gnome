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

#define RVAL2GI_ARG_INFO(rb_info)       GI_ARG_INFO(RVAL2GOBJ(rb_info))
#define RVAL2GI_BASE_INFO(rb_info)      GI_BASE_INFO(RVAL2GOBJ(rb_info))
#define RVAL2GI_CALLABLE_INFO(rb_info)  GI_CALLABLE_INFO(RVAL2GOBJ(rb_info))
#define RVAL2GI_INTERFACE_INFO(rb_info) GI_INTERFACE_INFO(RVAL2GOBJ(rb_info))
#define RVAL2GI_TYPE_INFO(rb_info)      GI_TYPE_INFO(RVAL2GOBJ(rb_info))

#define GI_ARRAY_TYPE2RVAL(type)     (GENUM2RVAL(type, GI_TYPE_ARRAY_TYPE))
#define GI_DIRECTION2RVAL(direction) (GENUM2RVAL(direction, GI_TYPE_DIRECTION))
#define GI_SCOPE_TYPE2RVAL(scope)    (GENUM2RVAL(scope, GI_TYPE_SCOPE_TYPE))
#define GI_TRANSFER2RVAL(transfer)   (GENUM2RVAL(transfer, GI_TYPE_TRANSFER))
#define GI_TYPE_TAG2RVAL(tag)        (GENUM2RVAL(tag, GI_TYPE_TYPE_TAG))
