/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 2002  Neil Conway
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

#ifdef HAVE_GTK_PLUG_GET_TYPE

static VALUE
plug_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE socket_id;
    GdkNativeWindow id;

    if (rb_scan_args(argc, argv, "01", &socket_id) == 0)
        id = 0;
    else {
#ifdef GDK_NATIVE_WINDOW_POINTER
        id = GUINT_TO_POINTER(NUM2ULONG(socket_id));
#else
        id = NUM2UINT(socket_id);
#endif
    }
    
    RBGTK_INITIALIZE(self, gtk_plug_new(id));
    return Qnil;
}

/*
GtkWidget*  gtk_plug_new_for_display        (GdkDisplay *display,
GdkNativeWindow socket_id);
void        gtk_plug_construct_for_display  (GtkPlug *plug,
GdkDisplay *display,
                                             GdkNativeWindow socket_id);
*/

static VALUE
plug_construct(VALUE self, VALUE socket_id)
{
#ifdef GDK_NATIVE_WINDOW_POINTER
    gtk_plug_construct(GTK_PLUG(RVAL2GOBJ(self)), GUINT_TO_POINTER(NUM2ULONG(socket_id)));
#else
    gtk_plug_construct(GTK_PLUG(RVAL2GOBJ(self)), (guint32)NUM2UINT(socket_id));
#endif
    return self;
}

static VALUE
plug_get_id(VALUE self)
{
    GdkNativeWindow id = gtk_plug_get_id(GTK_PLUG(RVAL2GOBJ(self)));
#ifdef GDK_NATIVE_WINDOW_POINTER
    return UINT2NUM(GPOINTER_TO_UINT(id));
#else
    return UINT2NUM(id);
#endif
}

#endif /* HAVE_GTK_PLUG_GET_TYPE */

void 
Init_gtk_plug(void)
{
#ifdef HAVE_GTK_PLUG_GET_TYPE
    VALUE gPlug = G_DEF_CLASS(GTK_TYPE_PLUG, "Plug", mGtk);
    
    rb_define_method(gPlug, "initialize", plug_initialize, -1);
    rb_define_method(gPlug, "construct", plug_construct, 1);
    rb_define_method(gPlug, "id", plug_get_id, 0);
#endif
}
