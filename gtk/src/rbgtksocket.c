/************************************************

  rbgtksocket.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 2002 Neil Conway
************************************************/

#include "global.h"

static VALUE
socket_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_socket_new());
    return Qnil;
}

static VALUE
socket_steal(self, wid)
    VALUE self, wid;
{
    gtk_socket_steal(GTK_SOCKET(RVAL2GOBJ(self)),
                     (guint32)NUM2INT(wid));
    return Qnil;
}

static VALUE
socket_plug_window(self)
    VALUE self;
{
    return make_gdkwindow(GTK_SOCKET(RVAL2GOBJ(self))->plug_window);
}

void Init_gtk_socket()
{
    static rbgtk_class_info cinfo;

    gSocket = rb_define_class_under(mGtk, "Socket", gContainer);
    cinfo.klass = gSocket;
    cinfo.gtype = GTK_TYPE_SOCKET;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gSocket, "initialize",  socket_initialize, 0);
    rb_define_method(gSocket, "steal", socket_steal, 1);
    rb_define_method(gSocket, "plug_window", socket_plug_window, 0);
}
