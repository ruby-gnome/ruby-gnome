/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_typeinstance.c -

  $Author: sakai $
  $Date: 2006/05/27 06:48:33 $
  created at: Sat May 27 14:18:55 JST 2006
 
  Copyright (C) 2006  Ruby-GNOME2 Project Team

**********************************************************************/

#include "global.h"

VALUE cInstantiatable;

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
each_cinfo(gpointer instance,
           void (*func)(gpointer instance, const RGObjClassInfo* cinfo, gpointer user_data),
           gpointer user_data)
{
    GType gtype = G_TYPE_FROM_INSTANCE(instance);
    GType* interfaces;
    guint n_interfaces = 0;

    interfaces = g_type_interfaces(gtype, &n_interfaces);
    {
        guint i;
        for (i = 0; i < n_interfaces; i++)
            func(instance, GTYPE2CINFO(interfaces[i]), user_data);
    }

    {
        GType i;
        for (i = gtype; gtype != G_TYPE_INVALID; gtype = g_type_parent(gtype))
            func(instance, GTYPE2CINFO(i), user_data);
    }
}

static void
call_cinfo_free(gpointer instance, const RGObjClassInfo* cinfo, gpointer user_data)
{
    if (cinfo->free) cinfo->free(instance);
}

static void
call_cinfo_mark(gpointer instance, const RGObjClassInfo* cinfo, gpointer user_data)
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
    VALUE obj = rbgobj_ruby_object_from_instance_if_exist(instance);
    rb_gc_mark(obj);
}

/**********************************************************************/

void
Init_gobject_typeinstance()
{
    /* should be renamed to GLib::Instance? */
    cInstantiatable = rb_define_class_under(mGLib, "Instantiatable", rb_cObject);
    rb_extend_object(cInstantiatable, mMetaInterface);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(cInstantiatable, "allocate", instantiatable_s_allocate, 0);
#else
    rb_define_alloc_func(cInstantiatable, (VALUE(*)_((VALUE)))instantiatable_s_allocate);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(cInstantiatable, "new", &generic_s_new, -1);
#endif

    rb_define_method(cInstantiatable, "gtype", instantiatable_get_gtype, 0);
    rb_define_method(cInstantiatable, "clone", instantiatable_clone, 0);
}
