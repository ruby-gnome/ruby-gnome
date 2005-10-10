/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gnome-print-unit.c -

  $Author: ktou $
  $Date: 2005/10/10 01:59:48 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

static VALUE
gp_unit_get_identity(VALUE self, VALUE base)
{
  const GnomePrintUnit *unit;
  unit = gnome_print_unit_get_identity(RVAL2GPUB(base));

  if (unit == NULL) {
    return Qnil;
  } else {
    return CONST_GPU2RVAL(unit);
  }
}

static VALUE
gp_unit_get_default(VALUE self)
{
  return CONST_GPU2RVAL(gnome_print_unit_get_default());
}

static VALUE
gp_unit_get_by_name(VALUE self, VALUE name)
{
  const GnomePrintUnit *unit;
  unit = gnome_print_unit_get_by_name((const guchar*)RVAL2CSTR(name));

  if (unit == NULL) {
    return Qnil;
  } else {
    return CONST_GPU2RVAL(unit);
  }
}

static VALUE
gp_unit_get_by_abbreviation(VALUE self, VALUE abbreviation)
{
  const GnomePrintUnit *unit;
  unit = gnome_print_unit_get_by_abbreviation((const guchar*)RVAL2CSTR(abbreviation));

  if (unit == NULL) {
    return Qnil;
  } else {
    return CONST_GPU2RVAL(unit);
  }
}

static VALUE
gp_unit_get_list(VALUE self, VALUE bases)
{
  VALUE array;
  GList *list = gnome_print_unit_get_list(RVAL2GPUB(bases));
  array = GLIST2ARY2(list, GNOME_TYPE_PRINT_UNIT);
  gnome_print_unit_free_list(list);
  return array;
}


static VALUE
gp_unit_get_version(VALUE self)
{
  return INT2NUM(RVAL2GPU(self)->version);
}

static VALUE
gp_unit_get_base(VALUE self)
{
  return GPUB2RVAL(RVAL2GPU(self)->base);
}

static VALUE
gp_unit_get_unittobase(VALUE self)
{
  return rb_float_new(RVAL2GPU(self)->unittobase);
}

static VALUE
gp_unit_get_name(VALUE self)
{
  return CSTR2RVAL((const char*)RVAL2GPU(self)->name);
}

static VALUE
gp_unit_get_abbr(VALUE self)
{
  return CSTR2RVAL((const char*)RVAL2GPU(self)->abbr);
}

static VALUE
gp_unit_get_plural(VALUE self)
{
  return CSTR2RVAL((const char*)RVAL2GPU(self)->plural);
}

static VALUE
gp_unit_get_abbr_plural(VALUE self)
{
  return CSTR2RVAL((const char*)RVAL2GPU(self)->abbr_plural);
}



static VALUE
gp_convert_distance(VALUE self, VALUE distance, VALUE to)
{
  gboolean ret;
  gdouble dist = NUM2DBL(distance);
  VALUE result;
  
  ret = gnome_print_convert_distance(&dist,
                                     RVAL2GPU(self),
                                     RVAL2GPU(to));

  if (ret) {
    result = rb_float_new(dist);
  } else {
    result = Qnil;
  }
  
  return result;
}

static VALUE
gp_convert_distance_full(VALUE self, VALUE distance, VALUE to,
                         VALUE ctm_scale, VALUE device_scale)
{
  gboolean ret;
  gdouble dist = NUM2DBL(distance);
  VALUE result;
  
  ret = gnome_print_convert_distance_full(&dist,
                                          RVAL2GPU(self),
                                          RVAL2GPU(to),
                                          NUM2DBL(ctm_scale),
                                          NUM2DBL(device_scale));

  if (ret) {
    result = rb_float_new(dist);
  } else {
    result = Qnil;
  }
  
  return result;
}

static VALUE
gp_generic_convert_distance(int argc, VALUE *argv, VALUE self)
{
  VALUE distance, to, ctmscale, devicescale;
  
  rb_scan_args(argc, argv, "22", &distance, &to, &ctmscale, &devicescale);

  if (NIL_P(ctmscale) || NIL_P(devicescale)) {
    return gp_convert_distance(self, distance, to);
  } else {
    return gp_convert_distance_full(self, distance, to, ctmscale, devicescale);
  }
}


void
Init_gnome_print_unit(VALUE mGnome)
{
  VALUE cUnit = G_DEF_CLASS(GNOME_TYPE_PRINT_UNIT, "PrintUnit", mGnome);
  VALUE cBase;
  
  cBase = G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_UNIT_BASE,
                      "PrintUnitBase", mGnome);

  G_DEF_CONSTANTS(cUnit, GNOME_TYPE_PRINT_PRINT_UNIT_BASE, "GNOME_PRINT_");
 
  rb_define_const(cUnit, "PS_UNIT", CONST_GPU2RVAL(GNOME_PRINT_PS_UNIT));
 
  rb_define_const(cUnit,
                  "UNIT_ALL",
                  rb_funcall(cBase,
                             rb_intern("new"),
                             1,
                             GPUB2RVAL(GNOME_PRINT_UNITS_ALL)));
  rb_define_const(cBase, "ALL", rb_const_get(cUnit, rb_intern("UNIT_ALL")));

  rb_define_method(cUnit, "version", gp_unit_get_version, 0);
  rb_define_method(cUnit, "base", gp_unit_get_base, 0);
  rb_define_method(cUnit, "unit_to_base", gp_unit_get_unittobase, 0);
  rb_define_method(cUnit, "name", gp_unit_get_name, 0);
  rb_define_method(cUnit, "abbr", gp_unit_get_abbr, 0);
  rb_define_method(cUnit, "plural", gp_unit_get_plural, 0);
  rb_define_method(cUnit, "abbr_plural", gp_unit_get_abbr_plural, 0);
  
  rb_define_module_function(cUnit, "get_identity", gp_unit_get_identity, 1);
  rb_define_module_function(cUnit, "default", gp_unit_get_default, 0);
  rb_define_module_function(cUnit, "get_by_name", gp_unit_get_by_name, 1);
  rb_define_module_function(cUnit, "get_by_abbreviation",
                            gp_unit_get_by_abbreviation, 1);
  rb_define_module_function(cUnit, "get_list", gp_unit_get_list, 1);


/* Utility */
  rb_define_method(cUnit, "convert_distance",
                   gp_generic_convert_distance, -1);
  rb_define_method(cUnit, "convert_distance_full",
                   gp_convert_distance_full, 4);
}
