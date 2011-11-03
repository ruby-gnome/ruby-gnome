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

#ifdef HAVE_GTK_SOCKET_GET_TYPE

#define RG_TARGET_NAMESPACE cSocket
#define _SELF(self) GTK_SOCKET(RVAL2GOBJ(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_socket_new());
    return Qnil;
}

static VALUE
rg_add_id(VALUE self, VALUE wid)
{
    gtk_socket_add_id(_SELF(self),
#ifdef GDK_NATIVE_WINDOW_POINTER
                      GUINT_TO_POINTER(NUM2ULONG(wid))
#else
                      (guint32)NUM2UINT(wid)
#endif                      
        );
    return self;
}

static VALUE
rg_plug_window(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->plug_window);
}

static VALUE
rg_id(VALUE self)
{
    GdkNativeWindow id =  gtk_socket_get_id(_SELF(self));
#ifdef GDK_NATIVE_WINDOW_POINTER
    return UINT2NUM(GPOINTER_TO_UINT(id));
#else
    return UINT2NUM(id);
#endif
}

#endif /* HAVE_GTK_SOCKET_GET_TYPE */

void 
Init_gtk_socket(VALUE mGtk)
{
#ifdef HAVE_GTK_SOCKET_GET_TYPE
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SOCKET, "Socket", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(plug_window, 0);
    RG_DEF_METHOD(add_id, 1);
    RG_DEF_METHOD(id, 0);
#endif
}
