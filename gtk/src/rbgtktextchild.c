/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextchild.c -

  $Author: mutoh $
  $Date: 2002/10/25 17:51:26 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

#define _SELF(self) GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(self))

static VALUE
textchild_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_text_child_anchor_new());
    return Qnil;
}

static VALUE
textchild_get_widgets(self)
    VALUE self;
{
    return GLIST2ARY(gtk_text_child_anchor_get_widgets(_SELF(self)));
}

static VALUE
textchild_get_deleted(self)
    VALUE self;
{
    return gtk_text_child_anchor_get_deleted(_SELF(self)) ? Qtrue : Qfalse;
}

void
Init_gtk_textchild()
{
    VALUE gTextChildAnchor = G_DEF_CLASS(GTK_TYPE_TEXT_CHILD_ANCHOR, "TextChildAnchor", mGtk);

    rb_define_method(gTextChildAnchor, "initialize", textchild_initialize, 0);
    rb_define_method(gTextChildAnchor, "widgets", textchild_get_widgets, 0);
    rb_define_method(gTextChildAnchor, "deleted?", textchild_get_deleted, 0);
}
