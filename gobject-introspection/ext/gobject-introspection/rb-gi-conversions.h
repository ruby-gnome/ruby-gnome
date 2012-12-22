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

#endif
