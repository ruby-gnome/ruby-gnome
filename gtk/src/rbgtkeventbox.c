/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkeventbox.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
eventbox_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_event_box_new());
    return Qnil;
}

void Init_gtk_eventbox()
{
    static RGObjClassInfo cinfo;

    gEventBox = rb_define_class_under(mGtk, "EventBox", gBin);
    cinfo.klass = gEventBox;
    cinfo.gtype = GTK_TYPE_EVENT_BOX;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gEventBox, "initialize", eventbox_initialize, 0);
}
