/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglwindow.c,v 1.3 2003/08/20 22:36:02 isambart Exp $ */
/* Gdk::GLWindow widget
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
glwindow_initialize(self, glconfig, window)
    VALUE self, glconfig, window;
{
    G_INITIALIZE(self, gdk_gl_window_new(_GL_CONFIG(glconfig),
                                         _WINDOW(window),
                                         NULL));
    return Qnil;
}

static VALUE
glwindow_destroy(self)
    VALUE self;
{
    gdk_gl_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
glwindow_get_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_window_get_window(_SELF(self)));
}

void
Init_gtkglext_gdk_glwindow(void)
{
    /* Gdk::GLWindow */
    VALUE GLWindow = G_DEF_CLASS(GDK_TYPE_GL_WINDOW, "GLWindow", mGdk);

    rb_define_method(GLWindow, "initialize", glwindow_initialize, 3);
    rb_define_method(GLWindow, "destroy",    glwindow_destroy,    0);
    rb_define_method(GLWindow, "window",     glwindow_get_window, 0);
}
