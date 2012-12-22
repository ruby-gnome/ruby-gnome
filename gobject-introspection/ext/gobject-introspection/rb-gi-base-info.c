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

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGIBaseInfo
#define SELF(self) RVAL2GI_BASE_IFNO(self)

VALUE
rb_gi_base_info_to_ruby(GIBaseInfo *info)
{
    GType g_type = GI_TYPE_BASE_INFO;
    GIInfoType info_type;

    info_type = g_base_info_get_type(info);
    switch (info_type) {
      case GI_INFO_TYPE_FUNCTION:
	g_type = GI_TYPE_FUNCTION_INFO;
	break;
      default:
	break;
    }

    return BOXED2RVAL(info, g_type);
}

void
rb_gi_base_info_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GI_TYPE_BASE_INFO, "BaseInfo", rb_mGI);

    rb_gi_function_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
