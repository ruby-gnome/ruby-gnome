/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglpixmap.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
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

#define _GL_PIXMAP(i) GDK_GL_PIXMAP(RVAL2GOBJ(i))
#define _GL_CONFIG(i) GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _PIXMAP(i)    GDK_PIXMAP(RVAL2GOBJ(i))

#define _SELF(i) _GL_PIXMAP(i)
static VALUE
gdkglpixmap_initialize(self, glconfig, pixmap)
    VALUE self, glconfig, pixmap;
{
    G_INITIALIZE(self, gdk_gl_pixmap_new(_GL_CONFIG(glconfig),
                                         _PIXMAP(pixmap),
                                         NULL));
    return Qnil;
}

static VALUE
gdkglpixmap_destroy(self)
    VALUE self;
{
    gdk_gl_pixmap_destroy(_SELF(self));
    return Qnil;
}

static VALUE
gdkglpixmap_get_pixmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_pixmap_get_pixmap(_SELF(self)));
}

#undef _SELF

/* OpenGL extension to GdkPixmap */
#define _SELF(i) _PIXMAP(i)

static VALUE
gdkpixmap_set_gl_capability(self, glconfig)
    VALUE self, glconfig;
{
    return GOBJ2RVAL(gdk_pixmap_set_gl_capability(_SELF(self),
                                                  _GL_CONFIG(glconfig),
                                                  NULL));
}

static VALUE
gdkpixmap_unset_gl_capability(self)
    VALUE self;
{
    gdk_pixmap_unset_gl_capability(_SELF(self));
    return self;
}

static VALUE
gdkpixmap_is_gl_capable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_pixmap_is_gl_capable(_SELF(self)));
}

static VALUE
gdkpixmap_get_gl_pixmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixmap_get_gl_pixmap(_SELF(self)));
}

void
Init_gdk_gl_pixmap(void)
{
    VALUE gdkGlPixmap = G_DEF_CLASS(GDK_TYPE_GL_PIXMAP, "Pixmap", mGdkGl);

    rb_define_method(gdkGlPixmap, "initialize", gdkglpixmap_initialize, 3);
    rb_define_method(gdkGlPixmap, "destroy", gdkglpixmap_destroy, 0);
    rb_define_method(gdkGlPixmap, "pixmap", gdkglpixmap_get_pixmap, 0);

    /* OpenGL extension to GdkPixmap */
    VALUE gdkPixmap = rb_const_get(mGdk, rb_intern("Pixmap"));

    rb_define_method(gdkPixmap, "set_gl_capability", gdkpixmap_set_gl_capability, 2);
    rb_define_method(gdkPixmap, "unset_gl_capability", gdkpixmap_unset_gl_capability, 0);
    rb_define_method(gdkPixmap, "gl_capable?", gdkpixmap_is_gl_capable, 0);
    rb_define_method(gdkPixmap, "gl_pixmap", gdkpixmap_get_gl_pixmap, 0);
}
