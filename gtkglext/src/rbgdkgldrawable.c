/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkgldrawable.c,v 1.2 2003/08/20 22:36:02 isambart Exp $ */
/* Gdk::GLDrawable Widget
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

#define _GL_DRAWABLE(i) GDK_GL_DRAWABLE(RVAL2GOBJ(i))
#define _CONTEXT(i)     GDK_GL_CONTEXT(RVAL2GOBJ(i))
#define _SELF(i)        _GL_DRAWABLE(i)

static VALUE
gldrawable_make_current(self, context)
    VALUE self, context;
{
    gboolean ok;
    ok = gdk_gl_drawable_make_current(_SELF(self), _CONTEXT(context));
    return CBOOL2RVAL(ok);
}

static VALUE
gldrawable_is_double_buffered(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_drawable_is_double_buffered(_SELF(self)));
}

static VALUE
gldrawable_swap_buffers(self)
    VALUE self;
{
    gdk_gl_drawable_swap_buffers(_SELF(self));
    return self;
}

static VALUE
gldrawable_wait_gl(self)
    VALUE self;
{
    gdk_gl_drawable_wait_gl(_SELF(self));
    return self;
}

static VALUE
gldrawable_wait_gdk(self)
    VALUE self;
{
    gdk_gl_drawable_wait_gdk(_SELF(self));
    return self;
}

static VALUE
gldrawable_gl_begin(self, context)
    VALUE self, context;
{
    return CBOOL2RVAL(gdk_gl_drawable_gl_begin(_SELF(self), _CONTEXT(context)));
}

static VALUE
gldrawable_gl_end(self)
    VALUE self;
{
    gdk_gl_drawable_gl_end(_SELF(self));
    return self;
}

static VALUE
gldrawable_get_gl_config(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_drawable_get_gl_config(_SELF(self)));
}

static VALUE
gldrawable_get_size(self)
    VALUE self;
{
    int width, height;

    gdk_gl_drawable_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gldrawable_get_current(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_drawable_get_current());
}

void
Init_gtkglext_gdk_gldrawable(void)
{
    /* Gdk::GLDrawable */
    VALUE GLDrawable = G_DEF_CLASS(GDK_TYPE_GL_DRAWABLE, "GLDrawable", mGdk);

    rb_define_method(GLDrawable, "make_current",     gldrawable_make_current,       1);
    rb_define_method(GLDrawable, "double_buffered?", gldrawable_is_double_buffered, 0);
    rb_define_method(GLDrawable, "swap_buffers",     gldrawable_swap_buffers,       0);
    rb_define_method(GLDrawable, "wait_gl",          gldrawable_wait_gl,            0);
    rb_define_method(GLDrawable, "wait_gdk",         gldrawable_wait_gdk,           0);
    rb_define_method(GLDrawable, "gl_begin",         gldrawable_gl_begin,           1);
    rb_define_method(GLDrawable, "gl_end",           gldrawable_gl_end,             0);
    rb_define_method(GLDrawable, "gl_config",        gldrawable_get_gl_config,      0);
    rb_define_method(GLDrawable, "size",             gldrawable_get_size,           0);

    rb_define_singleton_method(GLDrawable, "current", gldrawable_get_current, 0);
}
