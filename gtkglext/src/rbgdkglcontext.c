/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglcontext.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
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

#define _SELF(i) GDK_GL_CONTEXT(RVAL2GOBJ(i))
#define _DRAWABLE(i) GDK_GL_DRAWABLE(RVAL2GOBJ(i))
#define _CONTEXT(i) GDK_GL_CONTEXT(RVAL2GOBJ(i))

static VALUE
gdkglcontext_initialize(self, gldrawable, share_list, direct, render_type)
    VALUE self, gldrawable, share_list, direct, render_type;
{
    G_INITIALIZE(self, gdk_gl_context_new(_DRAWABLE(gldrawable),
                                          _CONTEXT(share_list),
                                          RVAL2CBOOL(direct),
                                          NUM2INT(render_type)));
    return Qnil;
}

static VALUE
gdkglcontext_destroy(self)
    VALUE self;
{
    gdk_gl_context_destroy(_SELF(self));
    return Qnil;
}

static VALUE
gdkglcontext_copy(self, src, mask)
    VALUE self, src, mask;
{
    return CBOOL2RVAL(gdk_gl_context_copy(_SELF(self), _CONTEXT(src), NUM2INT(mask)));
}

static VALUE
gdkglcontext_get_gl_drawable(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_gl_drawable(_SELF(self)));
}

static VALUE
gdkglcontext_get_gl_config(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_gl_config(_SELF(self)));
}

static VALUE
gdkglcontext_get_share_list(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_share_list(_SELF(self)));
}

static VALUE
gdkglcontext_is_direct(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_context_is_direct(_SELF(self)));
}

static VALUE
gdkglcontext_get_render_type(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_context_get_render_type(_SELF(self)));
}

static VALUE
gdkglcontext_get_current(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_current());
}

void
Init_gdk_gl_context(void)
{
    VALUE gdkGlContext = G_DEF_CLASS(GDK_TYPE_GL_CONTEXT, "Context", mGdkGl);

    rb_define_method(gdkGlContext, "initialize", gdkglcontext_initialize, 4);

    rb_define_method(gdkGlContext, "destroy", gdkglcontext_destroy, 0);
    rb_define_method(gdkGlContext, "copy", gdkglcontext_copy, 2);
    rb_define_method(gdkGlContext, "gl_drawable", gdkglcontext_get_gl_drawable, 0);
    rb_define_method(gdkGlContext, "gl_config", gdkglcontext_get_gl_config, 0);
    rb_define_method(gdkGlContext, "share_list", gdkglcontext_get_share_list, 0);
    rb_define_method(gdkGlContext, "direct?", gdkglcontext_is_direct, 0);
    rb_define_method(gdkGlContext, "render_type", gdkglcontext_get_render_type, 0);

    rb_define_singleton_method(gdkGlContext, "current", gdkglcontext_get_current, 0);
}
