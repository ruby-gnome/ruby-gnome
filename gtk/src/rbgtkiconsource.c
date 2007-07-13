/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconsource.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:09 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) ((GtkIconSource*)RVAL2BOXED(s, GTK_TYPE_ICON_SOURCE))

static VALUE
icon_source_initialize(self)
    VALUE self;
{
    GtkIconSource *icon_source;

    icon_source = gtk_icon_source_new();
    G_INITIALIZE(self, icon_source);
    return Qnil;
}

static VALUE
icon_source_get_direction(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_icon_source_get_direction(_SELF(self)), 
                      GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
icon_source_get_direction_wildcarded(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_icon_source_get_direction_wildcarded(_SELF(self)));
}

static VALUE
icon_source_get_filename(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_icon_source_get_filename(_SELF(self)));
}

static VALUE
icon_source_get_pixbuf(self)
    VALUE self;
{
    GdkPixbuf *pixbuf;
    pixbuf = gtk_icon_source_get_pixbuf(_SELF(self));
    return pixbuf ? GOBJ2RVAL(pixbuf) : Qnil;
}

static VALUE
icon_source_get_size(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_icon_source_get_size(_SELF(self)), GTK_TYPE_ICON_SIZE);
}

static VALUE
icon_source_get_size_wildcarded(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_icon_source_get_size_wildcarded(_SELF(self)));
}

static VALUE
icon_source_get_state(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_icon_source_get_state(_SELF(self)), GTK_TYPE_STATE_TYPE);
}

static VALUE
icon_source_get_state_wildcarded(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_icon_source_get_state_wildcarded(_SELF(self)));
}

static VALUE
icon_source_set_direction(self, direction)
    VALUE self, direction;
{
    gtk_icon_source_set_direction(_SELF(self),
                                  RVAL2GENUM(direction, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
icon_source_set_direction_wildcarded(self, setting)
    VALUE self, setting;
{
    gtk_icon_source_set_direction_wildcarded(_SELF(self), RTEST(setting));
    return self;
}

static VALUE
icon_source_set_filename(self, filename)
    VALUE self, filename;
{
    gtk_icon_source_set_filename(_SELF(self), RVAL2CSTR(filename));
    return self;
}

static VALUE
icon_source_set_pixbuf(self, pixbuf)
    VALUE self, pixbuf;
{
    gtk_icon_source_set_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}

static VALUE
icon_source_set_size(self, size)
    VALUE self, size;
{
    gtk_icon_source_set_size(_SELF(self), RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));
    return self;
}

static VALUE
icon_source_set_size_wildcarded(self, setting)
    VALUE self, setting;
{
    gtk_icon_source_set_size_wildcarded(_SELF(self), RTEST(setting));
    return self;
}

static VALUE
icon_source_set_state(self, state)
    VALUE self, state;
{
    gtk_icon_source_set_state(_SELF(self), RVAL2GENUM(state, GTK_TYPE_STATE_TYPE));
    return self;
}

static VALUE
icon_source_set_state_wildcarded(self, setting)
    VALUE self, setting;
{
    gtk_icon_source_set_state_wildcarded(_SELF(self), RTEST(setting));
    return self;
}

void
Init_icon_source()
{
    VALUE gIconSource = G_DEF_CLASS(GTK_TYPE_ICON_SOURCE, "IconSource", mGtk);

    rb_define_method(gIconSource, "initialize", icon_source_initialize, 0);
    
    rb_define_method(gIconSource, "direction", icon_source_get_direction , 0); ;
    rb_define_method(gIconSource, "direction_wildcarded?", icon_source_get_direction_wildcarded , 0); ;
    rb_define_method(gIconSource, "filename", icon_source_get_filename , 0); ;
    rb_define_method(gIconSource, "pixbuf", icon_source_get_pixbuf , 0); ;
    rb_define_method(gIconSource, "size", icon_source_get_size , 0); ;
    rb_define_method(gIconSource, "size_wildcarded?", icon_source_get_size_wildcarded , 0); ;
    rb_define_method(gIconSource, "state", icon_source_get_state , 0); ;
    rb_define_method(gIconSource, "state_wildcarded?" , icon_source_get_state_wildcarded , 0); ;

    rb_define_method(gIconSource, "set_direction", icon_source_set_direction , 1); ;
    rb_define_method(gIconSource, "set_direction_wildcarded", icon_source_set_direction_wildcarded , 1); ;
    rb_define_method(gIconSource, "set_filename", icon_source_set_filename , 1); ;
    rb_define_method(gIconSource, "set_pixbuf", icon_source_set_pixbuf , 1); ;
    rb_define_method(gIconSource, "set_size", icon_source_set_size , 1); ;
    rb_define_method(gIconSource, "set_size_wildcarded", icon_source_set_size_wildcarded , 1); ;
    rb_define_method(gIconSource, "set_state", icon_source_set_state , 1); ;
    rb_define_method(gIconSource, "set_state_wildcarded", icon_source_set_state_wildcarded , 1); ;

    G_DEF_SETTERS(gIconSource);
}
