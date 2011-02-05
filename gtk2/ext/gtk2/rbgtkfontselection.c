/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************

  rbgtkfontselection.c -

  $Author: mutoh $
  $Date: 2003/02/22 18:01:09 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
**********************************************************/

#include "global.h"

static VALUE
fs_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_font_selection_new());
    return Qnil;
}

void 
Init_gtk_font_selection()
{
    VALUE gFontSelection = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION, "FontSelection", mGtk);

    rb_define_method(gFontSelection, "initialize", fs_initialize, 0);
    rb_undef_method(gFontSelection, "font");
}
