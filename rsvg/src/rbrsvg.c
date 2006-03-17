/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbrsvg.c -

  $Author: ktou $
  $Date: 2006/03/17 13:37:12 $

  Copyright (C) 2005-2006 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#ifdef HAVE_LIBRSVG_RSVG_GZ_H
#  include <librsvg/rsvg-gz.h>
#else
#  include <librsvg/rsvg.h>
#endif

#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
#  include <rb_cairo.h>
#  include <librsvg/rsvg-cairo.h>
#endif

#ifdef HAVE_LIBRSVG_LIBRSVG_ENUM_TYPES_H
#  include <librsvg/librsvg-enum-types.h>
#else
#  include "librsvg-enum-types.h"
#endif

#include <librsvg/librsvg-features.h>

#define RBRSVG_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBRSVG_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBRSVG_MICRO_VERSION RBGLIB_MICRO_VERSION

#define LIBRSVG_CHECK_VERSION(major, minor, micro)                            \
    (LIBRSVG_MAJOR_VERSION > (major) ||                                       \
     (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION > (minor)) || \
     (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION == (minor) && \
      LIBRSVG_MICRO_VERSION >= (micro)))

#ifdef RSVG_TYPE_HANDLE
#  define _SELF(self) (RSVG_HANDLE(RVAL2GOBJ(self)))
#else
#  define _SELF(self) ((RsvgHandle *)DATA_PTR(self))
#endif
#define RVAL2DIM(obj) ((RsvgDimensionData *)DATA_PTR(obj))

static VALUE cHandle;

static ID id_call;
static ID id_callback;
static ID id_closed;
static ID id_to_s;

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

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
static VALUE
to_s(VALUE obj)
{
    return rb_funcall(obj, id_to_s, 0);
}

static VALUE cDim;

static void
rb_rsvg_dim_free(RsvgDimensionData *dimp)
{
    if (dimp) {
	free(dimp);
    }
}

static VALUE
rb_rsvg_dim_alloc(VALUE klass)
{
    RsvgDimensionData *dimp;
    return Data_Make_Struct(klass, RsvgDimensionData, 0,
                            rb_rsvg_dim_free, dimp);
}

static VALUE
rb_rsvg_dim_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE width, height, em, ex;
    RsvgDimensionData *dimp;

    dimp = RVAL2DIM(self);
    rb_scan_args(argc, argv, "04", &width, &height, &em, &ex);

    if (!NIL_P(width))
        dimp->width = NUM2INT(width);
    if (!NIL_P(height))
        dimp->height = NUM2INT(height);
    if (!NIL_P(em))
        dimp->em = NUM2DBL(em);
    if (!NIL_P(ex))
        dimp->ex = NUM2DBL(ex);

    return Qnil;
}

static VALUE
rb_rsvg_dim_get_width(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->width);
}

static VALUE
rb_rsvg_dim_set_width(VALUE self, VALUE width)
{
    RVAL2DIM(self)->width = NUM2INT(width);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_height(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->height);
}

static VALUE
rb_rsvg_dim_set_height(VALUE self, VALUE height)
{
    RVAL2DIM(self)->height = NUM2INT(height);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_em(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->em);
}

static VALUE
rb_rsvg_dim_set_em(VALUE self, VALUE em)
{
    RVAL2DIM(self)->em = NUM2DBL(em);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_ex(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->ex);
}

static VALUE
rb_rsvg_dim_set_ex(VALUE self, VALUE ex)
{
    RVAL2DIM(self)->ex = NUM2DBL(ex);
    return Qnil;
}

static VALUE
rb_rsvg_dim_to_a(VALUE self)
{
    return rb_ary_new3(4,
                       rb_rsvg_dim_get_width(self),
                       rb_rsvg_dim_get_height(self),
                       rb_rsvg_dim_get_em(self),
                       rb_rsvg_dim_get_ex(self));
}

