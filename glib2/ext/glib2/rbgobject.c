/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2006  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "ruby.h"
#include "rbgprivate.h"
#include <ctype.h>

#include "rbgprivate.h"

static ID id_relatives;
static ID id_delete;
static ID id_module_eval;

ID rbgobj_id_children;

/**********************************************************************/

void
rbgobj_initialize_object(VALUE obj, gpointer cobj)
{
    GType type;
    GType parent_type;

    if (!cobj)
        rb_raise(rb_eRuntimeError, "failed to initialize");

    type = RVAL2GTYPE(obj);

    for (parent_type = type;
         parent_type != G_TYPE_INVALID;
         parent_type = g_type_parent(parent_type)) {

         if (rbgobj_convert_initialize(parent_type, obj, cobj))
             return;
    }

    type = G_TYPE_FUNDAMENTAL(type);
    switch (type){
    case G_TYPE_OBJECT:
        rbgobj_gobject_initialize(obj, cobj);
        break;
    case G_TYPE_PARAM:
        rbgobj_param_spec_initialize(obj, cobj);
        break;
    case G_TYPE_BOXED:
        rbgobj_boxed_initialize(obj, cobj);
        break;
    default:
        rbgobj_convert_initialize(type, obj, cobj);
        break;
    }
}

gpointer
rbgobj_instance_from_ruby_object(VALUE obj)
{
    GType type;
    GType fundamental_type;

    if (NIL_P(obj))
        return NULL;

    type = RVAL2GTYPE(obj);
    if (rbgobj_convert_has_type(type)) {
        gpointer instance;
        if (rbgobj_convert_robj2instance(type, obj, &instance))
            return instance;
    }

    fundamental_type = G_TYPE_FUNDAMENTAL(type);
    switch (fundamental_type) {
    case G_TYPE_OBJECT:
        return rbgobj_get_gobject(obj);
    case G_TYPE_BOXED:
        return rbgobj_boxed_get(obj, type);
    case G_TYPE_PARAM:
        return rbgobj_get_param_spec(obj);
    default:
      {
        gpointer instance;
        if (!rbgobj_convert_robj2instance(fundamental_type, obj, &instance)) {
            rb_raise(rb_eTypeError, "%s isn't supported",
                     rb_class2name(CLASS_OF(obj)));
        }
        return instance;
      }
    }
}

VALUE
rbgobj_ruby_object_from_instance(gpointer instance)
{
    return rbgobj_ruby_object_from_instance2(instance, TRUE);
}

VALUE
rbgobj_ruby_object_from_instance2(gpointer instance, gboolean alloc)
{
    VALUE object;
    GType type;

    if (!instance)
    	return Qnil;

    type = G_TYPE_FROM_INSTANCE(instance);
    if (alloc) {
        GType parent_type;
        for (parent_type = type;
             parent_type != G_TYPE_INVALID;
             parent_type = g_type_parent(parent_type)) {
            if (rbgobj_convert_instance2robj(parent_type, instance, &object))
                return object;
        }
    }

    switch (G_TYPE_FUNDAMENTAL(type)) {
    case G_TYPE_OBJECT:
        return rbgobj_get_ruby_object_from_gobject(instance, alloc);
    case G_TYPE_PARAM:
        return rbgobj_get_ruby_object_from_param_spec(instance, alloc);
    default:
        if (alloc) {
            rb_raise(rb_eTypeError, "%s isn't supported", g_type_name(type));
        } else {
            return Qnil;
        }
    }
}

VALUE
rbgobj_ruby_object_from_instance_with_unref(gpointer instance)
{
    VALUE result = rbgobj_ruby_object_from_instance(instance);
    if (!NIL_P(result)) {
        GType type;

        type = G_TYPE_FROM_INSTANCE(instance);
        if (!rbgobj_convert_unref(type, instance)) {
            type = G_TYPE_FUNDAMENTAL(type);
            switch (type) {
              case G_TYPE_OBJECT:
                g_object_unref(instance);
                break;
              default:
                rbgobj_convert_unref(type, instance);
                break;
            }
        }
    }
    return result;
}

/**********************************************************************/

void
rbgobj_add_relative(VALUE obj, VALUE relative)
{
    VALUE hash = Qnil;

    if (RVAL2CBOOL(rb_ivar_defined(obj, id_relatives)))
        hash = rb_ivar_get(obj, id_relatives);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        hash = rb_hash_new();
        rb_ivar_set(obj, id_relatives, hash);
    }
    rb_hash_aset(hash, relative, Qnil);
}

void
rbgobj_invalidate_relatives(VALUE obj)
{
    if (RVAL2CBOOL(rb_ivar_defined(obj, id_relatives)))
        rb_ivar_set(obj, id_relatives, Qnil);
    if (RVAL2CBOOL(rb_ivar_defined(obj, rbgobj_id_children)))
        rb_ivar_set(obj, rbgobj_id_children, Qnil);
}

