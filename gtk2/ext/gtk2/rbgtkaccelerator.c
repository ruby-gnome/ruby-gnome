/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "global.h"

#define RVAL2MOD(mods) RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE)

static VALUE
accel_valid(G_GNUC_UNUSED VALUE self, VALUE keyval, VALUE modifiers)
{
    return CBOOL2RVAL(gtk_accelerator_valid(NUM2UINT(keyval), RVAL2MOD(modifiers)));
}

static VALUE
accel_parse(G_GNUC_UNUSED VALUE self, VALUE accelerator)
{
    guint key;
    GdkModifierType mods;
    gtk_accelerator_parse(RVAL2CSTR(accelerator), &key, &mods);
    return rb_ary_new3(2, UINT2NUM(key), GFLAGS2RVAL(mods, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
accel_name(G_GNUC_UNUSED VALUE self, VALUE key, VALUE mods)
{
    return CSTR2RVAL(gtk_accelerator_name(NUM2UINT(key), RVAL2MOD(mods)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
accel_get_label(G_GNUC_UNUSED VALUE self, VALUE key, VALUE mods)
{
    return CSTR2RVAL(gtk_accelerator_get_label(NUM2UINT(key), RVAL2MOD(mods)));
}
#endif

static VALUE
accel_set_default_mod_mask(VALUE self, VALUE default_mod_mask)
{
    gtk_accelerator_set_default_mod_mask(RVAL2MOD(default_mod_mask));
    return self;
}

static VALUE
accel_get_default_mod_mask(G_GNUC_UNUSED VALUE self)
{
    return GFLAGS2RVAL(gtk_accelerator_get_default_mod_mask(), GDK_TYPE_MODIFIER_TYPE);
}

void
Init_gtk_accelerator(void)
{
    VALUE mGtkAccel = rb_define_module_under(mGtk, "Accelerator");

    rb_define_singleton_method(mGtkAccel, "valid", accel_valid, 2);
    rb_define_singleton_method(mGtkAccel, "parse", accel_parse, 1);
    /* name is reserved by Ruby */
    rb_define_singleton_method(mGtkAccel, "to_name", accel_name, 2);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_singleton_method(mGtkAccel, "get_label", accel_get_label, 2);
#endif
    rb_define_singleton_method(mGtkAccel, "set_default_mod_mask", accel_set_default_mod_mask, 1);
    rb_define_singleton_method(mGtkAccel, "default_mod_mask", accel_get_default_mod_mask, 0);
    G_DEF_SETTERS(mGtkAccel);
}