static VALUE
rb_rsvg_dim_to_s(VALUE self)
{
    VALUE ret;

    ret = rb_str_new2("#<");
    rb_str_cat2(ret, rb_obj_classname(self));
    rb_str_cat2(ret, ":");
    rb_str_concat(ret, rb_funcall(INT2NUM(self), id_to_s, 0));
    rb_str_cat2(ret, " ");

    rb_str_cat2(ret, "width=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_width(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "height=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_height(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "em=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_em(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "ex=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_ex(self)));
    rb_str_cat2(ret, ">");

    return ret;
}
#endif

#if LIBRSVG_CHECK_VERSION(2, 9, 0)
static VALUE
rb_rsvg_init(VALUE klass)
{
    rsvg_init();
    return Qnil;
}

static VALUE
rb_rsvg_term(VALUE klass)
{
    rsvg_term();
    return Qnil;
}
#endif

static VALUE
rb_rsvg_set_default_dpi(VALUE self, VALUE dpi)
{
#ifdef HAVE_RSVG_SET_DEFAULT_DPI
    rsvg_set_default_dpi(NUM2DBL(dpi));
#else
    rb_warning("rsvg_set_default_dpi isn't supported in your librsvg");
#endif
    return self;
}

static VALUE
rb_rsvg_set_default_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
{
#ifdef HAVE_RSVG_SET_DEFAULT_DPI_X_Y
    rsvg_set_default_dpi_x_y(NUM2DBL(dpi_x), NUM2DBL(dpi_y));
#else
    rb_warning("rsvg_set_default_dpi_x_y isn't supported in your librsvg");
#endif
    return self;
}

static VALUE
rb_rsvg_handle_set_dpi(VALUE self, VALUE dpi)
{
#ifdef HAVE_RSVG_HANDLE_SET_DPI
    rsvg_handle_set_dpi(_SELF(self), NUM2DBL(dpi));
#else
    rb_warning("rsvg_handle_set_dpi isn't supported in your librsvg");
#endif
    return self;
}

static VALUE
rb_rsvg_handle_set_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
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
rb_rsvg_handle_new_from_data(VALUE self, VALUE data)
{
    GError *error = NULL;
    RsvgHandle *handle;

    handle = rsvg_handle_new_from_data((const guint8 *)RVAL2CSTR(data),
                                       RSTRING(data)->len,
                                       &error);

    if (error)
        RAISE_GERROR(error);

    return GOBJ2RVAL(handle);
}

static VALUE
rb_rsvg_handle_new_from_file(VALUE self, VALUE file)
{
    GError *error = NULL;
    RsvgHandle *handle;

    handle = rsvg_handle_new_from_file((const gchar *)RVAL2CSTR(file),
                                       &error);

    if (error)
        RAISE_GERROR(error);

    return GOBJ2RVAL(handle);
}
#endif

