/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrendereraccel.c -

  $Author: mutoh $
  $Date: 2007/02/02 19:19:59 $

  Copyright (C) 2006 Ruby-GNOME2 Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(s) (GTK_CELL_RENDERER_ACCEL(RVAL2GOBJ(s)))

static VALUE
craccel_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_accel_new());
    return Qnil;
}

#endif

void
Init_gtk_cellrendereraccel()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_ACCEL, "CellRendererAccel", mGtk);
    rb_define_method(renderer, "initialize", craccel_initialize, 0);

    /* GtkCellRendererAccelMode */
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_ACCEL_MODE, "Mode", renderer);
    G_DEF_CONSTANTS(renderer, GTK_TYPE_CELL_RENDERER_ACCEL_MODE, "GTK_CELL_RENDERER_ACCEL_");
#endif
}


 
