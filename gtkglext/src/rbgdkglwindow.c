/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglwindow.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
/*
 * Copyright (C) 2003 Vincent Isambart <isambart@netcourrier.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgtkglext.h"

#define _GL_WINDOW(i) GDK_GL_WINDOW(RVAL2GOBJ(i))
#define _GL_CONFIG(i) GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _WINDOW(i)    GDK_WINDOW(RVAL2GOBJ(i))

#define _SELF(i) _GL_WINDOW(i)
static VALUE
gdkglwindow_initialize(self, glconfig, window)
    VALUE self, glconfig, window;
{
    G_INITIALIZE(self, gdk_gl_window_new(_GL_CONFIG(glconfig),
                                         _WINDOW(window),
                                         NULL));
    return Qnil;
}

static VALUE
gdkglwindow_destroy(self)
    VALUE self;
{
    gdk_gl_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
gdkglwindow_get_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_window_get_window(_SELF(self)));
}

#undef _SELF

/* OpenGL extension to GdkWindow */
#define _SELF(i) _WINDOW(i)

static VALUE
gdkwindow_set_gl_capability(self, glconfig)
    VALUE self, glconfig;
{
    return GOBJ2RVAL(gdk_window_set_gl_capability(_SELF(self),
                                                  _GL_CONFIG(glconfig),
                                                  NULL));
}

static VALUE
gdkwindow_unset_gl_capability(self)
    VALUE self;
{
    gdk_window_unset_gl_capability(_SELF(self));
    return self;
}

static VALUE
gdkwindow_is_gl_capable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_window_is_gl_capable(_SELF(self)));
}

static VALUE
gdkwindow_get_gl_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_window_get_gl_window(_SELF(self)));
}

void
Init_gdk_gl_window(void)
{
    VALUE gdkGlWindow = G_DEF_CLASS(GDK_TYPE_GL_WINDOW, "Window", mGdkGl);

    rb_define_method(gdkGlWindow, "initialize", gdkglwindow_initialize, 3);
    rb_define_method(gdkGlWindow, "destroy", gdkglwindow_destroy, 0);
    rb_define_method(gdkGlWindow, "window", gdkglwindow_get_window, 0);

    /* OpenGL extension to GdkWindow */
    VALUE gdkWindow = rb_const_get(mGdk, rb_intern("Window"));

    rb_define_method(gdkWindow, "set_gl_capability", gdkwindow_set_gl_capability, 2);
    rb_define_method(gdkWindow, "unset_gl_capability", gdkwindow_unset_gl_capability, 0);
    rb_define_method(gdkWindow, "gl_capable?", gdkwindow_is_gl_capable, 0);
    rb_define_method(gdkWindow, "gl_window", gdkwindow_get_gl_window, 0);
}
