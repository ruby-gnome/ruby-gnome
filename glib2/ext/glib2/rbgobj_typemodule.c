/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE cTypeModule

static VALUE
rg_use(VALUE self)
{
    return CBOOL2RVAL(g_type_module_use(G_TYPE_MODULE(RVAL2GOBJ(self))));
}

static VALUE
rg_unuse(VALUE self)
{
    g_type_module_unuse(G_TYPE_MODULE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_name(VALUE self)
{
    return rb_str_new2(G_TYPE_MODULE(RVAL2GOBJ(self))->name);
}

static VALUE
rg_operator_set_name(VALUE self, VALUE name)
{
    g_type_module_set_name(G_TYPE_MODULE(RVAL2GOBJ(self)), StringValuePtr(name));
    return name;
}

#if 0
GType    g_type_module_register_type (GTypeModule     *module,
                      GType            parent_type,
                      const gchar     *type_name,
                      const GTypeInfo *type_info,
                      GTypeFlags       flags);
void     g_type_module_add_interface (GTypeModule           *module,
                      GType                  instance_type,
                      GType                  interface_type,
                      const GInterfaceInfo  *interface_info);
#endif

void
Init_gobject_gtypemodule(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_TYPE_MODULE, "TypeModule", mGLib);
    RG_DEF_METHOD(use, 0);
    RG_DEF_METHOD(unuse, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD_OPERATOR("name=", set_name, 1);
}