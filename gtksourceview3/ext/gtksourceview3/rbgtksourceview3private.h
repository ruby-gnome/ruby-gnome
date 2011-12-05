/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs
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

#ifndef __RBGTKSOURCEVIEW3PRIVATE_H__
#define __RBGTKSOURCEVIEW3PRIVATE_H__

#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gtksourceview/gtksourcemark.h>
#include <gtksourceview/gtksourceprintcompositor.h>
#include <gtksourceview/gtksourcestyle.h>
#include <gtksourceview/gtksourcestylescheme.h>
#include <gtksourceview/gtksourcestyleschememanager.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourceview-typebuiltins.h>

#include <ruby.h>
#include <rb_cairo.h>
#include <rbgdk3.h>
#include <rbgtk3.h>
#include "rbgtksourceview3conversions.h"
#include "rbgtksourceview3version.h"

G_GNUC_INTERNAL void Init_gtk_sourceview (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcebuffer (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcelanguage (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcelanguagemanager (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcemark (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourceprintcompositor (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcestyle (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcestylescheme (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtk_sourcestyleschememanager (VALUE mGtkSource);

#endif /* __RBGTKSOURCEVIEW3PRIVATE_H__ */
