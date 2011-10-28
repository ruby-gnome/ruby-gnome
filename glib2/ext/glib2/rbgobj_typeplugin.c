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

#define RG_TARGET_NAMESPACE mTypePlugin

static VALUE
rg_use(VALUE self)
{
    g_type_plugin_use(G_TYPE_PLUGIN(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_unuse(VALUE self)
{
    g_type_plugin_unuse(G_TYPE_PLUGIN(RVAL2GOBJ(self)));
    return self;
}

#if 0
void    g_type_plugin_complete_type_info   (GTypePlugin     *plugin,
                         GType            g_type,
                         GTypeInfo       *info,
                         GTypeValueTable *value_table);
void    g_type_plugin_complete_interface_info  (GTypePlugin     *plugin,
                         GType            interface_type,
                         GType            instance_type,
                         GInterfaceInfo  *info);
#endif

void
Init_gobject_gtypeplugin(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_TYPE_PLUGIN, "TypePlugin", mGLib);
    RG_DEF_METHOD(use, 0);
    RG_DEF_METHOD(unuse, 0);
}