/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkaccelgroup.c -

  $Author: sakai $
  $Date: 2002/07/28 05:34:04 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(w) GTK_ACCEL_GROUP(RVAL2GOBJ(w))

static VALUE
gaccelgrp_initialize(self)
    VALUE self;
{
    RBGOBJ_INITIALIZE(self, gtk_accel_group_new());
	return Qnil;
}

static VALUE
gaccelgrp_s_activate(self, obj, key, modtype)
    VALUE self, obj, key, modtype;
{
    gtk_accel_groups_activate(G_OBJECT(RVAL2GOBJ(obj)),
                              NUM2INT(key),
                              NUM2INT(modtype));
    return self;
}

static VALUE
gaccelgrp_attach(self, obj)
    VALUE self, obj;
{
     _gtk_accel_group_attach(_SELF(self), RVAL2GOBJ(obj));
    return Qnil;
}

static VALUE
gaccelgrp_detach(self, obj)
    VALUE self, obj;
{
    _gtk_accel_group_detach(_SELF(self), RVAL2GOBJ(obj));
    return Qnil;
}

static VALUE
gaccelgrp_lock(self)
    VALUE self;
{
    gtk_accel_group_lock(_SELF(self));
    return Qnil;
}

static VALUE
gaccelgrp_unlock(self)
    VALUE self;
{
    gtk_accel_group_unlock(_SELF(self));
    return Qnil;
}

void Init_gtk_accel_group()
{
    static RGObjClassInfo cinfo;
    gAccelGroup = rb_define_class_under(mGtk, "AccelGroup", rbgobj_cGObject);

    cinfo.klass = gAccelGroup;
    cinfo.gtype = GTK_TYPE_ACCEL_GROUP;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_singleton_method(gAccelGroup, "activate", gaccelgrp_s_activate, 3);
    rb_define_singleton_method(gAccelGroup, "initialize", gaccelgrp_initialize, 0);
    rb_define_method(gAccelGroup, "attach", gaccelgrp_attach, 1);
    rb_define_method(gAccelGroup, "detach", gaccelgrp_detach, 1);
    rb_define_method(gAccelGroup, "lock", gaccelgrp_lock, 0);
    rb_define_method(gAccelGroup, "unlock", gaccelgrp_unlock, 0);
    rb_define_const(gAccelGroup, "ACCEL_VISIBLE", INT2NUM(GTK_ACCEL_VISIBLE));
    rb_define_const(gAccelGroup, "ACCEL_LOCKED", INT2NUM(GTK_ACCEL_LOCKED));
    rb_define_const(gAccelGroup, "ACCEL_MASK", INT2NUM(GTK_ACCEL_MASK));
}
