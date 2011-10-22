/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masahiro Sakai
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

static VALUE
initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name;
    rb_scan_args(argc, argv, "01", &name);
    G_INITIALIZE(self, gtk_text_tag_new(NIL_P(name) ? NULL : RVAL2CSTR(name)));
    return Qnil;
}

static VALUE
get_priority(VALUE self)
{
    return INT2NUM(gtk_text_tag_get_priority(GTK_TEXT_TAG(RVAL2GOBJ(self))));
}

static VALUE
set_priority(VALUE self, VALUE priority)
{
    gtk_text_tag_set_priority(GTK_TEXT_TAG(RVAL2GOBJ(self)), NUM2INT(priority));
    return priority;
}

static VALUE
event(VALUE self, VALUE event_object, VALUE event, VALUE iter)
{
    gboolean ret = gtk_text_tag_event(GTK_TEXT_TAG(RVAL2GOBJ(self)), 
                                      RVAL2GOBJ(event_object),
                                      RVAL2GEV(event),
                                      (GtkTextIter*)RVAL2BOXED(iter, GTK_TYPE_TEXT_ITER));
    return CBOOL2RVAL(ret);
}

void
Init_gtk_texttag(void)
{
    VALUE gTextTag = G_DEF_CLASS(GTK_TYPE_TEXT_TAG, "TextTag", mGtk);
    rb_define_method(gTextTag, "initialize", initialize, -1);
    rb_define_method(gTextTag, "priority", get_priority, 0);
    rb_define_method(gTextTag, "set_priority", set_priority, 1);
    G_DEF_SETTER(gTextTag, "priority");
    rb_define_method(gTextTag, "event", event, 3);

    /* GtkWrapMode */
    G_DEF_CLASS(GTK_TYPE_WRAP_MODE, "WrapMode", gTextTag);
    G_DEF_CONSTANTS(gTextTag, GTK_TYPE_WRAP_MODE, "GTK_");
}
