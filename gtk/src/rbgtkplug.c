/************************************************

  rbgdkplug.c -

  $Author: sakai $
  $Date: 2002/08/05 16:24:02 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

#ifdef HAVE_GTK_PLUG_GET_TYPE

static VALUE
plug_initialize(self, socket_id)
    VALUE self, socket_id;
{
#ifdef GDK_NATIVE_WINDOW_POINTER
    GtkWidget* plug = gtk_plug_new(GUINT_TO_POINTER(NUM2ULONG(socket_id)));
#else
    GtkWidget* plug = gtk_plug_new((guint32)NUM2UINT(socket_id));
#endif
    RBGTK_INITIALIZE(self, plug);
    return Qnil;
}

static VALUE
plug_construct(self, socket_id)
    VALUE self, socket_id;
{
#ifdef GDK_NATIVE_WINDOW_POINTER
    gtk_plug_construct(GTK_PLUG(RVAL2GOBJ(self)), GUINT_TO_POINTER(NUM2ULONG(socket_id)));
#else
    gtk_plug_construct(GTK_PLUG(RVAL2GOBJ(self)), (guint32)NUM2UINT(socket_id));
#endif
    return Qnil;
}

#endif /* HAVE_GTK_PLUG_GET_TYPE */

void Init_gtk_plug()
{
#ifdef HAVE_GTK_PLUG_GET_TYPE
    VALUE gPlug = G_DEF_CLASS(GTK_TYPE_PLUG, "Plug", mGtk);
    
    rb_define_method(gPlug, "initialize", plug_initialize, 1);
    rb_define_method(gPlug, "construct", plug_construct, 1);
#endif
}
