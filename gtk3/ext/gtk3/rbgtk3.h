/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#ifndef __RBGTK3_H__
#define __RBGTK3_H__

#include <gtk/gtk.h>

#include <rbgobject.h>
#include "rbgtk3conversions.h"

#define RBGTK_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBGTK_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBGTK_MICRO_VERSION RBGLIB_MICRO_VERSION

#define RBGTK_INITIALIZE(obj,gtkobj)\
 (rbgtk_initialize_gtkobject(obj, G_OBJECT(gtkobj)))

#define GTK_TYPE_ACCEL_GROUP_ENTRY (gtk_accel_group_entry_get_type())
#define GTK_TYPE_ACCEL_KEY (gtk_accel_key_get_type())
#define GTK_TYPE_ALLOCATION (gtk_allocation_get_type())
#define GTK_TYPE_BINDING_SET (gtk_bindingset_get_type())
#define GTK_TYPE_RECENT_DATA (gtk_recent_data_get_type())
#define GTK_TYPE_RECENT_FILTER_INFO (gtk_recent_filter_info_get_type())
#define GTK_TYPE_TEXT_APPEARANCE (gtk_text_appearance_get_type())

extern void rbgtk_initialize_gtkobject(VALUE obj, GObject *gtkobj);
extern GType gtk_accel_group_entry_get_type(void);
extern GType gtk_accel_key_get_type(void);
extern GType gtk_allocation_get_type(void);
extern GType gtk_bindingset_get_type(void);
extern GType gtk_recent_data_get_type(void);
extern GType gtk_recent_filter_info_get_type(void);
extern GType gtk_text_appearance_get_type(void);

#endif /* __RBGTK3_H__ */
