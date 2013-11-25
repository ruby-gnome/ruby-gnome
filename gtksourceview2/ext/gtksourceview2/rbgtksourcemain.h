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

#ifndef __RBGTKSOURCEVIEWMAIN_H_
#define __RBGTKSOURCEVIEWMAIN_H_

#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourceiter.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gtksourceview/gtksourcemark.h>
#include <gtksourceview/gtksourceprintcompositor.h>
#include <gtksourceview/gtksourcestyle.h>
#include <gtksourceview/gtksourcestylescheme.h>
#include <gtksourceview/gtksourcestyleschememanager.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourceview-typebuiltins.h>

#include "ruby.h"
#include "rbgtk.h"
#include "rbgtksourceview2version.h"

#undef RG_DEF_METHOD
#define RG_DEF_METHOD(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#undef RG_DEF_SMETHOD
#define RG_DEF_SMETHOD(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)

extern void Init_gtksourceview2 (void);

G_GNUC_INTERNAL void Init_gtk_sourceview (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcebuffer (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourceiter (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcelanguage (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcelanguagemanager (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcemark (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourceprintcompositor (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcestyle (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcestylescheme (VALUE mGtk);
G_GNUC_INTERNAL void Init_gtk_sourcestyleschememanager (VALUE mGtk);

#endif /* __RBGTKSOURCEVIEWMAIN_H_ */
