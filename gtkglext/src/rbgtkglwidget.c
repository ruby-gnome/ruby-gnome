/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgtkglwidget.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
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

#define _SELF(i) GTK_WIDGET(RVAL2GOBJ(i))
#define _CONTEXT(i) GDK_GL_CONTEXT(RVAL2GOBJ(i))
#define _CONFIG(i) GDK_GL_CONFIG(RVAL2GOBJ(i))

static VALUE
gtkglwidget_set_gl_capability(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GdkGLConfig* glconfig;
    GdkGLContext* share_list = NULL;
    gboolean direct = TRUE;
    int render_type = GDK_GL_RGBA_TYPE;

    if (argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 4)", argc);
    else if (argc < 1)
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1)", argc);

    switch (argc) {
        case 4:
            render_type = NUM2INT(argv[3]);
        case 3:
            direct = RVAL2CBOOL(argv[2]);
        case 2:
            share_list = _CONTEXT(argv[1]);
        default:
            glconfig = _CONFIG(argv[0]);
    }
    return CBOOL2RVAL(gtk_widget_set_gl_capability(_SELF(self),
                                                   glconfig,
                                                   share_list,
                                                   direct,
                                                   render_type));
}

static VALUE
gtkglwidget_is_gl_capable(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_widget_is_gl_capable(_SELF(self)));
}

static VALUE
gtkglwidget_get_gl_config(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_config(_SELF(self)));
}

static VALUE
gtkglwidget_create_gl_context(self, share_list, direct, render_type)
    VALUE self, share_list, direct, render_type;
{
    return GOBJ2RVAL(gtk_widget_create_gl_context(_SELF(self),
                                                  _CONTEXT(share_list),
                                                  RVAL2CBOOL(direct),
                                                  NUM2INT(render_type)));
}

static VALUE
gtkglwidget_get_gl_context(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_context(_SELF(self)));
}

static VALUE
gtkglwidget_get_gl_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_window(_SELF(self)));
}

static VALUE
gtkglwidget_get_gl_drawable(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_drawable(_SELF(self)));
}

void
Init_gtk_gl_widget(void)
{
    VALUE gtkGlWidget = rb_const_get(mGtk, rb_intern("Widget"));

    rb_define_method(gtkGlWidget, "set_gl_capability", gtkglwidget_set_gl_capability, -1);
    rb_define_method(gtkGlWidget, "gl_capable?", gtkglwidget_is_gl_capable, 0);
    rb_define_method(gtkGlWidget, "gl_config", gtkglwidget_get_gl_config, 0);
    rb_define_method(gtkGlWidget, "create_gl_context", gtkglwidget_create_gl_context, 3);
    rb_define_method(gtkGlWidget, "gl_context", gtkglwidget_get_gl_context, 0);
    rb_define_method(gtkGlWidget, "gl_window", gtkglwidget_get_gl_window, 0);
    rb_define_method(gtkGlWidget, "gl_drawable", gtkglwidget_get_gl_drawable, 0);
}
