/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktexttag.c -

  $Author: mutoh $
  $Date: 2003/12/21 08:22:19 $

  Copyright (C) 2002,2003 Masahiro Sakai
************************************************/

#include "global.h"

static VALUE
initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name;
    rb_scan_args(argc, argv, "01", &name);
    G_INITIALIZE(self, gtk_text_tag_new(NIL_P(name) ? NULL : RVAL2CSTR(name)));
    return Qnil;
}

static VALUE
get_priority(self)
    VALUE self;
{
    return INT2NUM(gtk_text_tag_get_priority(GTK_TEXT_TAG(RVAL2GOBJ(self))));
}

static VALUE
set_priority(self, priority)
    VALUE self, priority;
{
    gtk_text_tag_set_priority(GTK_TEXT_TAG(RVAL2GOBJ(self)), NUM2INT(priority));
    return priority;
}

static VALUE
event(self, event_object, event, iter)
    VALUE self, event_object, event, iter;
{
    gboolean ret = gtk_text_tag_event(GTK_TEXT_TAG(RVAL2GOBJ(self)), 
                                      RVAL2GOBJ(event_object),
                                      RVAL2GEV(event),
                                      (GtkTextIter*)RVAL2BOXED(iter, GTK_TYPE_TEXT_ITER));
    return ret ? Qtrue : Qfalse;
}

void
Init_gtk_texttag()
{
    VALUE gTextTag = G_DEF_CLASS(GTK_TYPE_TEXT_TAG, "TextTag", mGtk);
    rb_define_method(gTextTag, "initialize", initialize, -1);
    rb_define_method(gTextTag, "priority", get_priority, 0);
    rb_define_method(gTextTag, "set_priority", set_priority, 1);
    rb_define_method(gTextTag, "event", event, 3);

    G_DEF_SETTERS(gTextTag);

    /* GtkWrapMode */
    G_DEF_CLASS(GTK_TYPE_WRAP_MODE, "WrapMode", gTextTag);
    G_DEF_CONSTANTS(gTextTag, GTK_TYPE_WRAP_MODE, "GTK_");
}
