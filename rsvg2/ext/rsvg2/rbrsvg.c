/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2005-2015  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Kouhei Sutou <kou@cozmixng.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rsvg2.h"

#define RG_TARGET_NAMESPACE mRSVG

#define RBRSVG_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBRSVG_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBRSVG_MICRO_VERSION RBGLIB_MICRO_VERSION

void Init_rsvg2(void);

static VALUE
rg_s_set_default_dpi(VALUE self, VALUE dpi)
{
#ifdef HAVE_RSVG_SET_DEFAULT_DPI
    rsvg_set_default_dpi(NUM2DBL(dpi));
#else
    rb_warning("rsvg_set_default_dpi isn't supported in your librsvg");
#endif
    return self;
}

static VALUE
rg_s_set_default_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
{
#ifdef HAVE_RSVG_SET_DEFAULT_DPI_X_Y
    rsvg_set_default_dpi_x_y(NUM2DBL(dpi_x), NUM2DBL(dpi_y));
#else
    rb_warning("rsvg_set_default_dpi_x_y isn't supported in your librsvg");
#endif
    return self;
}

/* Convenience API */
static VALUE
rg_s_pixbuf_from_file(G_GNUC_UNUSED VALUE self, VALUE file_name)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file(RVAL2CSTR(file_name), &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_s_pixbuf_from_file_at_zoom(G_GNUC_UNUSED VALUE self, VALUE file_name,
                                 VALUE x_zoom, VALUE y_zoom)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom(RVAL2CSTR(file_name),
                                           NUM2DBL(x_zoom),
                                           NUM2DBL(y_zoom),
                                           &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_s_pixbuf_from_file_at_size(G_GNUC_UNUSED VALUE self, VALUE file_name,
                                 VALUE width, VALUE height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_size(RVAL2CSTR(file_name),
                                           NUM2INT(width),
                                           NUM2INT(height),
                                           &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_s_pixbuf_from_file_at_max_size(G_GNUC_UNUSED VALUE self, VALUE file_name,
                                     VALUE max_width, VALUE max_height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_max_size(RVAL2CSTR(file_name),
                                               NUM2INT(max_width),
                                               NUM2INT(max_height),
                                               &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_s_pixbuf_from_file_at_zoom_with_max(G_GNUC_UNUSED VALUE self,
                                          VALUE file_name,
                                          VALUE x_zoom,
                                          VALUE y_zoom,
                                          VALUE max_width,
                                          VALUE max_height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom_with_max(RVAL2CSTR(file_name),
                                                    NUM2DBL(x_zoom),
                                                    NUM2DBL(y_zoom),
                                                    NUM2INT(max_width),
                                                    NUM2INT(max_height),
                                                    &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_s_cairo_available_p(G_GNUC_UNUSED VALUE self)
{
#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_rsvg2(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module("RSVG");

#if !LIBRSVG_CHECK_VERSION(2, 35, 0)
    rsvg_init();
    atexit(rsvg_term);
#endif

    G_DEF_ERROR(RSVG_ERROR, "Error", RG_TARGET_NAMESPACE, rb_eRuntimeError, RSVG_TYPE_ERROR);

    rb_define_const(RG_TARGET_NAMESPACE, "BINDING_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(RBRSVG_MAJOR_VERSION),
                                INT2FIX(RBRSVG_MINOR_VERSION),
                                INT2FIX(RBRSVG_MICRO_VERSION)));

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(LIBRSVG_MAJOR_VERSION),
                                INT2FIX(LIBRSVG_MINOR_VERSION),
                                INT2FIX(LIBRSVG_MICRO_VERSION)));

    RG_DEF_SMETHOD(set_default_dpi, 1);
    RG_DEF_SMETHOD(set_default_dpi_x_y, 2);

    /* Convenience API */
    RG_DEF_SMETHOD(pixbuf_from_file, 1);
    RG_DEF_SMETHOD(pixbuf_from_file_at_zoom, 3);
    RG_DEF_SMETHOD(pixbuf_from_file_at_size, 3);
    RG_DEF_SMETHOD(pixbuf_from_file_at_max_size, 3);
    RG_DEF_SMETHOD(pixbuf_from_file_at_zoom_with_max, 5);

    RG_DEF_SMETHOD_P(cairo_available, 0);

    Init_rsvg_handle(RG_TARGET_NAMESPACE);
    Init_rsvg_dimensiondata(RG_TARGET_NAMESPACE);
}
