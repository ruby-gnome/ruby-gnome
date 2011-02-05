/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkeventbox.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
eventbox_initialize(VALUE self)
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
