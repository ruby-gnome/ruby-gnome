/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrenderercombo.c -

  $Author: mutoh $
  $Date: 2005/01/03 18:55:03 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

static VALUE
crcombo_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_combo_new());
    return Qnil;
}

#endif

void
Init_gtk_cellrenderercombo()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_COMBO, "CellRendererCombo", mGtk);
    rb_define_method(renderer, "initialize", crcombo_initialize, 0);
#endif
}


 

