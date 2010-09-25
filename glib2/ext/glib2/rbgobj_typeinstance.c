/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_typeinstance.c -

  $Author: sakai $
  $Date: 2007/07/04 13:13:19 $
  created at: Sat May 27 14:18:55 JST 2006
 
  Copyright (C) 2002-2006  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "rbgprivate.h"

VALUE cInstantiatable;

typedef void (*ClassInfoCallbackFunc) (gpointer instance,
				       const RGObjClassInfo *class_info,
				       gpointer user_data);

static VALUE
instantiatable_s_allocate(klass)
     VALUE klass;
{
     rb_raise(rb_eTypeError, "abstract class");
}

static VALUE
instantiatable_get_gtype(self)
    VALUE self;
{
    return rbgobj_gtype_new(G_TYPE_FROM_INSTANCE(rbgobj_instance_from_ruby_object(self)));
}

static VALUE
instantiatable_clone(self)
    VALUE self;
{
    rb_raise(rb_eTypeError, "can't clone %s", rb_class2name(CLASS_OF(self)));
}

/**********************************************************************/

static void
each_cinfo(gpointer instance, ClassInfoCallbackFunc func, gpointer user_data)
{
    const GType gtype = G_TYPE_FROM_INSTANCE(instance);
    GType* interfaces;
    guint n_interfaces = 0;

    interfaces = g_type_interfaces(gtype, &n_interfaces);
    {
        guint i;
        for (i = 0; i < n_interfaces; i++) {
	    const RGObjClassInfo *info;

	    info = GTYPE2CINFO_NO_CREATE(interfaces[i]);
	    if (info)
		func(instance, info, user_data);
	}
    }
    g_free(interfaces);

    {
        GType type;
        for (type = gtype; type != G_TYPE_INVALID; type = g_type_parent(type)) {
	    const RGObjClassInfo *info;

	    info = GTYPE2CINFO_NO_CREATE(type);
	    if (info)
		func(instance, info, user_data);
	}
    }
}

static void
call_cinfo_free(gpointer instance, const RGObjClassInfo *cinfo, gpointer user_data)
{
    if (cinfo->free) cinfo->free(instance);
}

static void
call_cinfo_mark(gpointer instance, const RGObjClassInfo *cinfo, gpointer user_data)
{
    if (cinfo->mark) cinfo->mark(instance);
}

void
rbgobj_instance_call_cinfo_mark(gpointer instance)
{
    each_cinfo(instance, call_cinfo_mark, NULL);
}

void
rbgobj_instance_call_cinfo_free(gpointer instance)
{
    each_cinfo(instance, call_cinfo_free, NULL);
}

void
rbgobj_gc_mark_instance(gpointer instance)
{
    VALUE obj = rbgobj_ruby_object_from_instance2(instance, FALSE);
    rb_gc_mark(obj);
}

/**********************************************************************/

void
Init_gobject_typeinstance()
{
    /* should be renamed to GLib::Instance? */
    cInstantiatable = rb_define_class_under(mGLib, "Instantiatable", rb_cObject);
    rb_extend_object(cInstantiatable, mMetaInterface);

    rb_define_alloc_func(cInstantiatable, (VALUE(*)_((VALUE)))instantiatable_s_allocate);
    rb_define_method(cInstantiatable, "gtype", instantiatable_get_gtype, 0);
    rb_define_method(cInstantiatable, "clone", instantiatable_clone, 0);
}
