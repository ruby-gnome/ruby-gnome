/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2025  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE mMetaInterface

VALUE rbgobj_mInterface;
VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_append_features(G_GNUC_UNUSED VALUE self, VALUE klass)
{
    if (!RVAL2CBOOL(rb_class_inherited_p(klass, cInstantiatable)))
        rb_raise(rb_eTypeError, "Not a subclass of GLib::Instantiatable");
    return rb_call_super(1, &klass);
}

static void
interface_init(G_GNUC_UNUSED gpointer g_iface,
               G_GNUC_UNUSED gpointer iface_data)
{
}

static VALUE
rg_included(VALUE self, VALUE class_or_module)
{
    const GInterfaceInfo interface_info = {
        interface_init, NULL, NULL
    };
    const RGObjClassInfo *interface_cinfo;
    const RGObjClassInfo *class_cinfo;
    if (RVAL2CBOOL(rb_obj_is_instance_of(class_or_module, rb_cModule))) {
        rb_raise(rb_eTypeError,
                 "GLib::Interface based module (%" PRIsVALUE ") "
                 "must be included into a class directly: %" PRIsVALUE,
                 self,
                 class_or_module);
    }
    interface_cinfo = rbgobj_lookup_class(self);
    class_cinfo = rbgobj_lookup_class(class_or_module);
    if (class_cinfo->klass != class_or_module) {
        return Qnil;
    }
    if (g_type_is_a(class_cinfo->gtype, interface_cinfo->gtype)) {
        return Qnil;
    }
    g_type_add_interface_static(class_cinfo->gtype,
                                interface_cinfo->gtype,
                                &interface_info);
    return Qnil;
}

static VALUE
rg_install_property(VALUE self, VALUE pspec_obj)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    gpointer ginterface;
    GParamSpec* pspec;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class", rb_class2name(self));

    pspec = G_PARAM_SPEC(RVAL2GOBJ(pspec_obj));
    ginterface = g_type_default_interface_ref(cinfo->gtype);
    g_object_interface_install_property(ginterface, pspec);
    g_type_default_interface_unref(ginterface);

    /* FIXME: define accessor methods */
    return Qnil;
}

static VALUE
rg_property(VALUE self, VALUE property_name)
{
    gpointer ginterface;
    const char* name;
    GParamSpec* prop;
    VALUE result;
    GType gtype = CLASS2GTYPE(self);

    if (SYMBOL_P(property_name))
        name = rb_id2name(SYM2ID(property_name));
    else
        name = StringValuePtr(property_name);

    if (!G_TYPE_IS_INTERFACE(gtype))
        rb_raise(rb_eTypeError, "%s isn't interface module", rb_class2name(self));
    /* XXX: g_type_default_interface_ref(G_TYPE_INTERFACE) causes SEGV. */
    if (gtype == G_TYPE_INTERFACE) {
        rb_raise(rb_const_get(rbg_mGLib(), rb_intern("NoPropertyError")),
                 "No such property: %s", name);
    }

    ginterface = g_type_default_interface_ref(gtype);
    prop = g_object_interface_find_property(ginterface, name);
    if (!prop){
        g_type_default_interface_unref(ginterface);
        rb_raise(rb_const_get(rbg_mGLib(), rb_intern("NoPropertyError")),
                 "No such property: %s", name);
    }
    result = GOBJ2RVAL(prop);
    g_type_default_interface_unref(ginterface);

    return result;
}

static VALUE
rg_properties(int argc, VALUE* argv, VALUE self)
{
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary = rb_ary_new();
    guint i;
    gpointer ginterface;
    GType gtype = CLASS2GTYPE(self);

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    if (!G_TYPE_IS_INTERFACE(gtype))
        rb_raise(rb_eTypeError, "%s isn't interface module", rb_class2name(self));
    /* XXX: g_type_default_interface_ref(G_TYPE_INTERFACE) causes SEGV. */
    if (gtype == G_TYPE_INTERFACE) return ary;

    ginterface = g_type_default_interface_ref(gtype);
    props = g_object_interface_list_properties(ginterface, &n_properties);
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too) || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, rb_str_new2(props[i]->name));
    }
    g_free(props);
    g_type_default_interface_unref(ginterface);

    return ary;
}

void
rbgobj_init_interface(VALUE interf)
{
    static VALUE rb_mGLibInterface = Qnil;

    rb_extend_object(interf, RG_TARGET_NAMESPACE);
    if (CLASS2GTYPE(interf) == G_TYPE_INTERFACE) {
        rb_mGLibInterface = interf;
    } else {
        rb_extend_object(interf, rb_mGLibInterface);
        rb_include_module(interf, rb_mGLibInterface);
        rbgobj_define_property_accessors(interf);
    }
}

void
Init_gobject_typeinterface(void)
{
    RG_TARGET_NAMESPACE = rb_define_module_under(rbg_mGLib(), "MetaInterface");
    rb_define_method(RG_TARGET_NAMESPACE, "gtype", generic_s_gtype, 0);
    rb_define_method(RG_TARGET_NAMESPACE,
                     "to_s",
                     rbgutil_generic_s_to_s_gtype_name_fallback,
                     0);
    RG_DEF_ALIAS("inspect", "to_s");
    RG_DEF_METHOD(append_features, 1);
    RG_DEF_METHOD(included, 1);
    RG_DEF_METHOD(install_property, 1);
    RG_DEF_METHOD(property, 1);
    RG_DEF_METHOD(properties, -1);

    rbgobj_mInterface = G_DEF_INTERFACE(G_TYPE_INTERFACE, "Interface", rbg_mGLib());
}
