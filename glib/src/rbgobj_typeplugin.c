/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_typeplugin.c -

  $Author: sakai $
  $Date: 2002/09/01 13:19:21 $
  created at: Mon Aug  5 00:42:09 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
use(self)
   VALUE self;
{
    g_type_plugin_use(G_TYPE_PLUGIN(RVAL2GOBJ(self)));
    return self;
}

static VALUE
unuse(self)
   VALUE self;
{
    g_type_plugin_unuse(G_TYPE_PLUGIN(RVAL2GOBJ(self)));
    return self;
}

#if 0
void	g_type_plugin_complete_type_info	(GTypePlugin     *plugin,
						 GType            g_type,
						 GTypeInfo       *info,
						 GTypeValueTable *value_table);
void	g_type_plugin_complete_interface_info	(GTypePlugin     *plugin,
						 GType            interface_type,
						 GType            instance_type,
						 GInterfaceInfo  *info);
#endif

void
Init_gobject_gtypeplugin()
{
    VALUE iTypePlugin = G_DEF_INTERFACE(G_TYPE_TYPE_PLUGIN, "TypePlugin", mGLib);
    rb_define_method(iTypePlugin, "use", use, 0);
    rb_define_method(iTypePlugin, "unuse", unuse, 0);
}
