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

#define RG_TARGET_NAMESPACE cHandle
#ifdef RSVG_TYPE_HANDLE
#  define _SELF(self) (RSVG_HANDLE(RVAL2GOBJ(self)))
#else
#  define _SELF(self) ((RsvgHandle *)DATA_PTR(self))
#endif

static VALUE RG_TARGET_NAMESPACE;

static ID id_call;
static ID id_callback;
static ID id_closed;

static void
exec_callback(gint *width, gint *height, gpointer self)
{
    VALUE result;
    result = rb_funcall(rb_ivar_get((VALUE)self, id_callback),
                        id_call, 2, INT2NUM(*width), INT2NUM(*height));
    if (T_ARRAY == TYPE(result)) {
        VALUE w, h;
        w = rb_ary_entry(result, 0);
        h = rb_ary_entry(result, 1);
        if (!NIL_P(w)) {
            *width = NUM2INT(w);
        }
        if (!NIL_P(h)) {
            *height = NUM2INT(h);
        }
    }
}

static VALUE
rg_set_dpi(VALUE self, VALUE dpi)
{
#ifdef HAVE_RSVG_HANDLE_SET_DPI
    rsvg_handle_set_dpi(_SELF(self), NUM2DBL(dpi));
#else
    rb_warning("rsvg_handle_set_dpi isn't supported in your librsvg");
#endif
    return self;
}

static VALUE
rg_set_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
{
#ifdef HAVE_RSVG_HANDLE_SET_DPI_X_Y
    rsvg_handle_set_dpi_x_y(_SELF(self), NUM2DBL(dpi_x), NUM2DBL(dpi_y));
#else
    rb_warning("rsvg_handle_set_dpi_x_y isn't supported in your librsvg");
#endif
    return self;
}

#ifndef RSVG_TYPE_HANDLE
static void
rb_rsvg_handle_free(RsvgHandle *handle)
{
    if (handle) {
        rsvg_handle_free(handle);
    }
}

static VALUE
rb_rsvg_handle_alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, 0, rb_rsvg_handle_free, 0);
}
#endif

#if LIBRSVG_CHECK_VERSION(2, 14, 0)
static VALUE
rg_s_new_from_data(G_GNUC_UNUSED VALUE self, VALUE data)
{
    GError *error = NULL;
    RsvgHandle *handle;

    handle = rsvg_handle_new_from_data((const guint8 *)RVAL2CSTR(data),
                                       RSTRING_LEN(data), &error);

    if (error)
        RAISE_GERROR(error);

    return GOBJ2RVAL_UNREF(handle);
}

static VALUE
rg_s_new_from_file(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE rb_file_path, rb_options, rb_flags;
    GError *error = NULL;
    RsvgHandle *handle;

    rb_scan_args(argc, argv, "11", &rb_file_path, &rb_options);
    rbg_scan_options(rb_options,
                     "flags", &rb_flags,
                     NULL);

#if LIBRSVG_CHECK_VERSION(2, 40, 3)
    {
        GFile *file;
        GCancellable *cancellable = NULL;
        RsvgHandleFlags flags = RSVG_HANDLE_FLAGS_NONE;

        if (!NIL_P(rb_flags)) {
            flags = RVAL2GFLAGS(rb_flags, RSVG_TYPE_HANDLE_FLAGS);
        }

        file = g_file_new_for_path((const char *)RVAL2CSTR(rb_file_path));
        handle = rsvg_handle_new_from_gfile_sync(file, flags,
                                                 cancellable,
                                                 &error);
        g_object_unref(file);
    }
#else
    if (!NIL_P(rb_flags)) {
        rb_raise(rb_eArgError,
                 "librsvg 2.40.3 or later is required for :flags: <%d.%d.%d>",
                 LIBRSVG_MAJOR_VERSION,
                 LIBRSVG_MINOR_VERSION,
                 LIBRSVG_MICRO_VERSION);
    }
    handle = rsvg_handle_new_from_file((const gchar *)RVAL2CSTR(rb_file_path),
                                       &error);
#endif

    if (error)
        RAISE_GERROR(error);

    return GOBJ2RVAL_UNREF(handle);
}
#endif

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    RsvgHandle *handle;
    VALUE gz;
    rb_scan_args(argc, argv, "01", &gz);

