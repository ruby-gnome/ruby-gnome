/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs, based on gtktextview.c by Masao Mutoh
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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE mGtkSource

void
Init_gtksourceview3 (void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module("GtkSource");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GTKSOURCEVIEW3_MAJOR_VERSION),
                                INT2FIX(GTKSOURCEVIEW3_MINOR_VERSION),
                                INT2FIX(GTKSOURCEVIEW3_MICRO_VERSION)));

    Init_gtksource_view (RG_TARGET_NAMESPACE);
    Init_gtksource_buffer (RG_TARGET_NAMESPACE);
    Init_gtksource_language (RG_TARGET_NAMESPACE);
    Init_gtksource_languagemanager (RG_TARGET_NAMESPACE);
    Init_gtksource_mark (RG_TARGET_NAMESPACE);
    Init_gtksource_printcompositor (RG_TARGET_NAMESPACE);
    Init_gtksource_style (RG_TARGET_NAMESPACE);
    Init_gtksource_stylescheme (RG_TARGET_NAMESPACE);
    Init_gtksource_styleschememanager (RG_TARGET_NAMESPACE);
    Init_gtksource_gutter(RG_TARGET_NAMESPACE);
    Init_gtksource_gutterrenderer(RG_TARGET_NAMESPACE);
    Init_gtksource_gutterrendererpixbuf(RG_TARGET_NAMESPACE);
    Init_gtksource_gutterrenderertext(RG_TARGET_NAMESPACE);
    Init_gtksource_markattributes(RG_TARGET_NAMESPACE);
    Init_gtksource_undomanager(RG_TARGET_NAMESPACE);
}
