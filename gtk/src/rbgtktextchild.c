/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextchild.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

static VALUE
initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_text_child_anchor_new());
    return Qnil;
}

static void
collect_widgets(widget, ary)
    gpointer widget;
    VALUE ary;
{
    rb_ary_push(ary, GOBJ2RVAL(widget));
}

static VALUE
get_widgets(self)
    VALUE self;
{
    GList* list = gtk_text_child_anchor_get_widgets(
        GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(self)));
    VALUE result = rb_ary_new();
    g_list_foreach(list, collect_widgets, (gpointer)result);
    return result;
}

static VALUE
get_deleted(self)
    VALUE self;
{
    return gtk_text_child_anchor_get_deleted(GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

void
Init_gtk_textchild()
{
    VALUE gTextChildAnchor = G_DEF_CLASS(GTK_TYPE_TEXT_CHILD_ANCHOR, "TextChildAnchor", mGtk);

    rb_define_method(gTextChildAnchor, "widgets", get_widgets, 0);
    rb_define_method(gTextChildAnchor, "deleted?", get_deleted, 0);
}
