/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkpixmap.c,v 1.3 2006/05/27 13:50:21 ktou Exp $ */
/* OpenGL extension to Gdk::Pixmap
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

#define _GL_PIXMAP(i) GDK_GL_PIXMAP(RVAL2GOBJ(i))
#define _GL_CONFIG(i) GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _PIXMAP(i)    GDK_PIXMAP(RVAL2GOBJ(i))

#define _SELF(i) _PIXMAP(i)

static VALUE
pixmap_set_gl_capability(self, glconfig)
    VALUE self, glconfig;
{
    GdkGLPixmap* glpixmap;
    
    glpixmap = gdk_pixmap_set_gl_capability(
        _SELF(self), _GL_CONFIG(glconfig), NULL);
    return GOBJ2RVAL(glpixmap);
}

static VALUE
pixmap_unset_gl_capability(self)
    VALUE self;
{
    gdk_pixmap_unset_gl_capability(_SELF(self));
    return self;
}

static VALUE
pixmap_is_gl_capable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_pixmap_is_gl_capable(_SELF(self)));
}

static VALUE
pixmap_get_gl_pixmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixmap_get_gl_pixmap(_SELF(self)));
}

static VALUE
pixmap_get_gl_drawable(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixmap_get_gl_drawable(_SELF(self)));
}

void
Init_gtkglext_gdk_pixmap(void)
{
    /* Gdk::Pixmap */
    VALUE Pixmap = rb_const_get(mGdk, rb_intern("Pixmap"));

    rb_define_method(Pixmap, "set_gl_capability",   pixmap_set_gl_capability,   1);
    rb_define_method(Pixmap, "unset_gl_capability", pixmap_unset_gl_capability, 0);
    rb_define_method(Pixmap, "gl_capable?",         pixmap_is_gl_capable,       0);
    rb_define_method(Pixmap, "gl_pixmap",           pixmap_get_gl_pixmap,       0);
    rb_define_method(Pixmap, "gl_drawable",         pixmap_get_gl_drawable,     0);

    G_DEF_SETTERS(Pixmap);
}
