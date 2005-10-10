/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-print-preview.c -

  $Author: ktou $
  $Date: 2005/10/10 01:42:03 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#include <rbart.h>

#include <libgnomeprintui/gnome-print-preview.h>

#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
gpui_preview_new(int argc, VALUE *argv, VALUE self)
{
  VALUE config, canvas, transform, region;
  int n_args;

  n_args = rb_scan_args(argc, argv, "22",
                        &config, &canvas, &transform, &region);

  if (n_args == 2) {
    RBGTK_INITIALIZE(self, gnome_print_preview_new(RVAL2GOBJ(config),
                                                   RVAL2GOBJ(canvas)));
  } else if (n_args == 4) {
    RBGTK_INITIALIZE(self,
                     gnome_print_preview_new_full(RVAL2GOBJ(config),
                                                  RVAL2GOBJ(canvas),
                                                  get_art_affine(transform),
                                                  get_art_drect(region)));
  } else {
    rb_raise(rb_eArgError,
             "wrong number of arguments (%d for 2 or 4)",
             argc);
  }
  return Qnil;
}

void
Init_gnome_print_preview(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_PREVIEW,
                        "PrintPreview", mGnome);

  rb_define_method(c, "initialize", gpui_preview_new, -1);
}
