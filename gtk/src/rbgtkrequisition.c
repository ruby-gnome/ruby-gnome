/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkrequisition.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2002  Masao Mutoh

  This file is derived from rbgtk.c.
  rbgtk.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE 
grequisition_initialize(self, w, h)
    VALUE self, w, h;
{
    GtkRequisition r;

    r.width = NUM2INT(w);
    r.height = NUM2INT(h);
    RBGTK_INITIALIZE(self, make_grequisition(&r));
	return Qnil;
}

static VALUE
grequisition_w(self)
    VALUE self;
{
    return INT2NUM(get_grequisition(self)->width);
}

static VALUE
grequisition_h(self)
    VALUE self;
{
    return INT2NUM(get_grequisition(self)->height);
}

static VALUE
grequisition_set_w(self, w)
    VALUE self, w;
{
    get_grequisition(self)->width = NUM2INT(w);
    return self;
}

static VALUE
grequisition_set_h(self, h)
    VALUE self, h;
{
    get_grequisition(self)->height = NUM2INT(h);
    return self;
}

static VALUE
grequisition_to_a(self)
    VALUE self;
{
    GtkRequisition *r;

    r = get_grequisition(self);
    return rb_ary_new3(2, INT2FIX(r->width), INT2FIX(r->height));
}

static VALUE
grequisition_to_s(self)
    VALUE self;
{
    char str[2 +2*1 +5*2  +1]; /* member is guint16. max string size is 5 */
    GtkRequisition *r;

    r = get_grequisition(self);
    sprintf(str, "(%5d, %5d)", r->width, r->height);
    return rb_str_new2(str);
}

void Init_gtk_requisition()
{
	/* FIXME */
    gRequisition = rb_define_class_under(mGtk, "Requisition", rb_cData);

    rb_define_singleton_method(gRequisition, "initialize", grequisition_initialize, 2);
    rb_define_method(gRequisition, "width", grequisition_w, 0);
    rb_define_method(gRequisition, "height", grequisition_h, 0);
    rb_define_method(gRequisition, "width=", grequisition_set_w, 1);
    rb_define_method(gRequisition, "height=", grequisition_set_h, 1);
    rb_define_method(gRequisition, "to_a", grequisition_to_a, 0);
    rb_define_method(gRequisition, "to_s", grequisition_to_s, 0);
}
