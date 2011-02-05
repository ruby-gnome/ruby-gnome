/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscale.c -

  $Author: mutoh $
  $Date: 2004/11/14 11:01:43 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_SCALE(RVAL2GOBJ(self)))

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rbscale_get_layout(VALUE self)
{
    return GOBJ2RVAL(gtk_scale_get_layout(_SELF(self)));
}

static VALUE
rbscale_get_layout_offsets(VALUE self)
{
    gint x, y;
    gtk_scale_get_layout_offsets(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}
#endif

void 
Init_gtk_scale()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gScale = G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);
    rb_define_method(gScale, "layout", rbscale_get_layout, 0);
    rb_define_method(gScale, "layout_offsets", rbscale_get_layout_offsets, 0);
#else
    G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);
#endif
}
