/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkspan.c -

  $Author: mutoh $
  $Date: 2003/01/12 18:09:10 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(r) ((GdkSpan*)RVAL2BOXED(r, GDK_TYPE_SPAN))

/**********************************/
static GdkSpan*
gdkspan_copy(const GdkSpan *span)
{
    GdkSpan* new = NULL;
    g_return_val_if_fail (span != NULL, NULL);
    new = g_new(GdkSpan, 1);
    *new = *span;
    return new;
}

GType
gdk_span_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GdkSpan",
                    (GBoxedCopyFunc)gdkspan_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
gdkspan_initialize(self, x, y, width)
    VALUE self, x, y, width;
{
    GdkSpan new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);

    G_INITIALIZE(self, &new);
    return Qnil;
}

static VALUE
gdkspan_x(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
gdkspan_y(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
gdkspan_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
gdkspan_set_x(self, x)
    VALUE self, x;
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
gdkspan_set_y(self, y)
    VALUE self, y;
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
gdkspan_set_w(self, width)
    VALUE self, width;
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
gdkspan_to_a(self)
    VALUE self;
{
  GdkSpan* a = _SELF(self);
  return rb_ary_new3(3, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width));
}

void
Init_gtk_gdk_span()
{
    VALUE gdkSpan = G_DEF_CLASS(GDK_TYPE_SPAN, "Span", mGdk);

    rb_define_method(gdkSpan, "initialize", gdkspan_initialize, 3);
    rb_define_method(gdkSpan, "x", gdkspan_x, 0);
    rb_define_method(gdkSpan, "y", gdkspan_y, 0);
    rb_define_method(gdkSpan, "width", gdkspan_w, 0);
    rb_define_method(gdkSpan, "set_x", gdkspan_set_x, 1);
    rb_define_method(gdkSpan, "set_y", gdkspan_set_y, 1);
    rb_define_method(gdkSpan, "set_width", gdkspan_set_w, 1);
    rb_define_method(gdkSpan, "to_a", gdkspan_to_a, 0);

    G_DEF_SETTERS(gdkSpan);
}
