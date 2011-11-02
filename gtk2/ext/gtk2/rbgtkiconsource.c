/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#define RG_TARGET_NAMESPACE cIconSource
#define _SELF(s) ((GtkIconSource*)RVAL2BOXED(s, GTK_TYPE_ICON_SOURCE))

static VALUE
rg_initialize(VALUE self)
{
    GtkIconSource *icon_source;

    icon_source = gtk_icon_source_new();
    G_INITIALIZE(self, icon_source);
    return Qnil;
}

static VALUE
rg_direction(VALUE self)
{
    return GENUM2RVAL(gtk_icon_source_get_direction(_SELF(self)), 
                      GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
rg_direction_wildcarded_p(VALUE self)
{
    return CBOOL2RVAL(gtk_icon_source_get_direction_wildcarded(_SELF(self)));
}

static VALUE
rg_filename(VALUE self)
{
    return CSTR2RVAL(gtk_icon_source_get_filename(_SELF(self)));
}

static VALUE
rg_pixbuf(VALUE self)
{
    GdkPixbuf *pixbuf;
    pixbuf = gtk_icon_source_get_pixbuf(_SELF(self));
    return pixbuf ? GOBJ2RVAL(pixbuf) : Qnil;
}

static VALUE
rg_size(VALUE self)
{
    return GENUM2RVAL(gtk_icon_source_get_size(_SELF(self)), GTK_TYPE_ICON_SIZE);
}

static VALUE
rg_size_wildcarded_p(VALUE self)
{
    return CBOOL2RVAL(gtk_icon_source_get_size_wildcarded(_SELF(self)));
}

static VALUE
rg_state(VALUE self)
{
    return GENUM2RVAL(gtk_icon_source_get_state(_SELF(self)), GTK_TYPE_STATE_TYPE);
}

static VALUE
rg_state_wildcarded_p(VALUE self)
{
    return CBOOL2RVAL(gtk_icon_source_get_state_wildcarded(_SELF(self)));
}

static VALUE
rg_set_direction(VALUE self, VALUE direction)
{
    gtk_icon_source_set_direction(_SELF(self),
                                  RVAL2GENUM(direction, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
rg_set_direction_wildcarded(VALUE self, VALUE setting)
{
    gtk_icon_source_set_direction_wildcarded(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_set_filename(VALUE self, VALUE filename)
{
    gtk_icon_source_set_filename(_SELF(self), RVAL2CSTR(filename));
    return self;
}

static VALUE
rg_set_pixbuf(VALUE self, VALUE pixbuf)
{
    gtk_icon_source_set_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}

static VALUE
rg_set_size(VALUE self, VALUE size)
{
    gtk_icon_source_set_size(_SELF(self), RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));
    return self;
}

static VALUE
rg_set_size_wildcarded(VALUE self, VALUE setting)
{
    gtk_icon_source_set_size_wildcarded(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_set_state(VALUE self, VALUE state)
{
    gtk_icon_source_set_state(_SELF(self), RVAL2GENUM(state, GTK_TYPE_STATE_TYPE));
    return self;
}

static VALUE
rg_set_state_wildcarded(VALUE self, VALUE setting)
{
    gtk_icon_source_set_state_wildcarded(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

void
Init_gtk_icon_source(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ICON_SOURCE, "IconSource", mGtk);

    RG_DEF_METHOD(initialize, 0);

    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD_P(direction_wildcarded, 0);
    RG_DEF_METHOD(filename, 0);
    RG_DEF_METHOD(pixbuf, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD_P(size_wildcarded, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD_P(state_wildcarded, 0);

    RG_DEF_METHOD(set_direction, 1);
    RG_DEF_METHOD(set_direction_wildcarded, 1);
    RG_DEF_METHOD(set_filename, 1);
    RG_DEF_METHOD(set_pixbuf, 1);
    RG_DEF_METHOD(set_size, 1);
    RG_DEF_METHOD(set_size_wildcarded, 1);
    RG_DEF_METHOD(set_state, 1);
    RG_DEF_METHOD(set_state_wildcarded, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
