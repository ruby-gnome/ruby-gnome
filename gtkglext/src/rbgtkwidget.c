/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgtkwidget.c,v 1.4 2003/08/27 18:29:18 isambart Exp $ */
/* OpenGL extension to Gtk::Widget
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

#define _CONTEXT(i) GDK_GL_CONTEXT(RVAL2GOBJ(i))
#define _CONFIG(i)  GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _WIDGET(i)  GTK_WIDGET(RVAL2GOBJ(i))
#define _SELF(i)    _WIDGET(i)

static VALUE
widget_set_gl_capability(argc, argv, self)
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
            render_type = RVAL2GENUM(argv[3], GDK_TYPE_GL_RENDER_TYPE);
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
widget_is_gl_capable(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_widget_is_gl_capable(_SELF(self)));
}

static VALUE
widget_get_gl_config(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_config(_SELF(self)));
}

static VALUE
widget_create_gl_context(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GdkGLContext* share_list = NULL;
    gboolean direct = TRUE;
    int render_type = GDK_GL_RGBA_TYPE;

    if (argc > 3)
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 3)", argc);

    switch (argc) {
        case 3:
            render_type = RVAL2GENUM(argv[2], GDK_TYPE_GL_RENDER_TYPE);
        case 2:
            direct = RVAL2CBOOL(argv[1]);
        case 1:
            share_list = _CONTEXT(argv[0]);
        default:
            break;
    }
    return GOBJ2RVAL(gtk_widget_create_gl_context(_SELF(self),
                                                  share_list,
                                                  direct,
                                                  render_type));
}

static VALUE
widget_get_gl_context(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_context(_SELF(self)));
}

static VALUE
widget_get_gl_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_window(_SELF(self)));
}

static VALUE
widget_get_gl_drawable(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_gl_drawable(_SELF(self)));
}

void
Init_gtkglext_gtk_widget(void)
{
    /* Gtk::Widget */
    VALUE Widget = rb_const_get(mGtk, rb_intern("Widget"));

    rb_define_method(Widget, "set_gl_capability", widget_set_gl_capability,-1);
    rb_define_method(Widget, "gl_capable?",       widget_is_gl_capable,     0);
    rb_define_method(Widget, "gl_config",         widget_get_gl_config,     0);
    rb_define_method(Widget, "create_gl_context", widget_create_gl_context, 3);
    rb_define_method(Widget, "gl_context",        widget_get_gl_context,    0);
    rb_define_method(Widget, "gl_window",         widget_get_gl_window,     0);
    rb_define_method(Widget, "gl_drawable",       widget_get_gl_drawable,   0);
}
