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

#include "rbgprivate.h"
#include "rbglib2conversions.h"

#include <girepository/girepository.h>
#include <girepository/girffi.h>

#include "gi-enum-types.h"
#include "rbgi-conversions.h"

G_GNUC_INTERNAL void rbgi_arg_info_init(VALUE mGLib);
G_GNUC_INTERNAL void rbgi_callable_info_init(VALUE mGLib);
G_GNUC_INTERNAL void rbgi_base_info_init(VALUE mGLib);
G_GNUC_INTERNAL void rbgi_interface_info_init(VALUE mGLib);
G_GNUC_INTERNAL void rbgi_repository_init(VALUE mGLib);
G_GNUC_INTERNAL void rbgi_type_info_init(VALUE mGLib);

G_GNUC_INTERNAL gboolean
rbgi_arg_info_is_input_buffer(GIArgInfo *info);
G_GNUC_INTERNAL gboolean
rbgi_arg_info_is_output_buffer(GIArgInfo *info);
