/************************************************

  rbgtksocket.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:05 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

static VALUE
socket_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_socket_new());
    return Qnil;
}

static VALUE
socket_steal(self, wid)
    VALUE self, wid;
{
    gtk_socket_steal(GTK_SOCKET(get_widget(self)),
                     (guint32)NUM2INT(wid));
    return Qnil;
}

static VALUE
socket_plug_window(self)
    VALUE self;
{
    return make_gdkwindow(GTK_SOCKET(get_widget(self))->plug_window);
}

void Init_gtk_socket()
{
    gSocket = rb_define_class_under(mGtk, "Socket", gContainer);

    rb_define_method(gSocket, "initialize",  socket_initialize, 0);
    rb_define_method(gSocket, "steal", socket_steal, 1);
    rb_define_method(gSocket, "plug_window", socket_plug_window, 0);
}
