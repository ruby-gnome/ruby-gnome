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

#include "rbgp.h"
#include <libgnomeprint/gnome-print-config.h>

#define _SELF(self) (GP_CONFIG(self))

static VALUE s_string, s_boolean, s_int, s_double, s_length, s_transform;

static VALUE
gp_config_default(VALUE self)
{
  return GOBJ2RVAL(gnome_print_config_default());
}

static VALUE
gp_config_dup(VALUE self)
{
  return GOBJ2RVAL(gnome_print_config_dup(_SELF(self)));
}

static VALUE
gp_config_to_string(int argc, VALUE *argv, VALUE self)
{
  VALUE flags; /* currently not used in libgnomeprint. */
  rb_scan_args(argc, argv, "01", &flags);
  if (NIL_P(flags)) {
    flags = INT2NUM(0);
  }
  
  return CSTR2RVAL(gnome_print_config_to_string(_SELF(self),
                                                NUM2UINT(flags)));
}

static VALUE
gp_config_from_string(int argc, VALUE* argv, VALUE self)
{
  VALUE flags; /* currently not used in libgnomeprint. */
  VALUE string;
  rb_scan_args(argc, argv, "11", &string, &flags);
  if (NIL_P(flags)) {
    flags = INT2NUM(0);
  }
  
  return GOBJ2RVAL(gnome_print_config_from_string(RVAL2CSTR(string),
                                                  NUM2UINT(flags)));
}

static VALUE
gp_config_new(int argc, VALUE *argv, VALUE self)
{
  if (argc == 0) {
    return gp_config_default(self);
  } else if (argc == 1 || argc == 2) {
    return gp_config_from_string(argc, argv, self);
  } else {
    rb_raise(rb_eArgError, "invalid argument number");
    return rb_call_super(argc, argv);
  }
}

static VALUE
gp_config_get(VALUE self, VALUE key)
{
  guchar *value;
  value = gnome_print_config_get(_SELF(self),
                                 RVAL2CSTR(key));
  return(value ? CSTR2RVAL(value) : Qnil);
}

static VALUE
gp_config_get_boolean(VALUE self, VALUE key)
{
  gboolean value;
  gboolean result;
  
  result = gnome_print_config_get_boolean(_SELF(self),
                                          RVAL2CSTR(key),
                                          &value);
  return(result ? CBOOL2RVAL(value) : Qnil);
}

static VALUE
gp_config_get_int(VALUE self, VALUE key)
{
  gint value;
  gboolean result;

  result = gnome_print_config_get_int(_SELF(self),
                                      RVAL2CSTR(key),
                                      &value);
  return(result ? INT2NUM(value) : Qnil);
}

static VALUE
gp_config_get_double(VALUE self, VALUE key)
{
  gdouble value;
  gboolean result;

  result = gnome_print_config_get_double(_SELF(self),
                                         RVAL2CSTR(key),
                                         &value);
  return(result ? rb_float_new(value) : Qnil);
}

static VALUE
gp_config_get_length(VALUE self, VALUE key)
{
  gdouble value;
  gboolean result;
  const GnomePrintUnit *unit;
  
  result = gnome_print_config_get_length(_SELF(self),
                                         RVAL2CSTR(key),
                                         &value,
                                         &unit);
  if (result) {
    return rb_ary_new3(2,
                       rb_float_new(value),
                       GOBJ2RVAL((GnomePrintUnit *)unit));
  } else {
    return Qnil;
  }
}

#define TRANSFORM_SIZE 6

static VALUE
gp_config_get_transform(VALUE self, VALUE key)
{
  gdouble *value = ALLOCA_N(gdouble, TRANSFORM_SIZE);
  gboolean result;
  int i;
  
  result = gnome_print_config_get_transform(_SELF(self),
                                            RVAL2CSTR(key),
                                            value);

  if (result) {
    return Qnil;
  } else {
    VALUE array = rb_ary_new();
    for (i = 0; i < TRANSFORM_SIZE; i++) {
      rb_ary_push(array, rb_float_new(*value));
      value++;
    }
    
    return array;
  }
}

static VALUE
gp_config_get_generic(int argc, VALUE *argv, VALUE self)
{
  VALUE key, type, result;

  rb_scan_args(argc, argv, "11", &key, &type);

  if (NIL_P(type) || (RTEST(rb_equal(type, s_string)))) {
    result = gp_config_get(self, key);
  } else if (RTEST(rb_equal(type, s_boolean))) {
    result = gp_config_get_boolean(self, key);
  } else if (RTEST(rb_equal(type, s_int))) {
    result = gp_config_get_int(self, key);
  } else if (RTEST(rb_equal(type, s_double))) {
    result = gp_config_get_double(self, key);
  } else if (RTEST(rb_equal(type, s_length))) {
    result = gp_config_get_length(self, key);
  } else if (RTEST(rb_equal(type, s_transform))) {
    result = gp_config_get_transform(self, key);
  } else {
    result = Qnil;
  }
    
  return result;
}


