/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrendererprogress.c -

  $Author: mutoh $
  $Date: 2004/12/27 16:32:56 $

  Copyright (C) 2004 Darren Willis
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

static VALUE
crprogress_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_progress_new());
    return Qnil;
}

#endif

void
Init_gtk_cellrendererprogress()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_PROGRESS, "CellRendererProgress", mGtk);
    rb_define_method(renderer, "initialize", crprogress_initialize, 0);
#endif
}


 

