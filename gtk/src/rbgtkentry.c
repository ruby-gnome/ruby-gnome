/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkentry.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_ENTRY(RVAL2GOBJ(self)))

static VALUE
entry_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_entry_new());
    return Qnil;
}

static VALUE
entry_get_layout(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_entry_get_layout(_SELF(self)));
}

static VALUE
entry_get_layout_offsets(self)
    VALUE self;
{
    int x, y;
    gtk_entry_get_layout_offsets(_SELF(self), &x, &y);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

void 
Init_gtk_entry()
{
    VALUE gEntry = G_DEF_CLASS(GTK_TYPE_ENTRY, "Entry", mGtk);

    rb_define_method(gEntry, "initialize", entry_initialize, 0);
    rb_define_method(gEntry, "layout", entry_get_layout, 0);
    rb_define_method(gEntry, "layout_offsets", entry_get_layout_offsets, 0);
}
