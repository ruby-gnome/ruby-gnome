/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhandlebox.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:16 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hb_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_handle_box_new());
    return Qnil;
}

static VALUE
hb_child_detached(self)
    VALUE self;
{
    return GTK_HANDLE_BOX(RVAL2GOBJ(self))->child_detached ? Qtrue : Qfalse;
}

void 
Init_gtk_handle_box()
{
    VALUE gHandleBox = G_DEF_CLASS(GTK_TYPE_HANDLE_BOX, "HandleBox", mGtk);

    rb_define_method(gHandleBox, "initialize", hb_initialize, 0);
    rb_define_method(gHandleBox, "child_detached?", hb_child_detached, 0);
}
