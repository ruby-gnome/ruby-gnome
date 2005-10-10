/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-printer.c -

  $Author: ktou $
  $Date: 2005/10/10 02:07:41 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-printer.h>

#define _SELF(self) (GP_GPA_PRINTER(self))

static VALUE
gp_gpa_printer_initialize(VALUE self, VALUE id, VALUE name, VALUE model, VALUE settings)
{
    G_INITIALIZE(self, gpa_printer_new(RVAL2CSTR(id),
                                       RVAL2CSTR(name),
                                       RVAL2GOBJ(model),
                                       RVAL2GOBJ(settings)));
    return Qnil;
}

static VALUE
gp_gpa_printer_default(VALUE self)
{
    return GOBJ2RVAL(gpa_printer_get_default());
}

static VALUE
gp_gpa_printer_get_by_id(VALUE self, VALUE id)
{
    return GOBJ2RVAL(gpa_printer_get_by_id((const guchar*)RVAL2CSTR(id)));
}

/* static VALUE */
/* gp_gpa_printer_model(VALUE self) */
/* { */
/*   return GOBJ2RVAL((_SELF(self))->model); */
/* } */

static VALUE
gp_gpa_printer_set_polling(VALUE self, VALUE polling)
{
    gpa_printer_set_polling(_SELF(self), RVAL2CBOOL(polling));
    return Qnil;
}

static VALUE
gp_gpa_printer_default_settings(VALUE self)
{
    return GOBJ2RVAL(gpa_printer_get_default_settings(_SELF(self)));
}

static VALUE
gp_gpa_printer_settings_by_id(VALUE self, VALUE id)
{
    return GOBJ2RVAL(gpa_printer_get_settings_by_id(_SELF(self), (const guchar*)RVAL2CSTR(id)));
}

static VALUE
gp_gpa_printer_settings_generic(int argc, VALUE *argv, VALUE self)
{
    VALUE id;
    rb_scan_args(argc, argv, "01", &id);

    if (NIL_P(id)) {
        return gp_gpa_printer_default_settings(self);
    } else {
        return gp_gpa_printer_settings_by_id(self, id);
    }
}


static VALUE
gp_gpa_printer_state(VALUE self)
{
    return GOBJ2RVAL(gpa_printer_get_state(_SELF(self)));
}

static VALUE
gp_gpa_printer_state_by_id(VALUE self, VALUE id)
{
    return GOBJ2RVAL(gpa_printer_get_state_by_id(_SELF(self), (const guchar*)RVAL2CSTR(id)));
}

static VALUE
gp_gpa_printer_state_generic(int argc, VALUE *argv, VALUE self)
{
    VALUE id;
    rb_scan_args(argc, argv, "01", &id);

    if (NIL_P(id)) {
        return gp_gpa_printer_state(self);
    } else {
        return gp_gpa_printer_state_by_id(self, id);
    }
}


void
Init_gnome_print_gpa_printer(VALUE mGnome)
{
    VALUE c = G_DEF_CLASS(GPA_TYPE_PRINTER, "GPAPrinter", mGnome);

    rb_define_module_function(c, "default", gp_gpa_printer_default, 0);
    rb_define_module_function(c, "get", gp_gpa_printer_get_by_id, 1);
  
    rb_define_method(c, "initialize", gp_gpa_printer_initialize, 4);

    rb_define_method(c, "set_polling", gp_gpa_printer_set_polling, 1);
/*   rb_define_method(c, "model", gp_gpa_printer_model, 0); */
    rb_define_method(c, "settings", gp_gpa_printer_settings_generic, -1);
    rb_define_method(c, "state", gp_gpa_printer_state_generic, -1);
  
    G_DEF_SETTERS(c);
}
