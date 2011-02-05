/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextchild.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:10 $

  Copyright (C) 2002,2003 Masahiro Sakai
  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#define _SELF(self) GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(self))

static VALUE
textchild_initialize(int argc, VALUE *argv, VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_text_child_anchor_new());
    return Qnil;
}

static VALUE
textchild_get_widgets(VALUE self)
{
    return GLIST2ARYF(gtk_text_child_anchor_get_widgets(_SELF(self)));
}

static VALUE
textchild_get_deleted(VALUE self)
{
    return CBOOL2RVAL(gtk_text_child_anchor_get_deleted(_SELF(self)));
}

void
Init_gtk_textchild()
{
    VALUE gTextChildAnchor = G_DEF_CLASS(GTK_TYPE_TEXT_CHILD_ANCHOR, "TextChildAnchor", mGtk);

    rb_define_method(gTextChildAnchor, "initialize", textchild_initialize, 0);
    rb_define_method(gTextChildAnchor, "widgets", textchild_get_widgets, 0);
    rb_define_method(gTextChildAnchor, "deleted?", textchild_get_deleted, 0);
}
