/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglconfig.c,v 1.2 2003/08/20 14:43:19 isambart Exp $ */
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

#define _SELF(i) GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _SCREEN(i) GDK_SCREEN(RVAL2GOBJ(i))

static int*
convert_to_int_array(VALUE rb_array)
{
    int* array;
    int i;

    array = g_new(int, RARRAY(rb_array)->len+1);
    for (i=0; i<RARRAY(rb_array)->len; ++i) {
        array[i] = NUM2INT(RARRAY(rb_array)->ptr[i]);
    }
    array[RARRAY(rb_array)->len] = GDK_GL_ATTRIB_LIST_NONE;
    return array;
}

static VALUE
gdkglconfig_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
#ifndef GDK_MULTIHEAD_SAFE
    if (argc == 1)
    {
        if (TYPE(argv[0]) == T_ARRAY) {
            int *attrib_list;
            attrib_list = convert_to_int_array(argv[0]);
            G_INITIALIZE(self, gdk_gl_config_new(attrib_list));
            g_free(attrib_list);
        } else {
            G_INITIALIZE(self, gdk_gl_config_new_by_mode(NUM2INT(argv[0])));
        }
    }
    else
#endif /* !defined GDK_MULTIHEAD_SAFE */
#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
    if (argc == 2) {
        if (TYPE(argv[1]) == T_ARRAY) {
            int *attrib_list;
            attrib_list = convert_to_int_array(argv[1]);
            G_INITIALIZE(self, gdk_gl_config_new_for_screen(_SCREEN(argv[0]), attrib_list));
            g_free(attrib_list);
        } else {
            G_INITIALIZE(self, gdk_gl_config_new_by_mode_for_screen(_SCREEN(argv[0]), NUM2INT(argv[1])));
        }
    }
    else
#endif /* defined GDKGLEXT_MULTIHEAD_SUPPORT */
        rb_raise(rb_eArgError, "wrong number of arguments");

    return Qnil;
}

static VALUE
gdkglconfig_get_attrib(self, attribute)
    VALUE self, attribute;
{
    int value;
    gboolean ok;

    ok = gdk_gl_config_get_attrib(_SELF(self), NUM2INT(attribute), &value);
    if (ok)
        return INT2NUM(value);
    else
        return Qnil;
}

static VALUE
gdkglconfig_get_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_screen(_SELF(self)));
}

static VALUE
gdkglconfig_get_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_colormap(_SELF(self)));
}

static VALUE
gdkglconfig_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_visual(_SELF(self)));
}

static VALUE
gdkglconfig_get_depth(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_depth(_SELF(self)));
}

static VALUE
gdkglconfig_get_layer_plane(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_layer_plane(_SELF(self)));
}

static VALUE
gdkglconfig_get_n_aux_buffers(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_n_aux_buffers(_SELF(self)));
}

static VALUE
gdkglconfig_get_n_sample_buffers(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_n_sample_buffers(_SELF(self)));
}

static VALUE
gdkglconfig_is_rgba(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_rgba(_SELF(self)));
}

static VALUE
gdkglconfig_is_double_buffered(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_double_buffered(_SELF(self)));
}

static VALUE
gdkglconfig_is_stereo(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_stereo(_SELF(self)));
}

static VALUE
gdkglconfig_has_alpha(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_alpha(_SELF(self)));
}

static VALUE
gdkglconfig_has_depth_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_depth_buffer(_SELF(self)));
}

static VALUE
gdkglconfig_has_stencil_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_stencil_buffer(_SELF(self)));
}

static VALUE
gdkglconfig_has_accum_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_accum_buffer(_SELF(self)));
}

void
Init_gdk_gl_config(void)
{
    VALUE gdkGlConfig = G_DEF_CLASS(GDK_TYPE_GL_CONFIG, "Config", mGdkGl);

    G_DEF_CONSTANTS(gdkGlConfig, GDK_TYPE_GL_CONFIG_MODE,   "GDK_GL_");
    G_DEF_CONSTANTS(gdkGlConfig, GDK_TYPE_GL_CONFIG_ATTRIB, "GDK_GL_");

    rb_define_method(gdkGlConfig, "initialize", gdkglconfig_initialize, -1);

    rb_define_method(gdkGlConfig, "get_attrib", gdkglconfig_get_attrib, 1);

    rb_define_method(gdkGlConfig, "screen", gdkglconfig_get_screen, 0);
    rb_define_method(gdkGlConfig, "colormap", gdkglconfig_get_colormap, 0);
    rb_define_method(gdkGlConfig, "visual", gdkglconfig_get_visual, 0);
    rb_define_method(gdkGlConfig, "depth", gdkglconfig_get_depth, 0);
    rb_define_method(gdkGlConfig, "layer_plane", gdkglconfig_get_layer_plane, 0);
    rb_define_method(gdkGlConfig, "colormap", gdkglconfig_get_colormap, 0);
    rb_define_method(gdkGlConfig, "n_aux_buffers", gdkglconfig_get_n_aux_buffers, 0);
    rb_define_method(gdkGlConfig, "n_sample_buffers", gdkglconfig_get_n_sample_buffers, 0);

    rb_define_method(gdkGlConfig, "rgba?", gdkglconfig_is_rgba, 0);
    rb_define_method(gdkGlConfig, "double_buffered?", gdkglconfig_is_double_buffered, 0);
    rb_define_method(gdkGlConfig, "stereo?", gdkglconfig_is_stereo, 0);
    rb_define_method(gdkGlConfig, "has_alpha?", gdkglconfig_has_alpha, 0);
    rb_define_method(gdkGlConfig, "has_depth_buffer?", gdkglconfig_has_depth_buffer, 0);
    rb_define_method(gdkGlConfig, "has_stencil_buffer?", gdkglconfig_has_stencil_buffer, 0);
    rb_define_method(gdkGlConfig, "has_accum_buffer?", gdkglconfig_has_accum_buffer, 0);
}
