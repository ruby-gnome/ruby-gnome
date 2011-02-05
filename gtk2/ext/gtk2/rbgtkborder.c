/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkborder.c -

  $Author: mutoh $
  $Date: 2004/05/24 17:22:58 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) ((GtkBorder*)RVAL2BOXED(self, GTK_TYPE_BORDER))

#define ATTR_INT(name)\
static VALUE \
border_int_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
border_int_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

ATTR_INT(left);
ATTR_INT(right);
ATTR_INT(top);
ATTR_INT(bottom);

#define DEFINE_ACCESSOR(gt, type, name)                                  \
    rb_define_method(gt, G_STRINGIFY(name), border_ ## type ## _## name, 0);\
    rb_define_method(gt, G_STRINGIFY(set_ ## name), border_ ## type ## _set_## name, 1);

static VALUE
border_initialize(VALUE self, VALUE left, VALUE right, VALUE top, VALUE bottom)
{
    GtkBorder border;
    border.left = NUM2INT(left);
    border.right = NUM2INT(right);
    border.top = NUM2INT(top);
    border.bottom = NUM2INT(bottom);

    G_INITIALIZE(self, &border);
    return Qnil;
}

static VALUE
border_to_a(VALUE self)
{
    GtkBorder* border = _SELF(self);
    return rb_ary_new3(4, INT2NUM(border->left), INT2NUM(border->right),
                       INT2NUM(border->top), INT2NUM(border->bottom));
}

void 
Init_gtk_border()
{
    VALUE gBorder = G_DEF_CLASS(GTK_TYPE_BORDER, "Border", mGtk);

    rb_define_method(gBorder, "initialize", border_initialize, 4);
    rb_define_method(gBorder, "to_a", border_to_a, 0);

    DEFINE_ACCESSOR(gBorder, int, left);
    DEFINE_ACCESSOR(gBorder, int, right);
    DEFINE_ACCESSOR(gBorder, int, top);
    DEFINE_ACCESSOR(gBorder, int, bottom);

    G_DEF_SETTERS(gBorder);
}
