/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rb-gtk3.h"
#include "rb-gtk3-conversions.h"

#define RG_TARGET_NAMESPACE mContainer

#define RVAL2GTKCONTAINER(o)               (GTK_CONTAINER(RVAL2GOBJ(o)))
#define _SELF(self) (RVAL2GTKCONTAINER(self))

ID id_call;

static void
exec_callback(GtkWidget *widget, gpointer proc)
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

static VALUE
rg_foreach(int argc, VALUE *argv, VALUE self)
{
    VALUE callback;

    rb_scan_args(argc, argv, "01", &callback);
    if (NIL_P(callback)) {
        callback = rb_block_proc();
    }
    gtk_container_foreach(_SELF(self), exec_callback, (gpointer)callback);
    return self;
}

static VALUE
rg_forall(int argc, VALUE *argv, VALUE self)
{
    VALUE callback;

    rb_scan_args(argc, argv, "01", &callback);
    if (NIL_P(callback)) {
        callback = rb_block_proc();
    }
    gtk_container_forall(_SELF(self), exec_callback, (gpointer)callback);
    return self;
}


void
rb_gtk3_init_container(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE;
    id_call = rb_intern("call");
    /* Do not use RG_DEF_CLASS here, because the Gtk::Container class
     * is already created by the gi loader. */
    RG_TARGET_NAMESPACE = rbgobj_gtype_to_ruby_class(GTK_TYPE_CONTAINER);

    RG_DEF_METHOD(foreach, -1);
    RG_DEF_METHOD(forall, -1);
}
