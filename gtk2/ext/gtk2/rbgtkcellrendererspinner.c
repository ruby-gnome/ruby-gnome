/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrendererspinner.c -

  $Author$
  $Date$

  Copyright (C) 2011 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,20,0)

#define _SELF(s) (GTK_CELL_RENDERER_SPINNER(RVAL2GOBJ(s)))

static VALUE
crspinner_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_spinner_new());
    return Qnil;
}

#endif

void
Init_gtk_cellrendererspinner(void)
{
#if GTK_CHECK_VERSION(2,20,0)
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_SPINNER, "CellRendererSpinner", mGtk);
    rb_define_method(renderer, "initialize", crspinner_initialize, 0);
#endif
}

