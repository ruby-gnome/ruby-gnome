/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#define RG_TARGET_NAMESPACE cAccelKey
#define _SELF(s) ((GtkAccelKey*)RVAL2BOXED(s, GTK_TYPE_ACCEL_KEY))
#define GTK_TYPE_ACCEL_KEY (gtk_accel_key_get_type())

static GtkAccelKey *
accel_key_copy(const GtkAccelKey *key)
{
    GtkAccelKey *new_key;
    g_return_val_if_fail (key != NULL, NULL);
    new_key = g_new(GtkAccelKey, 1);
    *new_key = *key;
    return new_key;
}

GType
gtk_accel_key_get_type(void)
{
    static GType our_type = 0;
    if(our_type == 0)
        our_type = g_boxed_type_register_static("GtkAccelKey",
                                                (GBoxedCopyFunc)accel_key_copy,
                                                (GBoxedFreeFunc)g_free);
    return our_type;
}

/*
struct _GtkAccelKey
{
  guint           accel_key;
  GdkModifierType accel_mods;
  guint           accel_flags : 16;
};
*/

static VALUE
rg_initialize(VALUE self)
{
    GtkAccelKey key;
    G_INITIALIZE(self, &key);
    return Qnil;
}

static VALUE
rg_key(VALUE self)
{
    return UINT2NUM(_SELF(self)->accel_key);
}

static VALUE
rg_mods(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->accel_mods, GDK_TYPE_MODIFIER_TYPE);
}

static VALUE
rg_flags(VALUE self)
{
    return INT2FIX(_SELF(self)->accel_flags);
}

static VALUE
rg_set_key(VALUE self, VALUE key)
{
    _SELF(self)->accel_key = NUM2UINT(key);
    return self;
}

static VALUE
rg_set_mods(VALUE self, VALUE mods)
{
    _SELF(self)->accel_mods = RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE);
    return self;
}

static VALUE
rg_set_flags(VALUE self, VALUE flags)
{
    _SELF(self)->accel_flags = FIX2INT(flags);
    return self;
}

void
Init_gtk_accel_key(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCEL_KEY, "AccelKey", mGtk);

    RG_DEF_METHOD(initialize, 0);

    RG_DEF_METHOD(key, 0);
    RG_DEF_METHOD(mods, 0);
    RG_DEF_METHOD(flags, 0);

    RG_DEF_METHOD(set_key, 1);
    RG_DEF_METHOD(set_mods, 1);
    RG_DEF_METHOD(set_flags, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
