/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelkey.c -

  $Author: ogs $
  $Date: 2002/12/09 16:38:41 $

  Copyright (C) 2002 OGASAWARA, Takeshi
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
accel_key_initialize(self)
    VALUE self;
{
    GtkAccelKey key;
    G_INITIALIZE(self, &key);
    return Qnil;
}

static VALUE
accel_key_get_key(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->accel_key);
}

static VALUE
accel_key_get_mods(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->accel_mods);
}

static VALUE
accel_key_get_flags(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->accel_flags);
}

/*
static VALUE
accel_key_set_key(self, key)
    VALUE self, key;
{
    _SELF(self)->accel_key = NUM2UINT(key);
    return self;
}

static VALUE
accel_key_set_mods(self, mods)
    VALUE self, mods;
{
    _SELF(self)->accel_mods = FIX2INT(mods);
    return self;
}

static VALUE
accel_key_set_flags(self, flags)
    VALUE self, flags;
{
    _SELF(self)->accel_flags = FIX2INT(flags);
    return self;
}
*/

void
Init_accel_key()
{
    VALUE gAccelKey = G_DEF_CLASS(GTK_TYPE_ACCEL_KEY, "AccelKey", mGtk);

    rb_define_method(gAccelKey, "initialize", accel_key_initialize, 0);

    rb_define_method(gAccelKey, "key", accel_key_get_key, 0);
    rb_define_method(gAccelKey, "mods", accel_key_get_mods, 0);
    rb_define_method(gAccelKey, "flags", accel_key_get_flags, 0);
    /*
    rb_define_method(gAccelKey, "set_key", accel_key_set_key, 1);
    rb_define_method(gAccelKey, "set_mods", accel_key_set_mods, 1);
    rb_define_method(gAccelKey, "set_flags", accel_key_set_flags, 1);
    
    G_DEF_SETTERS(gAccelKey);
    */
}
