/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkwindow.c,v 1.2 2003/08/28 17:41:27 isambart Exp $ */
/* OpenGL extension to Gdk::Window
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

#define _SELF(i) _WINDOW(i)

static VALUE
window_set_gl_capability(self, glconfig)
    VALUE self, glconfig;
{
    GdkGLWindow* glwindow;

    glwindow = gdk_window_set_gl_capability(
        _SELF(self), _GL_CONFIG(glconfig), NULL);
    return GOBJ2RVAL(glwindow);
}

static VALUE
window_unset_gl_capability(self)
    VALUE self;
{
    gdk_window_unset_gl_capability(_SELF(self));
    return self;
}

static VALUE
window_is_gl_capable(self)
    VALUE self;
{
    gboolean capable;

    capable = gdk_window_is_gl_capable(_SELF(self));
    return CBOOL2RVAL(capable);
}

static VALUE
window_get_gl_window(self)
    VALUE self;
{
    GdkGLWindow* glwindow;

    glwindow = gdk_window_get_gl_window(_SELF(self));
    return GOBJ2RVAL(glwindow);
}

static VALUE
window_get_gl_drawable(self)
    VALUE self;
{
    GdkGLDrawable* gldrawable;

    gldrawable = gdk_window_get_gl_drawable(_SELF(self));
    return GOBJ2RVAL(gldrawable);
}

void
Init_gtkglext_gdk_window(void)
{
    /* Gtk::Window */
    VALUE Window = rb_const_get(mGdk, rb_intern("Window"));

    rb_define_method(Window, "set_gl_capability",   window_set_gl_capability,   2);
    rb_define_method(Window, "unset_gl_capability", window_unset_gl_capability, 0);
    rb_define_method(Window, "gl_capable?",         window_is_gl_capable,       0);
    rb_define_method(Window, "gl_window",           window_get_gl_window,       0);
    rb_define_method(Window, "gl_drawable",         window_get_gl_drawable,     0);
}
