/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvolumebutton.c -

  $Author: ggc $
  $Date: 2007/07/03 15:07:14 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
volumebutton_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_volume_button_new());
    return Qnil;
}

void 
Init_gtk_volumebutton()
{
    VALUE gVolumeButton = G_DEF_CLASS(GTK_TYPE_VOLUME_BUTTON, "VolumeButton", mGtk);
    rb_define_method(gVolumeButton, "initialize", volumebutton_initialize, 0);
}
