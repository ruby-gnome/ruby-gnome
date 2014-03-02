/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifndef __VTE3_RBGIO2CONVERSIONS_H__
#define __VTE3_RBGIO2CONVERSIONS_H__

#define RVAL2GCANCELLABLE(o)               (G_CANCELLABLE(RVAL2GOBJ(o)))
#define RVAL2GOUTPUTSTREAM(o)              (G_OUTPUT_STREAM(RVAL2GOBJ(o)))

#endif /* __RBGIO2CONVERSIONS_H__ */
