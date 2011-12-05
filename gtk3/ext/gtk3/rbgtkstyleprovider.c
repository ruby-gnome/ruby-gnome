/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE mStyleProvider
#define _SELF(self) (RVAL2GTKSTYLEPROVIDER(self))

static VALUE
rg_get_icon_factory(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_style_provider_get_icon_factory(_SELF(self),
                                                         RVAL2BOXED(path, GTK_TYPE_WIDGET_PATH)));
}

static VALUE
rg_get_style(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_style_provider_get_style(_SELF(self),
                                                  RVAL2BOXED(path, GTK_TYPE_WIDGET_PATH)));
}

static VALUE
rg_get_style_property(VALUE self, VALUE path, VALUE state, VALUE pspec)
{
    GValue value = G_VALUE_INIT;
    gboolean result;
    VALUE ret = Qnil;

    result = gtk_style_provider_get_style_property(_SELF(self),
                                                   RVAL2BOXED(path, GTK_TYPE_WIDGET_PATH),
                                                   RVAL2GFLAGS(state, GTK_TYPE_STATE_FLAGS),
                                                   RVAL2GPARAMSPEC(pspec),
                                                   &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value); /* TODO: needless? */
    } 

    return ret;
}

void
Init_gtk_styleprovider(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_STYLE_PROVIDER, "StyleProvider", mGtk);

    RG_DEF_METHOD(get_icon_factory, 1);
    RG_DEF_METHOD(get_style, 1);
    RG_DEF_METHOD(get_style_property, 3);
}
