/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextattributes.c -
 
  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) ((GtkTextAttributes*)RVAL2BOXED(s, GTK_TYPE_TEXT_ATTRIBUTES))

static VALUE
txt_attr_initialize(self)
    VALUE self;
{
    GtkTextAttributes *attr;

    attr = gtk_text_attributes_new();
    G_INITIALIZE(self, attr);
    return Qnil;
}

static VALUE
txt_attr_copy(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_text_attributes_copy(_SELF(self)),
                      GTK_TYPE_TEXT_ATTRIBUTES);
}

static VALUE
txt_attr_copy_values(self, dest)
    VALUE self;
{
    gtk_text_attributes_copy_values(_SELF(self), _SELF(dest));
    return self;
}

void
Init_txt_attr()
{
    VALUE gTextAttributes = G_DEF_CLASS(GTK_TYPE_TEXT_ATTRIBUTES, "TextAttributes", mGtk);
  
    rb_define_method(gTextAttributes, "initialize", txt_attr_initialize, 0);
    rb_define_method(gTextAttributes, "copy", txt_attr_copy, 0);
    rb_define_method(gTextAttributes, "copy_values", txt_attr_copy_values, 1);
}
