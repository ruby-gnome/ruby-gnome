/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglcontext.c,v 1.2 2003/08/20 22:36:02 isambart Exp $ */
/* Gdk::GLContext
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
#define _GL_CONTEXT(i)  GDK_GL_CONTEXT(RVAL2GOBJ(i))
#define _SELF(i)        _GL_CONTEXT(i)

static VALUE
glcontext_initialize(self, gldrawable, share_list, direct, render_type)
    VALUE self, gldrawable, share_list, direct, render_type;
{
    GdkGLContext* glcontext;
    
    glcontext = gdk_gl_context_new(_GL_DRAWABLE(gldrawable),
                                   _GL_CONTEXT(share_list),
                                   RVAL2CBOOL(direct),
                                   NUM2INT(render_type));
    G_INITIALIZE(self, glcontext);
    return Qnil;
}

static VALUE
glcontext_destroy(self)
    VALUE self;
{
    gdk_gl_context_destroy(_SELF(self));
    return Qnil;
}

static VALUE
glcontext_copy(self, src, mask)
    VALUE self, src, mask;
{
    gboolean ok;
    
    ok = gdk_gl_context_copy(_SELF(self), _GL_CONTEXT(src), NUM2INT(mask));
    return CBOOL2RVAL(ok);
}

static VALUE
glcontext_get_gl_drawable(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_gl_drawable(_SELF(self)));
}

static VALUE
glcontext_get_gl_config(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_gl_config(_SELF(self)));
}

static VALUE
glcontext_get_share_list(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_share_list(_SELF(self)));
}

static VALUE
glcontext_is_direct(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_context_is_direct(_SELF(self)));
}

static VALUE
glcontext_get_render_type(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_context_get_render_type(_SELF(self)));
}

static VALUE
glcontext_get_current(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_context_get_current());
}

void
Init_gtkglext_gdk_glcontext(void)
{
    /* Gdk::GLContext */
    VALUE GLContext = G_DEF_CLASS(GDK_TYPE_GL_CONTEXT, "GLContext", mGdk);

    rb_define_method(GLContext, "initialize",  glcontext_initialize,      4);
    rb_define_method(GLContext, "destroy",     glcontext_destroy,         0);
    rb_define_method(GLContext, "copy",        glcontext_copy,            2);
    rb_define_method(GLContext, "gl_drawable", glcontext_get_gl_drawable, 0);
    rb_define_method(GLContext, "gl_config",   glcontext_get_gl_config,   0);
    rb_define_method(GLContext, "share_list",  glcontext_get_share_list,  0);
    rb_define_method(GLContext, "direct?",     glcontext_is_direct,       0);
    rb_define_method(GLContext, "render_type", glcontext_get_render_type, 0);

    rb_define_singleton_method(GLContext, "current", glcontext_get_current, 0);
}
