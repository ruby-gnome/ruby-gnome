/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgtkglext.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
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

VALUE mGdk, mGdkGl;
VALUE mGtk, mGtkGl;

EXTERN VALUE rb_progname, rb_argv;

static VALUE
gtk_gdk_gl_m_init(argc, argv, self, init_gtk)
    int argc;
    VALUE *argv;
    VALUE self;
    gboolean init_gtk;
{
    gint i, gargc;
    VALUE argary;
    gchar** gargv;

    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        gargc = RARRAY(rb_argv)->len;
        argary = rb_argv;
    } else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY(argary)->len;
    }

    gargv = ALLOCA_N(char*,gargc + 1);
    gargv[0] = RVAL2CSTR(rb_progname);

    for (i = 0; i < gargc; i++) {
        if (TYPE(RARRAY(argary)->ptr[i]) == T_STRING) {
            gargv[i+1] = RVAL2CSTR(RARRAY(argary)->ptr[i]);
        }
        else {
            gargv[i+1] = "";
        }
    }
    gargc++;

    {
        gboolean is_initialized;

        if (init_gtk)
            is_initialized = gtk_gl_init_check(&gargc, &gargv);
        else
            is_initialized = gdk_gl_init_check(&gargc, &gargv);
        
        if (!is_initialized)
            rb_raise(rb_eRuntimeError, "failed to initialize gtkglext");
    }

    return self;
}

static VALUE
gtk_gl_m_init(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    return gtk_gdk_gl_m_init(argc, argv, self, TRUE);
}

static VALUE
gdk_gl_m_init(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    return gtk_gdk_gl_m_init(argc, argv, self, FALSE);
}

void
Init_gtkglext(void)
{
    mGtk = rb_define_module("Gtk");
    mGtkGl = rb_define_module_under(mGtk, "GL");
    mGdk = rb_define_module("Gdk");
    mGdkGl = rb_define_module_under(mGdk, "GL");

    G_DEF_CONSTANTS(mGdkGl, GDK_TYPE_GL_CONFIG_MODE,   "GDK_GL_");
    G_DEF_CONSTANTS(mGdkGl, GDK_TYPE_GL_CONFIG_ATTRIB, "GDK_GL_");
    G_DEF_CONSTANTS(mGdkGl, GDK_TYPE_GL_RENDER_TYPE,   "GDK_GL_");

    rb_define_module_function(mGtkGl, "init", gtk_gl_m_init, -1);
    rb_define_module_function(mGdkGl, "init", gdk_gl_m_init, -1);

    Init_gdk_gl_query();
    Init_gdk_gl_config();
    Init_gdk_gl_drawable();
    Init_gdk_gl_context();
    Init_gtk_gl_widget();
    Init_gdk_gl_pixmap();
    Init_gdk_gl_shapes();
    Init_gdk_gl_window();
    Init_gdk_gl_font();
}
