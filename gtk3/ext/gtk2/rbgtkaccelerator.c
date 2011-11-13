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

#define RG_TARGET_NAMESPACE mAccelerator

#define RVAL2MOD(mods) RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE)

static VALUE
rg_s_valid(G_GNUC_UNUSED VALUE self, VALUE keyval, VALUE modifiers)
{
    return CBOOL2RVAL(gtk_accelerator_valid(NUM2UINT(keyval), RVAL2MOD(modifiers)));
}

static VALUE
rg_s_parse(G_GNUC_UNUSED VALUE self, VALUE accelerator)
{
    guint key;
    GdkModifierType mods;
    gtk_accelerator_parse(RVAL2CSTR(accelerator), &key, &mods);
    return rb_ary_new3(2, UINT2NUM(key), GFLAGS2RVAL(mods, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
rg_s_to_name(G_GNUC_UNUSED VALUE self, VALUE key, VALUE mods)
{
    return CSTR2RVAL(gtk_accelerator_name(NUM2UINT(key), RVAL2MOD(mods)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_s_get_label(G_GNUC_UNUSED VALUE self, VALUE key, VALUE mods)
{
    return CSTR2RVAL(gtk_accelerator_get_label(NUM2UINT(key), RVAL2MOD(mods)));
}
#endif

static VALUE
rg_s_set_default_mod_mask(VALUE self, VALUE default_mod_mask)
{
    gtk_accelerator_set_default_mod_mask(RVAL2MOD(default_mod_mask));
    return self;
}

static VALUE
rg_s_default_mod_mask(G_GNUC_UNUSED VALUE self)
{
    return GFLAGS2RVAL(gtk_accelerator_get_default_mod_mask(), GDK_TYPE_MODIFIER_TYPE);
}

void
Init_gtk_accelerator(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGtk, "Accelerator");

    RG_DEF_SMETHOD(valid, 2);
    RG_DEF_SMETHOD(parse, 1);
    /* name is reserved by Ruby */
    RG_DEF_SMETHOD(to_name, 2);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_SMETHOD(get_label, 2);
#endif
    RG_DEF_SMETHOD(set_default_mod_mask, 1);
    RG_DEF_SMETHOD(default_mod_mask, 0);
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
