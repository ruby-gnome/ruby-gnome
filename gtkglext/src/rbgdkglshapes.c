/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglshapes.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
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

static VALUE
gdk_gl_m_draw_cube(self, solid, size)
    VALUE self, solid, size;
{
    gdk_gl_draw_cube(RVAL2CBOOL(solid), NUM2DBL(size));
    return self;
}

static VALUE
gdk_gl_m_draw_sphere(self, solid, radius, slices, stacks)
    VALUE self, solid, radius, slices, stacks;
{
    gdk_gl_draw_sphere(RVAL2CBOOL(solid),
                       NUM2DBL(radius),
                       NUM2INT(slices),
                       NUM2INT(stacks));
    return self;
}

static VALUE
gdk_gl_m_draw_cone(self, solid, base, height, slices, stacks)
    VALUE self, solid, base, height, slices, stacks;
{
    gdk_gl_draw_cone(RVAL2CBOOL(solid),
                     NUM2DBL(base),
                     NUM2DBL(height),
                     NUM2INT(slices),
                     NUM2INT(stacks));
    return self;
}

static VALUE
gdk_gl_m_draw_torus(self, solid, inner_radius, outer_radius, nsides, rings)
    VALUE self, solid, inner_radius, outer_radius, nsides, rings;
{
    gdk_gl_draw_torus(RVAL2CBOOL(solid),
                      NUM2DBL(inner_radius),
                      NUM2DBL(outer_radius),
                      NUM2INT(nsides),
                      NUM2INT(rings));
    return self;
}

static VALUE
gdk_gl_m_draw_tetrahedron(self, solid)
    VALUE self, solid;
{
    gdk_gl_draw_tetrahedron(RVAL2CBOOL(solid));
    return self;
}

static VALUE
gdk_gl_m_draw_octahedron(self, solid)
    VALUE self, solid;
{
    gdk_gl_draw_octahedron(RVAL2CBOOL(solid));
    return self;
}

static VALUE
gdk_gl_m_draw_dodecahedron(self, solid)
    VALUE self, solid;
{
    gdk_gl_draw_dodecahedron(RVAL2CBOOL(solid));
    return self;
}

static VALUE
gdk_gl_m_draw_icosahedron(self, solid)
    VALUE self, solid;
{
    gdk_gl_draw_icosahedron(RVAL2CBOOL(solid));
    return self;
}

static VALUE
gdk_gl_m_draw_teapot(self, solid, scale)
    VALUE self, solid, scale;
{
    gdk_gl_draw_teapot(RVAL2CBOOL(solid), NUM2DBL(scale));
    return self;
}

void
Init_gdk_gl_shapes(void)
{
    rb_define_module_function(mGdkGl, "draw_cube"        , gdk_gl_m_draw_cube        , 2);
    rb_define_module_function(mGdkGl, "draw_sphere"      , gdk_gl_m_draw_sphere      , 4);
    rb_define_module_function(mGdkGl, "draw_cone"        , gdk_gl_m_draw_cone        , 5);
    rb_define_module_function(mGdkGl, "draw_torus"       , gdk_gl_m_draw_torus       , 5);
    rb_define_module_function(mGdkGl, "draw_tetrahedron" , gdk_gl_m_draw_tetrahedron , 1);
    rb_define_module_function(mGdkGl, "draw_octahedron"  , gdk_gl_m_draw_octahedron  , 1);
    rb_define_module_function(mGdkGl, "draw_dodecahedron", gdk_gl_m_draw_dodecahedron, 1);
    rb_define_module_function(mGdkGl, "draw_icosahedron" , gdk_gl_m_draw_icosahedron , 1);
    rb_define_module_function(mGdkGl, "draw_teapot"      , gdk_gl_m_draw_teapot      , 2);
}
