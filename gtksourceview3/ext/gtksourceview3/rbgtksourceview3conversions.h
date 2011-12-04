/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#ifndef __RBGTKSOURCEVIEW3CONVERSIONS_H__
#define __RBGTKSOURCEVIEW3CONVERSIONS_H__

#define RVAL2GTKSOURCEBUFFER(o)                 (GTK_SOURCE_BUFFER(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCELANGUAGE(o)               (GTK_SOURCE_LANGUAGE(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCELANGUAGEMANAGER(o)        (GTK_SOURCE_LANGUAGE_MANAGER(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCEMARK(o)                   (GTK_SOURCE_MARK(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCEPRINTCOMPOSITOR(o)        (GTK_SOURCE_PRINT_COMPOSITOR(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCESTYLE(o)                  (GTK_SOURCE_STYLE(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCESTYLESCHEME(o)            (GTK_SOURCE_STYLE_SCHEME(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCESTYLESCHEMEMANAGER(o)     (GTK_SOURCE_STYLE_SCHEME_MANAGER(RVAL2GOBJ(o)))
#define RVAL2GTKSOURCEVIEW(o)                   (GTK_SOURCE_VIEW(RVAL2GOBJ(o)))

#endif /* __RBGTKSOURCEVIEW3CONVERSIONS_H__ */
