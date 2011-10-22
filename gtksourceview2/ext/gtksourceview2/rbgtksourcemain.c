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

extern void Init_gtk_sourceview (void);
extern void Init_gtk_sourcebuffer (void);
extern void Init_gtk_sourceiter (void);
extern void Init_gtk_sourcelanguage (void);
extern void Init_gtk_sourcelanguagemanager (void);
extern void Init_gtk_sourcemark (void);
extern void Init_gtk_sourceprintcompositor (void);
extern void Init_gtk_sourcestyle (void);
extern void Init_gtk_sourcestylescheme (void);
extern void Init_gtk_sourcestyleschememanager (void);

void
Init_gtksourceview2 (void)
{
    Init_gtk_sourceview ();
    Init_gtk_sourcebuffer ();
    Init_gtk_sourceiter ();
    Init_gtk_sourcelanguage ();
    Init_gtk_sourcelanguagemanager ();
    Init_gtk_sourcemark ();
    Init_gtk_sourceprintcompositor ();
    Init_gtk_sourcestyle ();
    Init_gtk_sourcestylescheme ();
    Init_gtk_sourcestyleschememanager ();
}
