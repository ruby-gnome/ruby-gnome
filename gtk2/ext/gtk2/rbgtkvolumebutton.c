/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvolumebutton.c -

  $Author: ggc $
  $Date: 2007/07/03 15:17:10 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,12,0)

static VALUE
volumebutton_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_volume_button_new());
    return Qnil;
}

#endif

void 
Init_gtk_volumebutton()
{
#if GTK_CHECK_VERSION(2,12,0)
    VALUE gVolumeButton = G_DEF_CLASS(GTK_TYPE_VOLUME_BUTTON, "VolumeButton", mGtk);
    rb_define_method(gVolumeButton, "initialize", volumebutton_initialize, 0);
#endif
}
