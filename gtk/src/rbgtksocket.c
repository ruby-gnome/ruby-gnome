/************************************************

  rbgtksocket.c -

  $Author: sakai $
  $Date: 2002/08/20 03:38:53 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

#ifdef HAVE_GTK_SOCKET_GET_TYPE

static VALUE
socket_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_socket_new());
    return Qnil;
}

#ifndef GTK_DISABLE_DEPRECATED
static VALUE
socket_steal(self, wid)
    VALUE self, wid;
{
    gtk_socket_steal(GTK_SOCKET(RVAL2GOBJ(self)),
#ifdef GDK_NATIVE_WINDOW_POINTER
                     GUINT_TO_POINTER(NUM2ULONG(wid)));
#else
                     (guint32)NUM2UINT(wid));
#endif

    return Qnil;
}
#endif /* GTK_DISABLE_DEPRECATED */

static VALUE
socket_add_id(self, wid)
    VALUE self, wid;
{
    gtk_socket_add_id(GTK_SOCKET(RVAL2GOBJ(self)),
#ifdef GDK_NATIVE_WINDOW_POINTER
                      GUINT_TO_POINTER(NUM2ULONG(wid))
#else
                      (guint32)NUM2UINT(wid)
#endif                      
                      );
    return self;
}

static VALUE
socket_plug_window(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_SOCKET(RVAL2GOBJ(self))->plug_window);
}

static VALUE
socket_get_socket_id(self)
    VALUE self;
{
    GdkNativeWindow id =  gtk_socket_get_id(GTK_SOCKET(RVAL2GOBJ(self)));
#ifdef GDK_NATIVE_WINDOW_POINTER
    return UINT2NUM(GPOINTER_TO_UINT(id));
#else
    return UINT2NUM(id);
#endif
}

#endif /* HAVE_GTK_SOCKET_GET_TYPE */

void 
Init_gtk_socket()
{
#ifdef HAVE_GTK_SOCKET_GET_TYPE
    VALUE gSocket = G_DEF_CLASS(GTK_TYPE_SOCKET, "Socket", mGtk);

    rb_define_method(gSocket, "initialize",  socket_initialize, 0);
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(gSocket, "steal", socket_steal, 1);
#endif
    rb_define_method(gSocket, "plug_window", socket_plug_window, 0);
    rb_define_method(gSocket, "add_id", socket_add_id, 1);
    rb_define_method(gSocket, "id", socket_get_socket_id, 0);
#ifdef GDK_WINDOWING_X11
    rb_define_alias(gSocket, "xwindow", "id");
#endif
#endif
}
