/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define _SELF(s) (GTK_ICON_FACTORY(RVAL2GOBJ(s)))

static VALUE
ifactory_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_icon_factory_new());
    return Qnil;
}

static VALUE
ifactory_add(VALUE self, VALUE id, VALUE icon_set)
{
    gtk_icon_factory_add(_SELF(self), RVAL2CSTR(id),
                         (GtkIconSet*)RVAL2BOXED(icon_set, GTK_TYPE_ICON_SET));
    return self;
}

static VALUE
ifactory_add_default(VALUE self)
{
    gtk_icon_factory_add_default(_SELF(self));
    return self;
}

static VALUE
ifactory_lookup(VALUE self, VALUE id)
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup(_SELF(self), RVAL2CSTR(id));
    return BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET);
}

static VALUE
ifactory_lookup_default(G_GNUC_UNUSED VALUE self, VALUE id)
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup_default(RVAL2CSTR(id));
    return BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET);
}

static VALUE
ifactory_remove_default(VALUE self)
{
    gtk_icon_factory_remove_default(_SELF(self));
    return self;
}

void
Init_gtk_icon_factory(void)
{
    VALUE gIconFactory = G_DEF_CLASS(GTK_TYPE_ICON_FACTORY, "IconFactory", mGtk);

    rb_define_method(gIconFactory, "initialize", ifactory_initialize, 0);
    rb_define_method(gIconFactory, "add", ifactory_add, 2);
    rb_define_method(gIconFactory, "add_default", ifactory_add_default, 0);
    rb_define_method(gIconFactory, "remove_default", ifactory_remove_default, 0);
    rb_define_method(gIconFactory, "lookup", ifactory_lookup, 1);
    
    rb_define_singleton_method(gIconFactory, "lookup_default", ifactory_lookup_default, 1);
}
