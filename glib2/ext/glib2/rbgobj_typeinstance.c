/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cInstantiatable

VALUE RG_TARGET_NAMESPACE;

typedef void (*ClassInfoCallbackFunc) (gpointer instance,
                                       const RGObjClassInfo *class_info,
                                       gpointer user_data);

static G_GNUC_NORETURN VALUE
instantiatable_s_allocate(G_GNUC_UNUSED VALUE klass)
{
     rb_raise(rb_eTypeError, "abstract class");
}

static VALUE
rg_gtype(VALUE self)
{
    return rbgobj_gtype_new(G_TYPE_FROM_INSTANCE(rbgobj_instance_from_ruby_object(self)));
}

static G_GNUC_NORETURN VALUE
rg_clone(VALUE self)
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
call_cinfo_free(gpointer instance, const RGObjClassInfo *cinfo, G_GNUC_UNUSED gpointer user_data)
{
    if (cinfo->free) cinfo->free(instance);
}

static void
call_cinfo_mark(gpointer instance, const RGObjClassInfo *cinfo, G_GNUC_UNUSED gpointer user_data)
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

gboolean
rbgobj_gc_mark_instance(gpointer instance)
{
    VALUE obj = rbgobj_ruby_object_from_instance2(instance, FALSE);

    if (NIL_P(obj)) {
        return FALSE;
    }

    rb_gc_mark(obj);
    return TRUE;
}

/**********************************************************************/

void
Init_gobject_typeinstance(void)
{
    /* should be renamed to GLib::Instance? */
    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Instantiatable", rb_cObject);
    rb_extend_object(RG_TARGET_NAMESPACE, mMetaInterface);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, (VALUE(*)_((VALUE)))instantiatable_s_allocate);
    RG_DEF_METHOD(gtype, 0);
    RG_DEF_METHOD(clone, 0);
}
