/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelkey.c -

  $Author: mutoh $
  $Date: 2004/05/20 16:57:59 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) ((GtkAccelKey*)RVAL2BOXED(s, GTK_TYPE_ACCEL_KEY))
#define GTK_TYPE_ACCEL_KEY (gtk_accel_key_get_type())

static GtkAccelKey*
accel_key_copy(const GtkAccelKey *key)
{
    GtkAccelKey *new_key;
    g_return_val_if_fail (key != NULL, NULL);
    new_key = g_new(GtkAccelKey, 1);
    *new_key = *key;
    return new_key;
}

GType
gtk_accel_key_get_type()
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
accel_key_initialize(VALUE self)
{
    GtkAccelKey key;
    G_INITIALIZE(self, &key);
    return Qnil;
}

static VALUE
accel_key_get_key(VALUE self)
{
    return UINT2NUM(_SELF(self)->accel_key);
}

static VALUE
accel_key_get_mods(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->accel_mods, GDK_TYPE_MODIFIER_TYPE);
}

static VALUE
accel_key_get_flags(VALUE self)
{
    return INT2FIX(_SELF(self)->accel_flags);
}

static VALUE
accel_key_set_key(VALUE self, VALUE key)
{
    _SELF(self)->accel_key = NUM2UINT(key);
    return self;
}

static VALUE
accel_key_set_mods(VALUE self, VALUE mods)
{
    _SELF(self)->accel_mods = RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE);
    return self;
}

static VALUE
accel_key_set_flags(VALUE self, VALUE flags)
{
    _SELF(self)->accel_flags = FIX2INT(flags);
    return self;
}

void
Init_accel_key()
{
    VALUE gAccelKey = G_DEF_CLASS(GTK_TYPE_ACCEL_KEY, "AccelKey", mGtk);

    rb_define_method(gAccelKey, "initialize", accel_key_initialize, 0);

    rb_define_method(gAccelKey, "key", accel_key_get_key, 0);
    rb_define_method(gAccelKey, "mods", accel_key_get_mods, 0);
    rb_define_method(gAccelKey, "flags", accel_key_get_flags, 0);

    rb_define_method(gAccelKey, "set_key", accel_key_set_key, 1);
    rb_define_method(gAccelKey, "set_mods", accel_key_set_mods, 1);
    rb_define_method(gAccelKey, "set_flags", accel_key_set_flags, 1);
    
    G_DEF_SETTERS(gAccelKey);
}
