/************************************************

  rbgdkplug.c -

  $Author: sakai $
  $Date: 2002/07/27 14:14:12 $

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
    static RGObjClassInfo cinfo;

    gPlug = rb_define_class_under(mGtk, "Plug", gWindow);
    cinfo.klass = gPlug;
    cinfo.gtype = GTK_TYPE_PLUG;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);
    
    rb_define_method(gPlug, "initialize", plug_initialize, 1);
    rb_define_method(gPlug, "construct", plug_construct, 1);
}
