/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksizegroup.c -
 
  $Author: ogs $
  $Date: 2002/12/14 14:40:39 $

  Copyright (C) 2002 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(self) (GTK_SIZE_GROUP(RVAL2GOBJ(self)))

static VALUE
sizegrp_initialize(self, mode)
    VALUE self;
{
    G_INITIALIZE(self, gtk_size_group_new(FIX2INT(mode)));
    return Qnil;
}

static VALUE
sizegrp_set_mode(self, mode)
    VALUE self, mode;
{
    gtk_size_group_set_mode(_SELF(self), FIX2INT(mode));
    return self;
}

static VALUE
sizegrp_get_mode(self)
    VALUE self;
{
    return INT2FIX(gtk_size_group_get_mode(_SELF(self)));
}

static VALUE
sizegrp_add_widget(self, widget)
    VALUE self, widget;
{
    gtk_size_group_add_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

static VALUE
sizegrp_remove_widget(self, widget)
    VALUE self, widget;
{
    gtk_size_group_remove_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

void
Init_sizegrp()
{
    VALUE gSizeGroup = G_DEF_CLASS(GTK_TYPE_SIZE_GROUP, "SizeGroup", mGtk);

    rb_define_method(gSizeGroup, "initialize", sizegrp_initialize, 1);

    rb_define_method(gSizeGroup, "set_mode", sizegrp_set_mode, 1);
    rb_define_method(gSizeGroup, "mode", sizegrp_get_mode, 0);
    rb_define_method(gSizeGroup, "add_widget", sizegrp_add_widget, 1);
    rb_define_method(gSizeGroup, "remove_widget", sizegrp_remove_widget, 1);

    G_DEF_SETTERS(gSizeGroup);

    /* GtkSizeGroupMode */
    rb_define_const(gSizeGroup, "NONE", INT2FIX(GTK_SIZE_GROUP_NONE));
    rb_define_const(gSizeGroup, "HORIZONTAL", INT2FIX(GTK_SIZE_GROUP_HORIZONTAL));
    rb_define_const(gSizeGroup, "VERTICAL", INT2FIX(GTK_SIZE_GROUP_VERTICAL));
    rb_define_const(gSizeGroup, "BOTH", INT2FIX(GTK_SIZE_GROUP_BOTH));
}

