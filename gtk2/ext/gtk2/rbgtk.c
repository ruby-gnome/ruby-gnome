/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2001 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#define RG_TARGET_NAMESPACE mGtk

VALUE RG_TARGET_NAMESPACE;

ID id_relative_callbacks;
ID id_call;
static ID id__windows__;

static void
remove_from_windows(G_GNUC_UNUSED GtkWidget *window, VALUE obj)
{
    VALUE klass, windows;

    klass = rb_obj_class(obj);
    windows = rb_ivar_get(klass, id__windows__);
    if (!NIL_P(windows)) {
        rb_hash_delete(windows, obj);
    }
}

void
rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj)
{
    gtkobj = g_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    G_INITIALIZE(obj, gtkobj);

    if (GTK_IS_WINDOW(gtkobj) || GTK_IS_MENU_SHELL(gtkobj)) {
        VALUE klass;
        klass = rb_obj_class(obj);
        if (rb_ivar_defined(klass, id__windows__) == Qfalse) {
            rb_ivar_set(klass, id__windows__, rb_hash_new());
        }
        rb_hash_aset(rb_ivar_get(klass, id__windows__), obj, Qnil);
        g_signal_connect_after(gtkobj, "destroy",
                               G_CALLBACK(remove_from_windows),
                               (gpointer)obj);
    }
}

void
exec_callback(GtkWidget *widget, gpointer proc)
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

/*
 * Gtk module
 */

/* We can't use rbgtk_poll() on native Win32.
   Because GPollFD doesn't have file descriptor but HANDLE. */
#define USE_POLL_FUNC

static VALUE
gtk_m_events_pending(G_GNUC_UNUSED VALUE self)
{
   return CBOOL2RVAL(gtk_events_pending());
}

/*
 * Init
 */
void 
Init_gtk_gtk(void)
{
    id_call = rb_intern("call");
    id__windows__ = rb_intern("__windows__");

    RG_TARGET_NAMESPACE = rb_define_module("Gtk");
    rb_ivar_set(RG_TARGET_NAMESPACE, id_relative_callbacks, Qnil);

    rb_define_module_function(RG_TARGET_NAMESPACE, "events_pending?", gtk_m_events_pending, 0);

}
