
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

GType gda_datasource_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaDataSource",
            (GBoxedCopyFunc)gda_config_copy_data_source_info,
            (GBoxedFreeFunc)gda_config_free_data_source_info);
    }
    return our_type;
}

static VALUE rb_gda_get_datasources(self)
    VALUE self;
{
    GList *list, *node;
    VALUE arr;

    list = gda_config_get_data_source_list();
    arr = rb_ary_new();

    for (node = g_list_first(list); 
         node != NULL;
         node = g_list_next(node))
    {
        VALUE v = RGDA_DATASOURCE_NEW(node->data);
        //rb_warn("name is %s", RGDA_DATASOURCE(v)->name);
        rb_ary_push(arr, v); 
    }
    gda_config_free_data_source_list(list);

    return arr;
}

static VALUE rb_gda_datasource_each(self)
    VALUE self;
{
    rb_ary_each(rb_gda_get_datasources(self));
    return Qnil;
}

static VALUE rb_gda_datasource_find(self, name)
    VALUE self, name;
{
    GdaDataSourceInfo *info = gda_config_find_data_source(RVAL2CSTR(name));
    return info != NULL
        ? RGDA_DATASOURCE_NEW(info)
        : Qnil;
}

static VALUE rb_gda_datasource_model(self)
    VALUE self;
{
    GdaDataModel *model = gda_config_get_data_source_model();
    return model != NULL
        ? RGDA_DATAMODEL_NEW(model)
	: Qnil;
}

static VALUE rb_gda_datasource_new(self, name, provider, cnc_string, 
                                   description, username, password)
    VALUE self, name, provider, cnc_string, 
          description, username, password;
{
    GdaDataSourceInfo *new = g_new(GdaDataSourceInfo, sizeof(GdaDataSourceInfo));
    new->name = RVAL2CSTR(name);
    new->provider = RVAL2CSTR(provider);
    new->cnc_string = RVAL2CSTR(cnc_string);
    new->description = RVAL2CSTR(description);
    new->username = RVAL2CSTR(username);
    new->password = NIL_P(password) ? NULL : RVAL2CSTR(password);
    G_INITIALIZE(self, new);
    return Qnil;
}

static VALUE rb_gda_datasource_save(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
#if defined(HAVE_GDA_CONFIG_SAVE_DATA_SOURCE_INFO)
    gda_config_save_data_source_info(info);
#else   /* <= 0.91.0 */
    gda_config_save_data_source(info->name,
                                info->provider,
                                info->cnc_string,
                                info->description,
                                info->username,
                                info->password);
#endif
    return self;

}

static VALUE rb_gda_datasource_remove(self)
    VALUE self;
{
    gda_config_remove_data_source(RGDA_DATASOURCE(self)->name);
    return self;
}

static VALUE rb_gda_datasource_get_name(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->name);
}

static VALUE rb_gda_datasource_get_provider(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->provider);
}

static VALUE rb_gda_datasource_get_cnc_string(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->cnc_string);
}

static VALUE rb_gda_datasource_get_description(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->description);
}

static VALUE rb_gda_datasource_get_username(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->username);
}

static VALUE rb_gda_datasource_get_password(self)
    VALUE self;
{
    GdaDataSourceInfo *info = RGDA_DATASOURCE(self);
    return CSTR2RVAL(info->password);
}

static VALUE rb_gda_datasource_is_equal(self, other_data)
    VALUE self, other_data;
{
    return NIL_P(other_data)
        ? Qfalse
        : rb_equal(rb_gda_datasource_get_name(self),
                   rb_gda_datasource_get_name(other_data)); 
}

void Init_gda_datasource(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATASOURCE, "DataSource", mGda);

    rb_define_singleton_method(c, "datasources", rb_gda_get_datasources,  0);
    rb_define_singleton_method(c, "each",        rb_gda_datasource_each,  0);
    rb_define_singleton_method(c, "find",        rb_gda_datasource_find,  1);
    rb_define_singleton_method(c, "model",       rb_gda_datasource_model, 0);

    rb_define_method(c, "initialize", rb_gda_datasource_new, 6);
    
    rb_define_method(c, "save",   rb_gda_datasource_save,   0);
    rb_define_method(c, "remove", rb_gda_datasource_remove, 0);

    rb_define_method(c, "name",        rb_gda_datasource_get_name,        0);
    rb_define_method(c, "provider",    rb_gda_datasource_get_provider,    0);
    rb_define_method(c, "cnc_string",  rb_gda_datasource_get_cnc_string,  0);
    rb_define_method(c, "description", rb_gda_datasource_get_description, 0);
    rb_define_method(c, "username",    rb_gda_datasource_get_username,    0);
    rb_define_method(c, "password",    rb_gda_datasource_get_password,    0);

    rb_define_method(c, "==", rb_gda_datasource_is_equal, 1);
}

