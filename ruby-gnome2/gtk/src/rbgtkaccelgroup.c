/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkaccelgroup.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:36 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
gaccelgrp_s_new(self)
    VALUE self;
{
    return make_gtkaccelgrp(gtk_accel_group_new());
}

static VALUE
gaccelgrp_s_get_default(self)
    VALUE self;
{
    return make_gtkaccelgrp(gtk_accel_group_get_default());
}

static VALUE
gaccelgrp_activate(self, key, modtype)
    VALUE self, key, modtype;
{
    gtk_accel_group_activate(get_gtkaccelgrp(self),
                             NUM2INT(key),
                             NUM2INT(modtype));
    return self;
}

static VALUE
gaccelgrp_s_activate(self, obj, key, modtype)
    VALUE self, obj, key, modtype;
{
    gtk_accel_groups_activate(get_gobject(obj),
                              NUM2INT(key),
                              NUM2INT(modtype));
    return self;
}

static VALUE
gaccelgrp_attach(self, obj)
    VALUE self, obj;
{
    gtk_accel_group_attach(get_gtkaccelgrp(self),
                           GTK_OBJECT(get_gobject(obj)));
    return Qnil;
}

static VALUE
gaccelgrp_detach(self, obj)
    VALUE self, obj;
{
    gtk_accel_group_detach(get_gtkaccelgrp(self),
                           GTK_OBJECT(get_gobject(obj)));
    return Qnil;
}

static VALUE
gaccelgrp_lock(self)
    VALUE self;
{
    gtk_accel_group_lock(get_gtkaccelgrp(self));
    return Qnil;
}

static VALUE
gaccelgrp_unlock(self)
    VALUE self;
{
    gtk_accel_group_unlock(get_gtkaccelgrp(self));
    return Qnil;
}

static VALUE
gaccelgrp_add(self, key, modtype, flag, obj, strsig)
    VALUE self, key, modtype, flag, obj, strsig;
{
    gtk_accel_group_add(get_gtkaccelgrp(self),
                        NUM2INT(key),
			NUM2INT(modtype),
			NUM2INT(flag),
			GTK_OBJECT(get_gobject(obj)),
			STR2CSTR(strsig));
    return Qnil;
}

static VALUE
gaccelgrp_remove(self, key, modtype, obj)
    VALUE self, key, modtype, obj;
{
    gtk_accel_group_remove(get_gtkaccelgrp(self),
                           NUM2INT(key),
			   NUM2INT(modtype),
			   GTK_OBJECT(get_gobject(obj)));
    return Qnil;
}

void Init_gtk_accel_group()
{
    gAccelGroup = rb_define_class_under(mGtk, "AccelGroup", rb_cData);
    rb_define_singleton_method(gAccelGroup, "new", gaccelgrp_s_new, 0);
    rb_define_singleton_method(gAccelGroup, "get_default", gaccelgrp_s_get_default, 0);
    rb_define_singleton_method(gAccelGroup, "activate", gaccelgrp_s_activate, 3);
    rb_define_method(gAccelGroup, "attach", gaccelgrp_attach, 1);
    rb_define_method(gAccelGroup, "detach", gaccelgrp_detach, 1);
    rb_define_method(gAccelGroup, "lock", gaccelgrp_lock, 0);
    rb_define_method(gAccelGroup, "unlock", gaccelgrp_unlock, 0);
    rb_define_method(gAccelGroup, "add", gaccelgrp_add, 5);
    rb_define_method(gAccelGroup, "remove", gaccelgrp_remove, 3);
    rb_define_method(gAccelGroup, "activate", gaccelgrp_activate, 2);
    rb_define_method(gAccelGroup, "activate", gaccelgrp_activate, 2);
    rb_define_const(gAccelGroup, "ACCEL_VISIBLE", INT2NUM(GTK_ACCEL_VISIBLE));
    rb_define_const(gAccelGroup, "ACCEL_SIGNAL_VISIBLE", INT2NUM(GTK_ACCEL_SIGNAL_VISIBLE));
    rb_define_const(gAccelGroup, "ACCEL_LOCKED", INT2NUM(GTK_ACCEL_LOCKED));
    rb_define_const(gAccelGroup, "ACCEL_MASK", INT2NUM(GTK_ACCEL_MASK));
}
