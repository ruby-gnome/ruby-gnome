/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkaccelgroup.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

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
gaccelgrp_s_activate(self, obj, key, modtype)
    VALUE self, obj, key, modtype;
{
    gtk_accel_groups_activate(G_OBJECT(get_gobject(obj)),
                              NUM2INT(key),
                              NUM2INT(modtype));
    return self;
}

static VALUE
gaccelgrp_attach(self, obj)
    VALUE self, obj;
{
     _gtk_accel_group_attach(get_gtkaccelgrp(self),
                           G_OBJECT(get_gobject(obj)));
    return Qnil;
}

static VALUE
gaccelgrp_detach(self, obj)
    VALUE self, obj;
{
    _gtk_accel_group_detach(get_gtkaccelgrp(self),
                            G_OBJECT(get_gobject(obj)));
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

void Init_gtk_accel_group()
{
    gAccelGroup = rb_define_class_under(mGtk, "AccelGroup", rb_cData);
    rb_define_singleton_method(gAccelGroup, "new", gaccelgrp_s_new, 0);
    rb_define_singleton_method(gAccelGroup, "activate", gaccelgrp_s_activate, 3);
    rb_define_method(gAccelGroup, "attach", gaccelgrp_attach, 1);
    rb_define_method(gAccelGroup, "detach", gaccelgrp_detach, 1);
    rb_define_method(gAccelGroup, "lock", gaccelgrp_lock, 0);
    rb_define_method(gAccelGroup, "unlock", gaccelgrp_unlock, 0);
    rb_define_const(gAccelGroup, "ACCEL_VISIBLE", INT2NUM(GTK_ACCEL_VISIBLE));
    rb_define_const(gAccelGroup, "ACCEL_LOCKED", INT2NUM(GTK_ACCEL_LOCKED));
    rb_define_const(gAccelGroup, "ACCEL_MASK", INT2NUM(GTK_ACCEL_MASK));
}