void
rbgobj_add_relative_removable(VALUE obj, VALUE relative, ID obj_ivar_id, VALUE hash_key)
{
    VALUE hash = Qnil;

    if (RVAL2CBOOL(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        hash = rb_hash_new();
        rb_ivar_set(obj, obj_ivar_id, hash);
    }
    rb_hash_aset(hash, hash_key, relative);
}

VALUE
rbgobj_get_relative_removable(VALUE obj, ID obj_ivar_id, VALUE hash_key)
{
    VALUE hash = Qnil;

    if (RVAL2CBOOL(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        return Qnil;
    }
    return rb_hash_aref(hash, hash_key);
}

void
rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key)
{
    VALUE hash = Qnil;

    if (RVAL2CBOOL(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        /* should not happen. */
    } else {
        rb_funcall(hash, id_delete, 1, hash_key);
    }
}

void
rbgobj_remove_relative_all(VALUE obj, ID obj_ivar_id)
{
    rb_ivar_set(obj, obj_ivar_id, Qnil);
}

/**********************************************************************/

static GHashTable* prop_exclude_list;

#define IS_FLAG(bitmask, flag) (((bitmask) & (flag)) == (flag))

void
rbgobj_define_property_accessors(VALUE klass)
{
    GType gtype;
    GParamSpec** pspecs = NULL;
    guint i;
    GString* source;
    guint n_properties = 0;
    gtype = CLASS2GTYPE(klass);

    if (G_TYPE_IS_INTERFACE(gtype)){
#if GLIB_CHECK_VERSION(2,4,0)
        gpointer iface = g_type_default_interface_ref(gtype);
        pspecs = g_object_interface_list_properties(iface, &n_properties);
        g_type_default_interface_unref(iface);
#endif
    } else {
        GObjectClass* oclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
        pspecs = g_object_class_list_properties(oclass, &n_properties);
        g_type_class_unref(oclass);
    }

    if (n_properties == 0)
        return;

    source = g_string_new(NULL);
    for (i = 0; i < n_properties; i++){
        GParamSpec* pspec = pspecs[i];
        char* buf;
        char* prop_name;
        char* p;

        if (pspec->owner_type != gtype)
            continue;

        buf = g_strdup(pspec->name);
        for (p = buf; *p; p++)
            if (*p == '-')
                *p = '_';

        if (!strncmp(buf, "is_", 3))
            prop_name = buf + 3;
        else
            prop_name = buf;

        if (g_hash_table_lookup(prop_exclude_list, prop_name)){
            g_free(buf);
            continue;
        }

        if (pspec->flags & G_PARAM_READABLE){
            g_string_append_printf(
                source, 
                "def %s%s; get_property('%s'); end\n",
                prop_name,
                (G_PARAM_SPEC_VALUE_TYPE(pspec) == G_TYPE_BOOLEAN) ? "?" : "",
                pspec->name);
        }

        if (IS_FLAG(pspec->flags, G_PARAM_WRITABLE) && !IS_FLAG(pspec->flags, G_PARAM_CONSTRUCT_ONLY)){
            g_string_append_printf(source,
                "def set_%s(val); set_property('%s', val); end\n",
                prop_name, pspec->name);
#ifdef HAVE_NODE_ATTRASGN
            g_string_append_printf(source, "alias %s= set_%s\n",
                                   prop_name, prop_name);
#else
            g_string_append_printf(source,
                "def %s=(val); set_property('%s', val); val; end\n",
                prop_name, pspec->name);
#endif
        }

        g_free(buf);
    }

    if (source->len > 0)
        rb_funcall(klass, id_module_eval, 1, rb_str_new2(source->str));
    g_string_free(source, TRUE);
}

/**********************************************************************/

void
Init_gobject(void)
{
    /* Not defined properties. They are already used as methods of Object */
    prop_exclude_list = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(prop_exclude_list, (gpointer)"class", (gpointer)"class");
    g_hash_table_insert(prop_exclude_list, (gpointer)"clone", (gpointer)"clone");
    g_hash_table_insert(prop_exclude_list, (gpointer)"dup", (gpointer)"dup");
    g_hash_table_insert(prop_exclude_list, (gpointer)"extend", (gpointer)"extend");
    g_hash_table_insert(prop_exclude_list, (gpointer)"freeze", (gpointer)"freeze");
    g_hash_table_insert(prop_exclude_list, (gpointer)"hash", (gpointer)"hash");
    g_hash_table_insert(prop_exclude_list, (gpointer)"method", (gpointer)"method");
    g_hash_table_insert(prop_exclude_list, (gpointer)"methods", (gpointer)"methods");
    g_hash_table_insert(prop_exclude_list, (gpointer)"object_id", (gpointer)"object_id");
    g_hash_table_insert(prop_exclude_list, (gpointer)"taint", (gpointer)"taint");
    g_hash_table_insert(prop_exclude_list, (gpointer)"untaint", (gpointer)"untaint");

    /* IDs */
    id_relatives = rb_intern("__relatives__");
    id_delete = rb_intern("delete");
    id_module_eval = rb_intern("module_eval");

    rbgobj_id_children = rb_intern("__stored_children__");

    Init_gobject_convert();

    Init_gobject_gtype();
    Init_gobject_typeinterface();
    Init_gobject_typeinstance();
    Init_gobject_gvalue();
    Init_gobject_gvaluetypes();
    Init_gobject_gboxed();
#if GLIB_CHECK_VERSION(2,6,0)
    Init_gobject_gstrv();
#endif
    Init_gobject_value_array();
    Init_gobject_genumflags();
    Init_gobject_gparam();
    Init_gobject_gparamspecs();
    Init_gobject_gclosure();
    Init_gobject_gobject();
    Init_gobject_gsignal();

    Init_gobject_gtypeplugin();
    Init_gobject_gtypemodule();
}
