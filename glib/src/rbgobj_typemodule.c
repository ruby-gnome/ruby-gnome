/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_typemodule.c -

  $Author: sakai $
  $Date: 2002/07/27 14:46:35 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

VALUE rbgobj_cTypeModule;

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
    StringValue(name);
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
    static RGObjClassInfo cinfo;

    rbgobj_cTypeModule = rb_define_class_under(mGLib, "TypeModule", rbgobj_cGObject);

    cinfo.klass = rbgobj_cTypeModule;
    cinfo.gtype = G_TYPE_TYPE_MODULE;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgobj_register_class(&cinfo);

    rb_define_method(rbgobj_cTypeModule, "use", use, 0);
    rb_define_method(rbgobj_cTypeModule, "unuse", unuse, 0);

    rb_define_method(rbgobj_cTypeModule, "name", get_name, 0);
    rb_define_method(rbgobj_cTypeModule, "name", set_name, 1);
}
