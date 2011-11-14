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

#include "rbgtksourcemain.h"

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

void
Init_gtksourceview3 (void)
{
    Init_gtk_sourceview (mGtk);
    Init_gtk_sourcebuffer (mGtk);
    Init_gtk_sourceiter (mGtk);
    Init_gtk_sourcelanguage (mGtk);
    Init_gtk_sourcelanguagemanager (mGtk);
    Init_gtk_sourcemark (mGtk);
    Init_gtk_sourceprintcompositor (mGtk);
    Init_gtk_sourcestyle (mGtk);
    Init_gtk_sourcestylescheme (mGtk);
    Init_gtk_sourcestyleschememanager (mGtk);
}
