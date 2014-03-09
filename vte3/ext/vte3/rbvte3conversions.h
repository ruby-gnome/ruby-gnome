/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2014  Ruby-GNOME2 Project Team
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

#ifndef __RBVTE3CONVERSIONS_H__
#define __RBVTE3CONVERSIONS_H__

#define RVAL2VTEPTY(o)                     (VTE_PTY(RVAL2GOBJ(o)))
#define RVAL2VTETERMINAL(o)                (VTE_TERMINAL(RVAL2GOBJ(o)))

#define RVAL2VTETERMINALCURSORBLINKMODE(o) (RVAL2GENUM(o, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE))
#define VTETERMINALCURSORBLINKMODE2RVAL(o) (GENUM2RVAL(o, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE))
#define RVAL2VTETERMINALCURSORSHAPE(o)     (RVAL2GENUM(o, VTE_TYPE_TERMINAL_CURSOR_SHAPE))
#define VTETERMINALCURSORSHAPE2RVAL(o)     (GENUM2RVAL(o, VTE_TYPE_TERMINAL_CURSOR_SHAPE))
#define RVAL2VTETERMINALERASEBINDING(o)    (RVAL2GENUM(o, VTE_TYPE_TERMINAL_ERASE_BINDING))
#define VTETERMINALERASEBINDING2RVAL(o)    (GENUM2RVAL(o, VTE_TYPE_TERMINAL_ERASE_BINDING))
#define RVAL2VTETERMINALWRITEFLAGS(o)      (RVAL2GENUM(o, VTE_TYPE_TERMINAL_WRITE_FLAGS))
#define VTETERMINALWRITEFLAGS2RVAL(o)      (GENUM2RVAL(o, VTE_TYPE_TERMINAL_WRITE_FLAGS))

#define RVAL2VTEPTYFLAGS(o)                (RVAL2GFLAGS(o, VTE_TYPE_PTY_FLAGS))
#define VTEPTYFLAGS2RVAL(o)                (GFLAGS2RVAL(o, VTE_TYPE_PTY_FLAGS))

/* For GIO. They are temporary. They will be needless after Ruby/GDK3
 * is based on Ruby/GObjectIntrospection.
 */
#define RVAL2GCANCELLABLE(o)               (G_CANCELLABLE(RVAL2GOBJ(o)))
#define RVAL2GOUTPUTSTREAM(o)              (G_OUTPUT_STREAM(RVAL2GOBJ(o)))

#endif /* __RBVTE3CONVERSIONS_H__ */