#if LIBRSVG_CHECK_VERSION(2, 11, 0)
    handle = rsvg_handle_new();
#else
    if (RVAL2CBOOL(gz)) {
#  ifdef HAVE_LIBRSVG_RSVG_GZ_H
        handle = rsvg_handle_new_gz();
#  else
        rb_warning("gz handling is not supported in your librsvg");
        handle = rsvg_handle_new();
#  endif
    } else {
        handle = rsvg_handle_new();
    }
#endif

#ifdef RSVG_TYPE_HANDLE
    G_INITIALIZE(self, handle);
#else
    DATA_PTR(self) = handle;
#endif

    rb_ivar_set(self, id_closed, Qfalse);
    return Qnil;
}

static VALUE
rg_set_size_callback(VALUE self)
{
    rb_ivar_set(self, id_callback, rb_block_proc());
    rsvg_handle_set_size_callback(_SELF(self), exec_callback,
                                  (gpointer)self, NULL);
    return self;
}

static VALUE
rg_write(VALUE self, VALUE buf)
{
    gboolean result;
    GError *error = NULL;

    result = rsvg_handle_write(_SELF(self), (const guchar*)RVAL2CSTR(buf),
                               RSTRING_LEN(buf), &error);

    if (!result) RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rg_close(VALUE self)
{
    gboolean result;
    GError *error = NULL;

    if (RVAL2CBOOL(rb_ivar_get(self, id_closed))) {
        return Qnil;
    }

    result = rsvg_handle_close(_SELF(self), &error);

    if (result) {
        rb_ivar_set(self, id_closed, Qtrue);
    } else {
        RAISE_GERROR(error);
    }

    return CBOOL2RVAL(result);
}

static VALUE
rg_closed_p(VALUE self)
{
    return rb_ivar_get(self, id_closed);
}

static VALUE
rg_pixbuf(int argc, VALUE *argv, VALUE self)
{
    VALUE id;
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf = NULL;

    rb_scan_args(argc, argv, "01", &id);
    if (NIL_P(id)) {
        pixbuf = rsvg_handle_get_pixbuf(_SELF(self));
    } else {
#ifdef HAVE_RSVG_HANDLE_GET_PIXBUF_SUB
        pixbuf = rsvg_handle_get_pixbuf_sub(_SELF(self),
                                            (const char *)RVAL2CSTR(id));
#else
        rb_warning("rsvg_handle_get_pixbuf_sub isn't "
                   "supported in your librsvg");
#endif
    }

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);
    return rb_pixbuf;
}

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
static VALUE
rg_dimensions(VALUE self)
{
    RsvgDimensionData dim;
    VALUE args[4];
    VALUE cDimensionData = rb_const_get(rb_const_get(rb_cObject,
                                rb_intern("RSVG")), rb_intern("DimensionData"));

    rsvg_handle_get_dimensions(_SELF(self), &dim);
    args[0] = INT2NUM(dim.width);
    args[1] = INT2NUM(dim.height);
    args[2] = rb_float_new(dim.em);
    args[3] = rb_float_new(dim.ex);
    return rb_class_new_instance(sizeof(args) / sizeof(VALUE),
                                 args, cDimensionData);
}
#endif

#if !LIBRSVG_CHECK_VERSION(2, 36, 0)
/* Accessibility API */
static VALUE
rg_title(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_title(_SELF(self)));
}

static VALUE
rg_desc(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_desc(_SELF(self)));
}

#  ifdef HAVE_RSVG_HANDLE_GET_METADATA
static VALUE
rg_metadata(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_metadata(_SELF(self)));
}
#  endif
#endif

