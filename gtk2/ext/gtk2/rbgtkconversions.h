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

#ifndef _RBGTKCONVERSIONS_H
#define _RBGTKCONVERSIONS_H

#include <rbgobject.h>
#include <gtk/gtk.h>

#include <rbgdkconversions.h>

#define GTKTREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2GTKTREEPATH(t) ((GtkTreePath *)RVAL2BOXED(t, GTK_TYPE_TREE_PATH))
#define GTKTREEITER2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2GTKTREEITER(i) ((GtkTreeIter *)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

#define RVAL2GTKSTATETYPE(type) (RVAL2GENUM(type, GTK_TYPE_STATE_TYPE))
#define GTKRCFLAGS2RVAL(flags) (GFLAGS2RVAL(flags, GTK_TYPE_RC_FLAGS))
#define RVAL2GTKRCFLAGS(flags) (RVAL2GFLAGS(flags, GTK_TYPE_RC_FLAGS))

#endif /* _RBGTKCONVERSIONS_H */
