/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkeventbox.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:34 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
eventbox_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_event_box_new());
    return Qnil;
}

void Init_gtk_eventbox()
{
    gEventBox = rb_define_class_under(mGtk, "EventBox", gBin);

    rb_define_method(gEventBox, "initialize", eventbox_initialize, 0);
}
