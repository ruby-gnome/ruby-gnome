#include "rbgp.h"
#include <libgnomeprint/gnome-print-unit.h>
#include <libgnomeprint/libgnomeprint-enum-types.h>

#define RVAL2GPU(obj) ((GnomePrintUnit *)RVAL2BOXED(obj, GNOME_TYPE_PRINT_UNIT))
#define GPU2RVAL(obj) (BOXED2RVAL(obj, GNOME_TYPE_PRINT_UNIT))
#define CONST_GPU2RVAL(obj) (GPU2RVAL((GnomePrintUnit *)obj))


static VALUE
gp_unit_get_identity(VALUE self, VALUE base)
{
  return CONST_GPU2RVAL(gnome_print_unit_get_identity(NUM2INT(base)));
}

static VALUE
gp_unit_get_default(VALUE self)
{
  return CONST_GPU2RVAL(gnome_print_unit_get_default());
}

static VALUE
gp_unit_get_by_name(VALUE self, VALUE name)
{
  return CONST_GPU2RVAL(gnome_print_unit_get_by_name(RVAL2CSTR(name)));
}

static VALUE
gp_unit_get_by_abbreviation(VALUE self, VALUE abbreviation)
{
  return CONST_GPU2RVAL(gnome_print_unit_get_by_abbreviation(RVAL2CSTR(abbreviation)));
}

static VALUE
gp_unit_get_list(VALUE self, VALUE bases)
{
  VALUE array;
  GList *list = gnome_print_unit_get_list(NUM2INT(bases));
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
  return INT2NUM(RVAL2GPU(self)->base);
}

static VALUE
gp_unit_get_unittobase(VALUE self)
{
  return rb_float_new(RVAL2GPU(self)->unittobase);
}

static VALUE
gp_unit_get_name(VALUE self)
{
  return CSTR2RVAL(RVAL2GPU(self)->name);
}

static VALUE
gp_unit_get_abbr(VALUE self)
{
  return CSTR2RVAL(RVAL2GPU(self)->abbr);
}

static VALUE
gp_unit_get_plural(VALUE self)
{
  return CSTR2RVAL(RVAL2GPU(self)->plural);
}

static VALUE
gp_unit_get_abbr_plural(VALUE self)
{
  return CSTR2RVAL(RVAL2GPU(self)->abbr_plural);
}



static VALUE
gp_convert_distance(VALUE self, VALUE distance, VALUE from, VALUE to)
{
  gboolean ret;
  gdouble dist = NUM2DBL(distance);
  
  ret = gnome_print_convert_distance(&dist,
                                     RVAL2GPU(from),
                                     RVAL2GPU(to));
  
  return rb_float_new(dist);
}

static VALUE
gp_convert_distance_full(VALUE self, VALUE distance, VALUE from, VALUE to,
                         VALUE ctmscale, VALUE devicescale)
{
  gboolean ret;
  gdouble dist = NUM2DBL(distance);
  
  ret = gnome_print_convert_distance_full(&dist,
                                          RVAL2GPU(from),
                                          RVAL2GPU(to),
                                          NUM2DBL(ctmscale),
                                          NUM2DBL(devicescale));

  return rb_float_new(dist);
}


void
Init_gnome_print_unit(VALUE mGnome, VALUE mGP)
{
  VALUE cUnit = G_DEF_CLASS(GNOME_TYPE_PRINT_UNIT, "PrintUnit", mGnome);
  VALUE cBase;
  
  cBase = G_DEF_CLASS(GNOME_TYPE_PRINT_PRINT_UNIT_BASE,
                      "PrintUnitBase", mGnome);

  rb_define_const(cUnit, "PS_UNIT", CONST_GPU2RVAL(GNOME_PRINT_PS_UNIT));

  rb_define_const(cBase, "ALL",
                  rb_funcall(cBase,
                             rb_intern("new"),
                             1,
                             INT2NUM(GNOME_PRINT_UNITS_ALL)));

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


  G_DEF_SETTERS(cUnit);

/* Utility */
  rb_define_module_function(mGP, "convert_distance",
                            gp_convert_distance, 3);
  rb_define_module_function(mGP, "convert_distance_full",
                            gp_convert_distance_full, 5);
}
