/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrendererspin.c -

  $Author: mutoh $
  $Date: 2006/10/21 16:58:00 $

  Copyright (C) 2006 Ruby-GNOME2 Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (GTK_CELL_RENDERER_SPIN(RVAL2GOBJ(s)))

static VALUE
crspin_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_spin_new());
    return Qnil;
}

#endif

void
Init_gtk_cellrendererspin()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_SPIN, "CellRendererSpin", mGtk);
    rb_define_method(renderer, "initialize", crspin_initialize, 0);
#endif
}


 
