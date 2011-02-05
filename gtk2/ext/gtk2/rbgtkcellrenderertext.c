/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrenderertext.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"


#define _SELF(s) (GTK_CELL_RENDERER_TEXT(RVAL2GOBJ(s)))

static VALUE
crtext_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_text_new());
    return Qnil;
}

static VALUE
crtext_set_fixed_height_from_font(VALUE self, VALUE number_of_rows)
{
    gtk_cell_renderer_text_set_fixed_height_from_font(_SELF(self), NUM2INT(number_of_rows));
    return self;
}

void
Init_gtk_cellrenderertext()
{
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_TEXT, "CellRendererText", mGtk);
    
    rb_define_method(renderer, "initialize", crtext_initialize, 0);
    rb_define_method(renderer, "set_fixed_height_from_font", crtext_set_fixed_height_from_font, 1);

    G_DEF_SETTERS(renderer);
}


 
