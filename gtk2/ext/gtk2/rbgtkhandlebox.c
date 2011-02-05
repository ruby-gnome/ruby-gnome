/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhandlebox.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:09 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hb_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_handle_box_new());
    return Qnil;
}

static VALUE
hb_child_detached(VALUE self)
{
    return CBOOL2RVAL(GTK_HANDLE_BOX(RVAL2GOBJ(self))->child_detached);
}

void 
Init_gtk_handle_box()
{
    VALUE gHandleBox = G_DEF_CLASS(GTK_TYPE_HANDLE_BOX, "HandleBox", mGtk);

    rb_define_method(gHandleBox, "initialize", hb_initialize, 0);
    rb_define_method(gHandleBox, "child_detached?", hb_child_detached, 0);
    rb_undef_method(gHandleBox, "shadow");
    rb_undef_method(gHandleBox, "shadow=");
    rb_undef_method(gHandleBox, "set_shadow");
}