static VALUE
gp_config_get_page_size(VALUE self)
{
  gdouble width, height;
  gnome_print_config_get_page_size(_SELF(self),
                                   &width,
                                   &height);
  return rb_ary_new3(2, rb_float_new(width), rb_float_new(height));
}


static VALUE
gp_config_set(VALUE self, VALUE key, VALUE value)
{
  return CBOOL2RVAL(gnome_print_config_set(_SELF(self),
                                           RVAL2CSTR(key),
                                           RVAL2CSTR(value)));
}

static VALUE
gp_config_set_boolean(VALUE self, VALUE key, VALUE value)
{
  return CBOOL2RVAL(gnome_print_config_set_boolean(_SELF(self),
                                                   RVAL2CSTR(key),
                                                   RVAL2CBOOL(value)));
}

static VALUE
gp_config_set_int(VALUE self, VALUE key, VALUE value)
{
  return CBOOL2RVAL(gnome_print_config_set_int(_SELF(self),
                                               RVAL2CSTR(key),
                                               NUM2INT(value)));
}

static VALUE
gp_config_set_double(VALUE self, VALUE key, VALUE value)
{
  return CBOOL2RVAL(gnome_print_config_set_double(_SELF(self),
                                                  RVAL2CSTR(key),
                                                  NUM2DBL(value)));
}

static VALUE
gp_config_set_length(VALUE self, VALUE key, VALUE value, VALUE unit)
{
  return CBOOL2RVAL(gnome_print_config_set_length(_SELF(self),
                                                  RVAL2CSTR(key),
                                                  NUM2DBL(value),
                                                  RVAL2GOBJ(unit)));
}

static VALUE
gp_config_set_generic(int argc, VALUE *argv, VALUE self)
{
  VALUE key, value, unit, result;
  
  rb_scan_args(argc, argv, "21", &key, &value, &unit);
  
  switch (TYPE(value)) {
  case T_STRING:
    result = gp_config_set(self, key, value);
    break;
  case T_TRUE:
  case T_FALSE:
    result = gp_config_set_boolean(self, key, value);
    break;
  case T_FIXNUM:
    result = gp_config_set_int(self, key, value);
    break;
  case T_FLOAT:
    if (NIL_P(unit)) {
      result = gp_config_set_double(self, key, value);
    } else {
      result = gp_config_set_length(self, key, value, unit);
    }
    break;
  default:
    rb_raise(rb_eArgError, "invalid argument");
    break;
  }
  
  return result;
}


static VALUE
gp_config_dump(VALUE self)
{
  gnome_print_config_dump(_SELF(self));
  return Qnil;
}


void
Init_gnome_print_config(VALUE mGnome, VALUE mGP)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PRINT_CONFIG, "PrintConfig", mGnome);

  s_string = ID2SYM(rb_intern("string"));
  s_boolean = ID2SYM(rb_intern("boolean"));
  s_int = ID2SYM(rb_intern("int"));
  s_double = ID2SYM(rb_intern("double"));
  s_length = ID2SYM(rb_intern("length"));
  s_transform = ID2SYM(rb_intern("transform"));


  rb_define_module_function(c, "default", gp_config_default, 0);
  
  rb_define_module_function(c, "new", gp_config_new, -1);

  rb_define_method(c, "dup", gp_config_dup, 0);
  rb_define_method(c, "to_s", gp_config_to_string, -1);

  rb_define_method(c, "get", gp_config_get_generic, -1);
  rb_define_method(c, "[]", gp_config_get_generic, -1);
  rb_define_method(c, "get_boolean", gp_config_get_boolean, 1);
  rb_define_method(c, "get_int", gp_config_get_int, 1);
  rb_define_method(c, "get_double", gp_config_get_double, 1);
  rb_define_method(c, "get_length", gp_config_get_length, 1);
  rb_define_method(c, "get_transform", gp_config_get_transform, 1);

  rb_define_method(c, "set", gp_config_set_generic, -1);
  rb_define_method(c, "[]=", gp_config_set_generic, -1);

  rb_define_method(c, "page_size", gp_config_get_page_size, 0);

  rb_define_method(c, "dump", gp_config_dump, 0);


/*
 * Convenience keys
 */
  rb_define_const(c, "KEY_PAPER_SIZE", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_SIZE));
  rb_define_const(c, "KEY_PAPER_WIDTH", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_WIDTH));
  rb_define_const(c, "KEY_PAPER_HEIGHT", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_HEIGHT));
  rb_define_const(c, "KEY_PAPER_ORIENTATION", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_ORIENTATION));
  rb_define_const(c, "KEY_PAPER_ORIENTATION_MATRIX", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_ORIENTATION_MATRIX));

  rb_define_const(c, "KEY_PAGE_ORIENTATION", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_ORIENTATION));
  rb_define_const(c, "KEY_PAGE_ORIENTATION_MATRIX", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_ORIENTATION_MATRIX));

