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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE mUndoManager
#define _SELF(self) (RVAL2GTKSOURCEUNDOMANAGER(self))

static VALUE
rg_end_not_undoable_action(VALUE self)
{
    gtk_source_undo_manager_end_not_undoable_action(_SELF(self));

    return self;
}

static VALUE
rg_begin_not_undoable_action(VALUE self)
{
    gtk_source_undo_manager_begin_not_undoable_action(_SELF(self));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, rg_end_not_undoable_action, self);

    return self;
}

static VALUE
rg_can_redo_p(VALUE self)
{
    return CBOOL2RVAL(gtk_source_undo_manager_can_redo(_SELF(self)));
}

static VALUE
rg_can_undo_p(VALUE self)
{
    return CBOOL2RVAL(gtk_source_undo_manager_can_undo(_SELF(self)));
}

static VALUE
rg_redo(VALUE self)
{
    gtk_source_undo_manager_redo(_SELF(self));

    return self;
}

static VALUE
rg_undo(VALUE self)
{
    gtk_source_undo_manager_undo(_SELF(self));

    return self;
}

void
Init_gtksource_undomanager(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_SOURCE_TYPE_UNDO_MANAGER, "UndoManager", mGtkSource);

    RG_DEF_METHOD(begin_not_undoable_action, 0);
    RG_DEF_METHOD_P(can_redo, 0);
    RG_DEF_METHOD_P(can_undo, 0);
    RG_DEF_METHOD(end_not_undoable_action, 0);
    RG_DEF_METHOD(redo, 0);
    RG_DEF_METHOD(undo, 0);
}
