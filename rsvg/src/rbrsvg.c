/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
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

#include <ruby.h>

#include <rbglib.h>
#include <rbgobject.h>

#include <librsvg/rsvg-gz.h>

#ifdef HAVE_LIBRSVG_LIBRSVG_ENUM_TYPES_H
#  include <librsvg/librsvg-enum-types.h>
#else
#  include "librsvg-enum-types.h"
#endif

#define _SELF(self) ((RsvgHandle *)DATA_PTR(self))

static VALUE cHandle;

static ID id_at;
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
    w = rb_funcall(result, id_at, 1, INT2NUM(0));
    h = rb_funcall(result, id_at, 1, INT2NUM(1));
    if (!NIL_P(w)) {
      *width = NUM2INT(w);
    }
    if (!NIL_P(h)) {
      *height = NUM2INT(h);
    }
  }
}

static VALUE
rb_rsvg_set_default_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
{
#ifdef HAVE_RSVG_SET_DEFAULT_DPI_X_Y
  rsvg_set_default_dpi_x_y(NUM2DBL(dpi_x), NUM2DBL(dpi_y));
#else
  rb_warning("not supported in your librsvg");
#endif
  return Qnil;
}

static VALUE
rb_rsvg_handle_set_dpi_x_y(VALUE self, VALUE dpi_x, VALUE dpi_y)
{
#ifdef HAVE_RSVG_HANDLE_SET_DPI_X_Y
  rsvg_handle_set_dpi_x_y(_SELF(self), NUM2DBL(dpi_x), NUM2DBL(dpi_y));
#else
  rb_warning("not supported in your librsvg");
#endif
  return Qnil;
}

static VALUE
rb_rsvg_handle_new(int argc, VALUE *argv, VALUE klass)
{
  RsvgHandle *handle;
  VALUE gz, obj;
  rb_scan_args(argc, argv, "01", &gz);

  if (RTEST(gz)) {
    handle = rsvg_handle_new_gz();
  } else {
    handle = rsvg_handle_new();
  }

  obj = Data_Wrap_Struct(cHandle, 0, rsvg_handle_free, handle);
  if (argc == 0) {
    rb_obj_call_init(obj, argc, argv);
  } else{
    rb_obj_call_init(obj, argc - 1, &argv[1]);
  }
  return obj;
}

static VALUE
rb_rsvg_handle_initialize(VALUE self)
{
  rb_ivar_set(self, id_closed, Qfalse);
  return Qnil;
}

static VALUE
rb_rsvg_handle_set_size_callback(VALUE self)
{
  rb_ivar_set(self, id_callback, rb_block_proc());
  rsvg_handle_set_size_callback(_SELF(self), exec_callback,
                                (gpointer)self, NULL);
  return Qnil;
}

static VALUE
rb_rsvg_handle_write(VALUE self, VALUE buf)
{
  gboolean result;
  GError *error = NULL;

  result = rsvg_handle_write(_SELF(self), RVAL2CSTR(buf),
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
rb_rsvg_handle_get_pixbuf(VALUE self)
{
  return GOBJ2RVAL(rsvg_handle_get_pixbuf(_SELF(self)));
}


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

void
Init_rsvg2(void) {
  VALUE mRSVG = rb_define_module("RSVG");

  cHandle = rb_define_class_under(mRSVG, "Handle", rb_cObject);

  G_DEF_ERROR(RSVG_ERROR, "Error", mRSVG, rb_eRuntimeError, RSVG_TYPE_ERROR);

  id_call = rb_intern("call");
  id_at = rb_intern("at");
  id_callback = rb_intern("callback");
  id_closed = rb_intern("closed");

  rb_define_module_function(mRSVG, "set_default_dpi_x_y",
                            rb_rsvg_set_default_dpi_x_y, 2);

  rb_define_singleton_method(cHandle, "new", rb_rsvg_handle_new, -1);

  rb_define_method(cHandle, "initialize", rb_rsvg_handle_initialize, 0);
  rb_define_method(cHandle, "set_size_callback",
                   rb_rsvg_handle_set_size_callback, 0);
  rb_define_method(cHandle, "set_dpi_x_y", rb_rsvg_handle_set_dpi_x_y, 2);
  rb_define_method(cHandle, "write", rb_rsvg_handle_write, 1);
  rb_define_method(cHandle, "close", rb_rsvg_handle_close, 0);
  rb_define_method(cHandle, "closed?", rb_rsvg_handle_closed, 0);
  rb_define_method(cHandle, "pixbuf", rb_rsvg_handle_get_pixbuf, 0);

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

/* Accessibility API */

  rb_define_method(cHandle, "title", rb_rsvg_handle_get_title, 0);
  rb_define_method(cHandle, "desc", rb_rsvg_handle_get_desc, 0);

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
  
  G_DEF_SETTERS(cHandle);
}
