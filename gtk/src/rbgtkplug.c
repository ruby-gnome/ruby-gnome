/************************************************

  rbgdkpixmap.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:04 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

static VALUE
plug_initialize(self, socket_id)
    VALUE self, socket_id;
{
    GtkWidget* plug = gtk_plug_new((guint32)NUM2INT(socket_id));
    set_widget(self, plug);
    return Qnil;
}

static VALUE
plug_construct(self, socket_id)
    VALUE self, socket_id;
{
    gtk_plug_construct(GTK_PLUG(get_widget(self)), (guint32)NUM2INT(socket_id));
    return Qnil;
}

void Init_gtk_plug()
{
    gPlug = rb_define_class_under(mGtk, "Plug", gWindow);
    
    rb_define_method(gPlug, "initialize", plug_initialize, 1);
    rb_define_method(gPlug, "construct", plug_construct, 1);
}
