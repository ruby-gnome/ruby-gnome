/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkeventbox.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
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

void 
Init_gtk_eventbox()
{
    VALUE gEventBox = G_DEF_CLASS(GTK_TYPE_EVENT_BOX, "EventBox", mGtk);

    rb_define_method(gEventBox, "initialize", eventbox_initialize, 0);
}
