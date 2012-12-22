/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#ifndef RB_GI_CONVERSIONS_H
#define RB_GI_CONVERSIONS_H

#define RVAL2GI_REPOSITORY(rb_object) (G_IREPOSITORY(RVAL2GOBJ(rb_object)))
#define RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags) \
    (RVAL2GFLAGS(rb_flags, G_TYPE_I_REPOSITORY_LOAD_FLAGS))

#define GI_BASE_INFO2RVAL(info)      (rb_gi_base_info_to_ruby(info))
#define RVAL2GI_BASE_INFO(rb_object) (rb_gi_base_info_from_ruby(rb_object))

#define GI_TYPE_FUNCTION_INFO   (gi_function_info_get_type())

VALUE       rb_gi_base_info_to_ruby  (GIBaseInfo *info);
GIBaseInfo *rb_gi_base_info_from_ruby(VALUE rb_info);

GType gi_function_info_get_type(void);

#endif