/* Just a reminder - application is only interested in logical orientation */
  rb_define_const(c, "KEY_ORIENTATION", CSTR2RVAL(GNOME_PRINT_KEY_ORIENTATION));

  rb_define_const(c, "KEY_LAYOUT", CSTR2RVAL(GNOME_PRINT_KEY_LAYOUT));
  rb_define_const(c, "KEY_LAYOUT_WIDTH", CSTR2RVAL(GNOME_PRINT_KEY_LAYOUT_WIDTH));
  rb_define_const(c, "KEY_LAYOUT_HEIGHT", CSTR2RVAL(GNOME_PRINT_KEY_LAYOUT_HEIGHT));

  rb_define_const(c, "KEY_PAPER_SOURCE", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_SOURCE));

/* Master resolution, i.e. ink dots for color printer RGB resolution is usually smaller */
  rb_define_const(c, "KEY_RESOLUTION", CSTR2RVAL(GNOME_PRINT_KEY_RESOLUTION));
  rb_define_const(c, "KEY_RESOLUTION_DPI", CSTR2RVAL(GNOME_PRINT_KEY_RESOLUTION_DPI));
  rb_define_const(c, "KEY_RESOLUTION_DPI_X", CSTR2RVAL(GNOME_PRINT_KEY_RESOLUTION_DPI_X));
  rb_define_const(c, "KEY_RESOLUTION_DPI_Y", CSTR2RVAL(GNOME_PRINT_KEY_RESOLUTION_DPI_Y));

/* These belong to 'Output' because PGL may implement multiple copies itself */
  rb_define_const(c, "KEY_NUM_COPIES", CSTR2RVAL(GNOME_PRINT_KEY_NUM_COPIES));
  rb_define_const(c, "KEY_NONCOLLATED_COPIES_IN_HW", CSTR2RVAL(GNOME_PRINT_KEY_NONCOLLATED_COPIES_IN_HW));
  rb_define_const(c, "KEY_COLLATED_COPIES_IN_HW", CSTR2RVAL(GNOME_PRINT_KEY_COLLATED_COPIES_IN_HW));

  rb_define_const(c, "KEY_COLLATE", CSTR2RVAL(GNOME_PRINT_KEY_COLLATE));
  rb_define_const(c, "KEY_DUPLEX", CSTR2RVAL(GNOME_PRINT_KEY_DUPLEX));
  rb_define_const(c, "KEY_TUMBLE", CSTR2RVAL(GNOME_PRINT_KEY_TUMBLE));
  rb_define_const(c, "KEY_HOLD", CSTR2RVAL(GNOME_PRINT_KEY_HOLD));

/* These are ignored by libgnomeprint, but you may want to get/set/inspect these */
/* Libgnomeprintui uses these for displaying margin symbols */
  rb_define_const(c, "KEY_PAGE_MARGIN_LEFT", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_MARGIN_LEFT));
  rb_define_const(c, "KEY_PAGE_MARGIN_RIGHT", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_MARGIN_RIGHT));
  rb_define_const(c, "KEY_PAGE_MARGIN_TOP", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_MARGIN_TOP));
  rb_define_const(c, "KEY_PAGE_MARGIN_BOTTOM", CSTR2RVAL(GNOME_PRINT_KEY_PAGE_MARGIN_BOTTOM));

/* These are ignored by libgnomeprint, and you most probably cannot change these too */
/* Also - these are relative to ACTUAL PAGE IN PRINTER - not physicalpage */
/* Libgnomeprintui uses these for displaying margin symbols */
  rb_define_const(c, "KEY_PAPER_MARGIN_LEFT", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_MARGIN_LEFT));
  rb_define_const(c, "KEY_PAPER_MARGIN_RIGHT", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_MARGIN_RIGHT));
  rb_define_const(c, "KEY_PAPER_MARGIN_TOP", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_MARGIN_TOP));
  rb_define_const(c, "KEY_PAPER_MARGIN_BOTTOM", CSTR2RVAL(GNOME_PRINT_KEY_PAPER_MARGIN_BOTTOM));

/* More handy keys */
  rb_define_const(c, "KEY_OUTPUT_FILENAME", CSTR2RVAL(GNOME_PRINT_KEY_OUTPUT_FILENAME));
  rb_define_const(c, "KEY_DOCUMENT_NAME", CSTR2RVAL(GNOME_PRINT_KEY_DOCUMENT_NAME));
  rb_define_const(c, "KEY_PREFERED_UNIT", CSTR2RVAL(GNOME_PRINT_KEY_PREFERED_UNIT));
}

