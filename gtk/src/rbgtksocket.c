/************************************************

  rbgtksocket.c -

  $Author: sakai $
  $Date: 2002/08/10 00:05:39 $

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
#endif

static VALUE
socket_plug_window(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_SOCKET(RVAL2GOBJ(self))->plug_window);
}

#if defined GDK_WINDOWING_X11
static VALUE
socket_get_socket_id(self)
    VALUE self;
{
    return INT2NUM(GDK_WINDOW_XWINDOW(GTK_WIDGET(RVAL2GOBJ(self))->window));
}
#endif

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
#if deffned GDK_WINDOWING_X11
    rb_define_method(gSocket, "xwindow", socket_get_socket_id, 0);
#endif
#endif
}
