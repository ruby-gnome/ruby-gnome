/************************************************

  rbgdkpixmap.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

static VALUE
plug_initialize(self, socket_id)
    VALUE self, socket_id;
{
    GtkWidget* plug = gtk_plug_new((guint32)NUM2INT(socket_id));
    RBGTK_INITIALIZE(self, plug);
    return Qnil;
}

static VALUE
plug_construct(self, socket_id)
    VALUE self, socket_id;
{
    gtk_plug_construct(GTK_PLUG(RVAL2GOBJ(self)), (guint32)NUM2INT(socket_id));
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