static VALUE
rb_rsvg_handle_initialize(int argc, VALUE *argv, VALUE self)
{
    RsvgHandle *handle;
    VALUE gz;
    rb_scan_args(argc, argv, "01", &gz);

#if LIBRSVG_CHECK_VERSION(2, 11, 0)
    handle = rsvg_handle_new();
#else
    if (RTEST(gz)) {
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

    DATA_PTR(self) = handle;

    rb_ivar_set(self, id_closed, Qfalse);
    return Qnil;
}

static VALUE
rb_rsvg_handle_set_size_callback(VALUE self)
{
    rb_ivar_set(self, id_callback, rb_block_proc());
    rsvg_handle_set_size_callback(_SELF(self), exec_callback,
                                  (gpointer)self, NULL);
    return self;
}

static VALUE
rb_rsvg_handle_write(VALUE self, VALUE buf)
{
    gboolean result;
    GError *error = NULL;

    result = rsvg_handle_write(_SELF(self), (const guchar*)RVAL2CSTR(buf),
                               RSTRING(buf)->len, &error);

    if (!result) RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rb_rsvg_handle_close(VALUE self)
{
    gboolean result;
    GError *error = NULL;

    if (RTEST(rb_ivar_get(self, id_closed))) {
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
rb_rsvg_handle_closed(VALUE self)
{
    return rb_ivar_get(self, id_closed);
}

static VALUE
rb_rsvg_handle_get_pixbuf(int argc, VALUE *argv, VALUE self)
{
    VALUE id;
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

    return GOBJ2RVAL(pixbuf);
}

#if LIBRSVG_CHECK_VERSION(2, 9, 0)
static VALUE
rb_rsvg_handle_get_base_uri(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_base_uri(_SELF(self)));
}

static VALUE
rb_rsvg_handle_set_base_uri(VALUE self, VALUE base_uri)
{
    rsvg_handle_set_base_uri(_SELF(self), RVAL2CSTR(base_uri));
    return self;
}
#endif

/* Convenience API */
static VALUE
rb_rsvg_pixbuf_from_file(VALUE self, VALUE file_name)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file(RVAL2CSTR(file_name), &error);

    if (error) RAISE_GERROR(error);
  
    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_zoom(VALUE self, VALUE file_name,
                                 VALUE x_zoom, VALUE y_zoom)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom(RVAL2CSTR(file_name),
                                           NUM2DBL(x_zoom),
                                           NUM2DBL(y_zoom),
                                           &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_size(VALUE self, VALUE file_name,
                                 VALUE width, VALUE height)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_size(RVAL2CSTR(file_name),
                                           NUM2INT(width),
                                           NUM2INT(height),
                                           &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_max_size(VALUE self, VALUE file_name,
                                     VALUE max_width, VALUE max_height)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_max_size(RVAL2CSTR(file_name),
                                               NUM2INT(max_width),
                                               NUM2INT(max_height),
                                               &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_zoom_with_max(VALUE self,
                                          VALUE file_name,
                                          VALUE x_zoom,
                                          VALUE y_zoom,
                                          VALUE max_width,
                                          VALUE max_height)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom_with_max(RVAL2CSTR(file_name),
                                                    NUM2DBL(x_zoom),
                                                    NUM2DBL(y_zoom),
                                                    NUM2INT(max_width),
                                                    NUM2INT(max_height),
                                                    &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
static VALUE
rb_rsvg_handle_get_dim(VALUE self)
{
    RsvgDimensionData dim;
    VALUE args[4];

    rsvg_handle_get_dimensions(_SELF(self), &dim);
    args[0] = INT2NUM(dim.width);
    args[1] = INT2NUM(dim.height);
    args[2] = rb_float_new(dim.em);
    args[3] = rb_float_new(dim.ex);
    return rb_class_new_instance(sizeof(args) / sizeof(VALUE),
                                 args, cDim);
}
#endif

/* Accessibility API */
static VALUE
rb_rsvg_handle_get_title(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_title(_SELF(self)));
}

static VALUE
rb_rsvg_handle_get_desc(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_desc(_SELF(self)));
}

#ifdef HAVE_RSVG_HANDLE_GET_METADATA
static VALUE
rb_rsvg_handle_get_metadata(VALUE self)
{
    return CSTR2RVAL(rsvg_handle_get_metadata(_SELF(self)));
}
#endif


#if !LIBRSVG_CHECK_VERSION(2, 11, 0)
/* Extended Convenience API */
static VALUE
rb_rsvg_pixbuf_from_file_at_size_ex(VALUE self, VALUE file_name,
                                    VALUE width, VALUE height)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_size_ex(_SELF(self),
                                              RVAL2CSTR(file_name),
                                              NUM2INT(width),
                                              NUM2INT(height),
                                              &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_ex(VALUE self, VALUE file_name)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_ex(_SELF(self),
                                      RVAL2CSTR(file_name),
                                      &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_zoom_ex(VALUE self, VALUE file_name,
                                    VALUE x_zoom, VALUE y_zoom)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_zoom_ex(_SELF(self),
                                              RVAL2CSTR(file_name),
                                              NUM2DBL(x_zoom),
                                              NUM2DBL(y_zoom),
                                              &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_max_size_ex(VALUE self, VALUE file_name,
                                        VALUE max_width, VALUE max_height)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    pixbuf = rsvg_pixbuf_from_file_at_max_size_ex(_SELF(self),
                                                  RVAL2CSTR(file_name),
                                                  NUM2INT(max_width),
                                                  NUM2INT(max_height),
                                                  &error);

    if (error) RAISE_GERROR(error);

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rb_rsvg_pixbuf_from_file_at_zoom_with_max_ex(VALUE self,
                                             VALUE file_name,
                                             VALUE x_zoom,
                                             VALUE y_zoom,
                                             VALUE max_width,
                                             VALUE max_height)
{
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

    return GOBJ2RVAL(pixbuf);
}
#endif

static VALUE
rb_rsvg_cairo_available(VALUE self)
{
#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
    return Qtrue;
#else
    return Qfalse;
#endif
}

#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
static VALUE
rb_rsvg_handle_render_cairo(int argc, VALUE *argv, VALUE self)
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
Init_rsvg2(void)
{
    VALUE mRSVG = rb_define_module("RSVG");

#ifdef RSVG_TYPE_HANDLE
    cHandle = G_DEF_CLASS(RSVG_TYPE_HANDLE, "Handle", mRSVG);
#else
    cHandle = rb_define_class_under(mRSVG, "Handle", rb_cObject);
    rb_define_alloc_func(cHandle, rb_rsvg_handle_alloc);
#endif

    G_DEF_ERROR(RSVG_ERROR, "Error", mRSVG, rb_eRuntimeError, RSVG_TYPE_ERROR);

    id_call = rb_intern("call");
    id_callback = rb_intern("callback");
    id_closed = rb_intern("closed");
    id_to_s = rb_intern("to_s");

    rb_define_const(mRSVG, "BINDING_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(RBRSVG_MAJOR_VERSION),
                                INT2FIX(RBRSVG_MINOR_VERSION),
                                INT2FIX(RBRSVG_MICRO_VERSION)));

    rb_define_const(mRSVG, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(LIBRSVG_MAJOR_VERSION),
                                INT2FIX(LIBRSVG_MINOR_VERSION),
                                INT2FIX(LIBRSVG_MICRO_VERSION)));

    rb_define_const(mRSVG, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(librsvg_major_version),
                                INT2FIX(librsvg_minor_version),
                                INT2FIX(librsvg_micro_version)));

    rb_define_const(mRSVG, "MAJOR_VERSION", INT2FIX(librsvg_major_version));
    rb_define_const(mRSVG, "MINOR_VERSION", INT2FIX(librsvg_minor_version));
    rb_define_const(mRSVG, "MICRO_VERSION", INT2FIX(librsvg_micro_version));

#if LIBRSVG_CHECK_VERSION(2, 9, 0)
    rb_define_module_function(mRSVG, "init", rb_rsvg_init, 0);
    rb_define_module_function(mRSVG, "term", rb_rsvg_term, 0);
#endif

    rb_define_module_function(mRSVG, "set_default_dpi",
                              rb_rsvg_set_default_dpi, 1);
    rb_define_module_function(mRSVG, "set_default_dpi_x_y",
                              rb_rsvg_set_default_dpi_x_y, 2);

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
    cDim = rb_define_class_under(mRSVG, "DimensionData", rb_cObject);

    rb_define_alloc_func(cDim, rb_rsvg_dim_alloc);
    rb_define_method(cDim, "initialize", rb_rsvg_dim_initialize, -1);

    rb_define_method(cDim, "width", rb_rsvg_dim_get_width, 0);
    rb_define_method(cDim, "set_width", rb_rsvg_dim_set_width, 1);
    rb_define_method(cDim, "height", rb_rsvg_dim_get_height, 0);
    rb_define_method(cDim, "set_height", rb_rsvg_dim_set_height, 1);
    rb_define_method(cDim, "em", rb_rsvg_dim_get_em, 0);
    rb_define_method(cDim, "set_em", rb_rsvg_dim_set_em, 1);
    rb_define_method(cDim, "ex", rb_rsvg_dim_get_ex, 0);
    rb_define_method(cDim, "set_ex", rb_rsvg_dim_set_ex, 1);

    rb_define_method(cDim, "to_s", rb_rsvg_dim_to_s, 0);
    rb_define_method(cDim, "to_a", rb_rsvg_dim_to_a, 0);

    G_DEF_SETTERS(cDim);
#endif


#if LIBRSVG_CHECK_VERSION(2, 14, 0)
    rb_define_module_function(cHandle, "new_from_data",
                              rb_rsvg_handle_new_from_data, 1);
    rb_define_module_function(cHandle, "new_from_file",
                              rb_rsvg_handle_new_from_file, 1);
#endif

    rb_define_method(cHandle, "initialize", rb_rsvg_handle_initialize, -1);
    rb_define_method(cHandle, "set_size_callback",
                     rb_rsvg_handle_set_size_callback, 0);
    rb_define_method(cHandle, "set_dpi", rb_rsvg_handle_set_dpi, 1);
    rb_define_method(cHandle, "set_dpi_x_y", rb_rsvg_handle_set_dpi_x_y, 2);
    rb_define_method(cHandle, "write", rb_rsvg_handle_write, 1);
    rb_define_method(cHandle, "close", rb_rsvg_handle_close, 0);
    rb_define_method(cHandle, "closed?", rb_rsvg_handle_closed, 0);
    rb_define_method(cHandle, "pixbuf", rb_rsvg_handle_get_pixbuf, -1);

#if LIBRSVG_CHECK_VERSION(2, 9, 0)
    rb_define_method(cHandle, "base_uri", rb_rsvg_handle_get_base_uri, 0);
    rb_define_method(cHandle, "set_base_uri", rb_rsvg_handle_set_base_uri, 1);
#endif

    /* Convenience API */
    rb_define_module_function(mRSVG, "pixbuf_from_file",
                              rb_rsvg_pixbuf_from_file, 1);
    rb_define_module_function(mRSVG, "pixbuf_from_file_at_zoom",
                              rb_rsvg_pixbuf_from_file_at_zoom, 3);
    rb_define_module_function(mRSVG, "pixbuf_from_file_at_size",
                              rb_rsvg_pixbuf_from_file_at_size, 3);
    rb_define_module_function(mRSVG, "pixbuf_from_file_at_max_size",
                              rb_rsvg_pixbuf_from_file_at_max_size, 3);
    rb_define_module_function(mRSVG, "pixbuf_from_file_at_zoom_with_max",
                              rb_rsvg_pixbuf_from_file_at_zoom_with_max, 5);

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
    rb_define_method(cHandle, "dimensions", rb_rsvg_handle_get_dim, 0);
#endif

    /* Accessibility API */
    rb_define_method(cHandle, "title", rb_rsvg_handle_get_title, 0);
    rb_define_method(cHandle, "desc", rb_rsvg_handle_get_desc, 0);
#ifdef HAVE_RSVG_HANDLE_GET_METADATA
    rb_define_method(cHandle, "metadata", rb_rsvg_handle_get_metadata, 0);
#endif


#if !LIBRSVG_CHECK_VERSION(2, 11, 0)
    /* Extended Convenience API */
    rb_define_method(cHandle, "pixbuf_from_file_at_size",
                     rb_rsvg_pixbuf_from_file_at_size_ex, 3);
    rb_define_method(cHandle, "pixbuf_from_file",
                     rb_rsvg_pixbuf_from_file_ex, 1);
    rb_define_method(cHandle, "pixbuf_from_file_at_zoom",
                     rb_rsvg_pixbuf_from_file_at_zoom_ex, 3);
    rb_define_method(cHandle, "pixbuf_from_file_at_max_size",
                     rb_rsvg_pixbuf_from_file_at_max_size_ex, 3);
    rb_define_method(cHandle, "pixbuf_from_file_at_zoom_with_max",
                     rb_rsvg_pixbuf_from_file_at_zoom_with_max_ex, 5);
#endif

    rb_define_singleton_method(mRSVG, "cairo_available?",
                               rb_rsvg_cairo_available, 0);
#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
    rb_define_method(cHandle, "render_cairo", rb_rsvg_handle_render_cairo, -1);
#endif

    G_DEF_SETTERS(cHandle);
}
