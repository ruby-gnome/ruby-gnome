/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelerator.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/
#include "global.h"

static VALUE
accel_valid(self, keyval, modifiers)
    VALUE self, keyval, modifiers;
{
    return gtk_accelerator_valid(NUM2UINT(keyval), FIX2INT(modifiers)) ? Qtrue : Qfalse;
}

static VALUE
accel_parse(self, accelerator)
    VALUE self, accelerator;
{
    guint key;
    GdkModifierType mods;
    gtk_accelerator_parse(RVAL2CSTR(accelerator), &key, &mods);
    return rb_ary_new3(2, UINT2NUM(key), INT2FIX(mods));
}

static VALUE
accel_name(self, key, mods)
    VALUE self, key, mods;
{
    return CSTR2RVAL(gtk_accelerator_name(NUM2UINT(key), FIX2INT(mods)));
}

static VALUE
accel_set_default_mod_mask(self, default_mod_mask)
    VALUE self, default_mod_mask;
{
    gtk_accelerator_set_default_mod_mask(FIX2INT(default_mod_mask));
    return self;
}

static VALUE
accel_get_default_mod_mask(self)
    VALUE self;
{
    return UINT2NUM(gtk_accelerator_get_default_mod_mask());
}

void
Init_gtk_accelerator()
{
    VALUE mGtkAccel = rb_define_module_under(mGtk, "Accelerator");

    rb_define_singleton_method(mGtkAccel, "valid", accel_valid, 2);
    rb_define_singleton_method(mGtkAccel, "parse", accel_parse, 1);
    /* name is reserved by Ruby */
    rb_define_singleton_method(mGtkAccel, "to_name", accel_name, 2);
    rb_define_singleton_method(mGtkAccel, "set_default_mod_mask", accel_set_default_mod_mask, 1);
    rb_define_singleton_method(mGtkAccel, "default_mod_mask", accel_get_default_mod_mask, 0);
    G_DEF_SETTERS(mGtkAccel);
}
