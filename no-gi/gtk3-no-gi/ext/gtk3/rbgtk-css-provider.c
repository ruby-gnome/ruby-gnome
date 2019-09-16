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

#define RG_TARGET_NAMESPACE cCssProvider
#define _SELF(self) (RVAL2GTKCSSPROVIDER(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_css_provider_new());

    return Qnil;
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_css_provider_get_default());
}

static VALUE
rg_s_get_named(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE name, variant;
    GtkCssProvider *provider;

    rb_scan_args(argc, argv, "11", &name, &variant);
    provider = gtk_css_provider_get_named(RVAL2CSTR(name),
                                          RVAL2CSTR_ACCEPT_NIL(variant));

    return GOBJ2RVAL(provider);
}

static VALUE
rg_load(VALUE self, VALUE arg)
{
    gboolean result;
    GError *error = NULL;

    if (TYPE(arg) == T_HASH) {
        VALUE data, file, path;
        rbg_scan_options(arg,
                         "data", &data,
                         "file", &file,
                         "path", &path,
                         NULL);
        if (!NIL_P(data)) {
            StringValue(data);
            result = gtk_css_provider_load_from_data(_SELF(self),
                                                     RSTRING_PTR(data),
                                                     RSTRING_LEN(data),
                                                     &error);
        } else if (!NIL_P(file)) {
            result = gtk_css_provider_load_from_file(_SELF(self),
                                                     RVAL2GFILE(file),
                                                     &error);
        } else if (!NIL_P(path)) {
            result = gtk_css_provider_load_from_path(_SELF(self),
                                                     RVAL2CSTR(path),
                                                     &error);
        } else {
            rb_raise(rb_eArgError, "Invalid arguments.");
        }
    } else {
        result = gtk_css_provider_load_from_file(_SELF(self),
                                                 RVAL2GFILE(arg),
                                                 &error);
    }
    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rg_to_s(VALUE self)
{
    return CSTR2RVAL(gtk_css_provider_to_string(_SELF(self)));
}

void
Init_gtk_cssprovider(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_CSS_PROVIDER, "CssProvider", mGtk);

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_SMETHOD(get_named, -1);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(load, 1);
    RG_DEF_METHOD(to_s, 0);
}
