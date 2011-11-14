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

#define RG_TARGET_NAMESPACE cIconFactory
#define _SELF(s) (GTK_ICON_FACTORY(RVAL2GOBJ(s)))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_icon_factory_new());
    return Qnil;
}

static VALUE
rg_add(VALUE self, VALUE id, VALUE icon_set)
{
    gtk_icon_factory_add(_SELF(self), RVAL2CSTR(id),
                         (GtkIconSet*)RVAL2BOXED(icon_set, GTK_TYPE_ICON_SET));
    return self;
}

static VALUE
rg_add_default(VALUE self)
{
    gtk_icon_factory_add_default(_SELF(self));
    return self;
}

static VALUE
rg_lookup(VALUE self, VALUE id)
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup(_SELF(self), RVAL2CSTR(id));
    return BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET);
}

static VALUE
rg_s_lookup_default(G_GNUC_UNUSED VALUE self, VALUE id)
{
    GtkIconSet *icon_set;

    icon_set = gtk_icon_factory_lookup_default(RVAL2CSTR(id));
    return BOXED2RVAL(icon_set, GTK_TYPE_ICON_SET);
}

static VALUE
rg_remove_default(VALUE self)
{
    gtk_icon_factory_remove_default(_SELF(self));
    return self;
}

void
Init_gtk_icon_factory(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ICON_FACTORY, "IconFactory", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add, 2);
    RG_DEF_METHOD(add_default, 0);
    RG_DEF_METHOD(remove_default, 0);
    RG_DEF_METHOD(lookup, 1);

    RG_DEF_SMETHOD(lookup_default, 1);
}