#if !LIBRSVG_CHECK_VERSION(2, 11, 0)
/* Extended Convenience API */
static VALUE
rg_pixbuf_from_file_at_size(VALUE self, VALUE file_name,
                                    VALUE width, VALUE height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_size_ex(_SELF(self),
                                              RVAL2CSTR(file_name),
                                              NUM2INT(width),
                                              NUM2INT(height),
                                              &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_pixbuf_from_file(VALUE self, VALUE file_name)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_ex(_SELF(self),
                                      RVAL2CSTR(file_name),
                                      &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_pixbuf_from_file_at_zoom(VALUE self, VALUE file_name,
                                    VALUE x_zoom, VALUE y_zoom)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom_ex(_SELF(self),
                                              RVAL2CSTR(file_name),
                                              NUM2DBL(x_zoom),
                                              NUM2DBL(y_zoom),
                                              &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_pixbuf_from_file_at_max_size(VALUE self, VALUE file_name,
                                        VALUE max_width, VALUE max_height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_max_size_ex(_SELF(self),
                                                  RVAL2CSTR(file_name),
                                                  NUM2INT(max_width),
                                                  NUM2INT(max_height),
                                                  &error);

    if (error) RAISE_GERROR(error);

    rb_pixbuf = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return rb_pixbuf;
}

static VALUE
rg_pixbuf_from_file_at_zoom_with_max(VALUE self,
                                             VALUE file_name,
                                             VALUE x_zoom,
                                             VALUE y_zoom,
                                             VALUE max_width,
                                             VALUE max_height)
{
    VALUE rb_pixbuf;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom_with_max_ex(_SELF(self),
                                                       RVAL2CSTR(file_name),
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
#endif

#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
static VALUE
rg_render_cairo(int argc, VALUE *argv, VALUE self)
{
    VALUE cr, id;
    rb_scan_args(argc, argv, "11", &cr, &id);

    if (NIL_P(id)) {
        rsvg_handle_render_cairo( _SELF(self), RVAL2CRCONTEXT(cr));
    } else {
        rsvg_handle_render_cairo_sub( _SELF(self), RVAL2CRCONTEXT(cr),
                                      (const char *)RVAL2CSTR(id));
    }

    return Qnil;
}
#endif

void
Init_rsvg_handle(VALUE mRSVG)
{
    id_call = rb_intern("call");
    id_callback = rb_intern("callback");
    id_closed = rb_intern("closed");

#ifdef RSVG_TYPE_HANDLE
    RG_TARGET_NAMESPACE = G_DEF_CLASS(RSVG_TYPE_HANDLE, "Handle", mRSVG);
#else
    RG_TARGET_NAMESPACE = rb_define_class_under(mRSVG, "Handle", rb_cObject);
    rb_define_alloc_func(RG_TARGET_NAMESPACE, rb_rsvg_handle_alloc);
#endif

#if LIBRSVG_CHECK_VERSION(2, 14, 0)
    RG_DEF_SMETHOD(new_from_data, 1);
    RG_DEF_SMETHOD(new_from_file, -1);
#endif

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_size_callback, 0);
    RG_DEF_METHOD(set_dpi, 1);
    RG_DEF_METHOD(set_dpi_x_y, 2);
    RG_DEF_METHOD(write, 1);
    RG_DEF_METHOD(close, 0);
    RG_DEF_METHOD_P(closed, 0);
    RG_DEF_METHOD(pixbuf, -1);

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
    RG_DEF_METHOD(dimensions, 0);
#endif

#if !LIBRSVG_CHECK_VERSION(2, 36, 0)
    /* Accessibility API */
    RG_DEF_METHOD(title, 0);
    RG_DEF_METHOD(desc, 0);
#  ifdef HAVE_RSVG_HANDLE_GET_METADATA
    RG_DEF_METHOD(metadata, 0);
#  endif
#endif

#if !LIBRSVG_CHECK_VERSION(2, 11, 0)
    /* Extended Convenience API */
    RG_DEF_METHOD(pixbuf_from_file_at_size, 3);
    RG_DEF_METHOD(pixbuf_from_file, 1);
    RG_DEF_METHOD(pixbuf_from_file_at_zoom, 3);
    RG_DEF_METHOD(pixbuf_from_file_at_max_size, 3);
    RG_DEF_METHOD(pixbuf_from_file_at_zoom_with_max, 5);
#endif

#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
    RG_DEF_METHOD(render_cairo, -1);
#endif

#if LIBRSVG_CHECK_VERSION(2, 40, 3)
    /* RsvgHandleFlags */
    G_DEF_CLASS(RSVG_TYPE_HANDLE_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, RSVG_TYPE_HANDLE_FLAGS, "RSVG_HANDLE_");
#endif
}
