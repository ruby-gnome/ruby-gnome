/* -*- C-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrequisition.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 2002  Masao Mutoh

  This file is derived from rbgtk.c.
  rbgtk.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(r) ((GtkRequisition*)RVAL2BOXED(r))

static VALUE 
grequisition_initialize(self, w, h)
    VALUE self, w, h;
{
    GtkRequisition r;

    r.width = NUM2INT(w);
    r.height = NUM2INT(h);
    G_INITIALIZE(self, &r);
    return Qnil;
}

static VALUE
grequisition_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
grequisition_h(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
grequisition_set_w(self, w)
    VALUE self, w;
{
    _SELF(self)->width = NUM2INT(w);
    return self;
}

static VALUE
grequisition_set_h(self, h)
    VALUE self, h;
{
    _SELF(self)->height = NUM2INT(h);
    return self;
}

static VALUE
grequisition_to_a(self)
    VALUE self;
{
    GtkRequisition *r;

    r = _SELF(self);
    return rb_ary_new3(2, INT2FIX(r->width), INT2FIX(r->height));
}

void Init_gtk_requisition()
{
    VALUE gRequisition = G_DEF_CLASS(GTK_TYPE_REQUISITION, "Requisition", mGtk);

    rb_define_singleton_method(gRequisition, "initialize", grequisition_initialize, 2);
    rb_define_method(gRequisition, "width", grequisition_w, 0);
    rb_define_method(gRequisition, "height", grequisition_h, 0);
    rb_define_method(gRequisition, "width=", grequisition_set_w, 1);
    rb_define_method(gRequisition, "height=", grequisition_set_h, 1);
    rb_define_method(gRequisition, "to_a", grequisition_to_a, 0);
}
