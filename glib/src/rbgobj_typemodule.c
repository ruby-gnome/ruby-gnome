/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_typemodule.c -

  $Author: sakai $
  $Date: 2007/07/01 17:43:55 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
use(self)
   VALUE self;
{
    return g_type_module_use(G_TYPE_MODULE(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

static VALUE
unuse(self)
   VALUE self;
{
    g_type_module_unuse(G_TYPE_MODULE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
get_name(self)
   VALUE self;
{
    return rb_str_new2(G_TYPE_MODULE(RVAL2GOBJ(self))->name);
}

static VALUE
set_name(self, name)
   VALUE self, name;
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
Init_gobject_gtypemodule()
{
    VALUE cTypeModule = G_DEF_CLASS(G_TYPE_TYPE_MODULE, "TypeModule", mGLib);
    rb_define_method(cTypeModule, "use", use, 0);
    rb_define_method(cTypeModule, "unuse", unuse, 0);
    rb_define_method(cTypeModule, "name", get_name, 0);
    rb_define_method(cTypeModule, "name=", set_name, 1);
}
