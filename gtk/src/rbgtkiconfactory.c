/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconfactory.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) (GTK_ICON_FACTORY(RVAL2GOBJ(s)))

static VALUE
ifactory_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_icon_factory_new());
    return Qnil;
}

static VALUE
ifactory_add(self, id, icon_set)
    VALUE self, id, icon_set;
{
    gtk_icon_factory_add(_SELF(self), RVAL2CSTR(id),
                         (GtkIconSet*)RVAL2BOXED(icon_set, GTK_TYPE_ICON_SET));
    return self;
}

static VALUE
ifactory_add_default(self)
    VALUE self;
{
    gtk_icon_factory_add_default(_SELF(self));
    return self;
}

static VALUE
ifactory_lookup(self, id)
    VALUE self, id;
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup(_SELF(self), RVAL2CSTR(id));
    return icon_set ? BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET) : Qnil;
}

static VALUE
ifactory_lookup_default(self, id)
    VALUE self, id;
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup_default(RVAL2CSTR(id));
    return icon_set ? BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET) : Qnil;
}

static VALUE
ifactory_remove_default(self)
    VALUE self;
{
    gtk_icon_factory_remove_default(_SELF(self));
    return self;
}

void
Init_icon_factory()
{
    VALUE gIconFactory = G_DEF_CLASS(GTK_TYPE_ICON_FACTORY, "IconFactory", mGtk);

    rb_define_method(gIconFactory, "initialize", ifactory_initialize, 0);
    rb_define_method(gIconFactory, "add", ifactory_add, 2);
    rb_define_method(gIconFactory, "add_default", ifactory_add_default, 0);
    rb_define_method(gIconFactory, "remove_default", ifactory_remove_default, 0);
    rb_define_method(gIconFactory, "lookup", ifactory_lookup, 1);
    
    rb_define_singleton_method(gIconFactory, "lookup_default", ifactory_lookup_default, 1);
}
