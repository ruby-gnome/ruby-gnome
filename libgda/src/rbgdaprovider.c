
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/Gda.
 *
 * Ruby/Gda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Gda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Gda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

static GdaProviderInfo* provider_copy(const GdaProviderInfo* provider) {
    GdaProviderInfo* new_provider;
    g_return_val_if_fail (provider != NULL, NULL);
    new_provider = g_new(GdaProviderInfo, sizeof(GdaProviderInfo));
    new_provider->id = g_strdup(provider->id); 
    new_provider->location = g_strdup(provider->location);
    new_provider->description = g_strdup(provider->description);
    /* TODO should copy provider->gda_params */
    new_provider->gda_params = NULL;
    return new_provider;
}

GType gda_provider_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaProvider",
            (GBoxedCopyFunc)provider_copy,
            (GBoxedFreeFunc)gda_config_free_provider_info);
    }
    return our_type;
}

static VALUE rb_gda_get_providers(self)
    VALUE self;
{
    GList *list, *node;
    VALUE arr;

    list = gda_config_get_provider_list();
    arr = rb_ary_new();

    for (node = g_list_first(list); 
         node != NULL;
         node = g_list_next(node))
    {
        rb_ary_push(arr, RGDA_PROVIDER_NEW(node->data));
    }
    gda_config_free_provider_list(list);

    return arr;
}

static VALUE rb_gda_provider_each(self)
    VALUE self;
{
    rb_ary_each(rb_gda_get_providers(self));
    return Qnil;
}

static VALUE rb_gda_provider_get_by_name(self, name)
    VALUE self, name;
{
    GdaProviderInfo *info = gda_config_get_provider_by_name(RVAL2CSTR(name));
    return info != NULL
        ? RGDA_PROVIDER_NEW(info)
        : Qnil;
}

static VALUE rb_gda_provider_get_id(self)
    VALUE self;
{
    GdaProviderInfo *info = RGDA_PROVIDER(self);
    return CSTR2RVAL(info->id);
}

static VALUE rb_gda_provider_get_location(self)
    VALUE self;
{
    GdaProviderInfo *info = RGDA_PROVIDER(self);
    return CSTR2RVAL(info->location);
}

static VALUE rb_gda_provider_get_description(self)
    VALUE self;
{
    GdaProviderInfo *info = RGDA_PROVIDER(self);
    return CSTR2RVAL(info->description);
}

void Init_gda_provider(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_PROVIDER, "Provider", mGda);

    rb_define_singleton_method(c, "providers", rb_gda_get_providers, 0);
    rb_define_singleton_method(c, "each",      rb_gda_provider_each, 0);

    rb_define_singleton_method(c, "get_by_name", rb_gda_provider_get_by_name, 1);

    rb_define_method(c, "prov_id",     rb_gda_provider_get_id,          0);
    rb_define_method(c, "location",    rb_gda_provider_get_location,    0);
    rb_define_method(c, "description", rb_gda_provider_get_description, 0);
}

