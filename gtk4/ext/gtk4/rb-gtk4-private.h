/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2022  Ruby-GNOME Project Team
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

/*
 * AlmaLinux 9: GTK 4.4
 * Debian GNU/Linux bookworm: GTK 4.6
 * Ubuntu 22.04: GTK 4.6
 */
#define GDK_VERSION_MIN_REQUIRED GDK_VERSION_4_4

#include "rb-gtk4.h"

G_GNUC_INTERNAL void rb_gtk4_widget_init(void);
G_GNUC_INTERNAL void rb_gtk4_window_init(void);
