/************************************************

  rbgdkplug.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

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

void Init_gtk_plug()
{
    VALUE gPlug = G_DEF_CLASS(GTK_TYPE_PLUG, "Plug", mGtk);
    
    rb_define_method(gPlug, "initialize", plug_initialize, 1);
    rb_define_method(gPlug, "construct", plug_construct, 1);
}
