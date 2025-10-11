/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2025  Ruby-GNOME Project Team
 *  Copyright (C) 2002-2003  Masahiro Sakai
 *  Copyright (C) 1998-2000  Yukihiro Matsumoto,
 *                           Daisuke Kanda,
 *                           Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE rbgobj_cObject

VALUE RG_TARGET_NAMESPACE;
static VALUE eNoPropertyError;
static GQuark RUBY_GOBJECT_OBJ_KEY;

VALUE
rbg_cGLibObject(void)
{
    return RG_TARGET_NAMESPACE;
}

gboolean
rbg_is_object(VALUE object)
{
    return RVAL2CBOOL(rb_obj_is_kind_of(object, RG_TARGET_NAMESPACE));
}

/* deprecated */
void
rbgobj_add_abstract_but_create_instance_class(G_GNUC_UNUSED GType gtype)
{
}

static void
weak_notify(gpointer data, G_GNUC_UNUSED GObject *where_the_object_was)
{
    gobj_holder *holder = data;

    g_hash_table_unref(holder->rb_relatives);
    holder->rb_relatives = NULL;
    holder->destroyed = TRUE;
    holder->gobj = NULL;
}

static void
holder_relatives_mark(G_GNUC_UNUSED gpointer key,
                      gpointer value,
                      G_GNUC_UNUSED gpointer user_data)
{
    VALUE rb_relative = (VALUE)value;
    rb_gc_mark(rb_relative);
}

static void
holder_mark(void *data)
{
    gobj_holder *holder = data;

    if (!holder->gobj)
        return;
    if (holder->destroyed)
        return;

    rbgobj_instance_call_cinfo_mark(holder->gobj);
    g_hash_table_foreach(holder->rb_relatives, holder_relatives_mark, NULL);
}

static void
holder_unref(gobj_holder *holder)
{
    if (holder->gobj) {
        if (!holder->destroyed) {
            g_object_set_qdata(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
            g_object_weak_unref(holder->gobj, (GWeakNotify)weak_notify, holder);
            rbgobj_instance_call_cinfo_free(holder->gobj);
            g_object_unref(holder->gobj);
            weak_notify(holder, holder->gobj);
        }
        holder->gobj = NULL;
    }
}

static void
holder_free(void *data)
{
    gobj_holder *holder = data;
    holder_unref(holder);
    xfree(holder);
}

static const rb_data_type_t rg_glib_object_type = {
    "GLib::Object",
    {
        holder_mark,
        holder_free,
    },
    &rbg_glib_instantiatable_type,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY | RUBY_TYPED_FROZEN_SHAREABLE,
};

void
rbgobj_object_add_relative(VALUE rb_gobject, VALUE rb_relative)
{
    gobj_holder *holder;
    TypedData_Get_Struct(rb_gobject,
                         gobj_holder,
                         &rg_glib_object_type,
                         holder);
    if (holder->rb_relatives) {
        g_hash_table_insert(holder->rb_relatives,
                            (gpointer)(rb_relative),
                            (gpointer)(rb_relative));
    }
}

void
rbgobj_gobject_remove_relative(GObject *gobject, VALUE rb_relative)
{
    gobj_holder *holder = g_object_get_qdata(gobject, RUBY_GOBJECT_OBJ_KEY);
    if (holder && holder->rb_relatives) {
        g_hash_table_remove(holder->rb_relatives, (gpointer)(rb_relative));
    }
}

void
rbgobj_object_remove_relative(VALUE rb_gobject, VALUE rb_relative)
{
    gobj_holder *holder;
    TypedData_Get_Struct(rb_gobject,
                         gobj_holder,
                         &rg_glib_object_type,
                         holder);
    if (holder->rb_relatives) {
        g_hash_table_remove(holder->rb_relatives, (gpointer)(rb_relative));
    }
}

static gboolean
rbgobj_object_remove_relatives_body(G_GNUC_UNUSED gpointer key,
                                    gpointer value,
                                    gpointer user_data)
{
    VALUE rb_relative = (VALUE)value;
    VALUE rb_relative_class = (VALUE)user_data;

    return RVAL2CBOOL(rb_obj_is_kind_of(rb_relative, rb_relative_class));
}

void
rbgobj_object_remove_relatives(VALUE rb_gobject, VALUE rb_relative_class)
{
    gobj_holder *holder;
    TypedData_Get_Struct(rb_gobject,
                         gobj_holder,
                         &rg_glib_object_type,
                         holder);
    g_hash_table_foreach_remove(holder->rb_relatives,
                                rbgobj_object_remove_relatives_body,
                                (gpointer)(rb_relative_class));
}

VALUE
rbgobj_object_alloc_func(VALUE klass)
{
    gobj_holder* holder;
    VALUE result;

    result = TypedData_Make_Struct(klass,
                                   gobj_holder,
                                   &rg_glib_object_type,
                                   holder);
    holder->self  = result;
    holder->gobj  = NULL;
    holder->cinfo = NULL;
    holder->destroyed = FALSE;
    holder->rb_relatives = g_hash_table_new(g_direct_hash, g_direct_equal);

    return result;
}

/* deprecated */
VALUE
rbgobj_create_object(VALUE klass)
{
    return rbgobj_object_alloc_func(klass);
}

void
rbgobj_gobject_initialize(VALUE obj, gpointer cobj)
{
    gobj_holder* holder = g_object_get_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        rb_raise(rb_eRuntimeError, "ruby wrapper for this GObject* already exists.");
    TypedData_Get_Struct(obj, gobj_holder, &rg_glib_object_type, holder);
    holder->cinfo = RVAL2CINFO(obj);
    holder->gobj  = (GObject*)cobj;
    holder->destroyed = FALSE;

    g_object_set_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY, (gpointer)holder);
    g_object_weak_ref((GObject*)cobj, (GWeakNotify)weak_notify, holder);
    {
        GType t1 = G_TYPE_FROM_INSTANCE(cobj);
        GType t2 = CLASS2GTYPE(CLASS_OF(obj));

        if (t1 != t2) {
            if (!g_type_is_a(t1, t2))
                rb_raise(rb_eTypeError, "%s is not subtype of %s",
                         g_type_name(t1), g_type_name(t2));
        }
    }
}

VALUE
rbgobj_get_ruby_object_from_gobject(GObject* gobj, gboolean alloc)
{
    gobj_holder *holder;

    holder = g_object_get_qdata(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder) {
        return holder->self;
    } else if (alloc) {
        VALUE obj;

        obj = rbgobj_object_alloc_func(GTYPE2CLASS(G_OBJECT_TYPE(gobj)));
        gobj = g_object_ref(gobj);
        rbgobj_gobject_initialize(obj, (gpointer)gobj);
        return obj;
    } else {
        return Qnil;
    }
}

GObject*
rbgobj_get_gobject(VALUE obj)
{
    gobj_holder* holder;

    TypedData_Get_Struct(obj, gobj_holder, &rg_glib_object_type, holder);

    if (holder->destroyed)
        rb_raise(rb_eTypeError, "destroyed GLib::Object");
    if (!holder->gobj)
        rb_raise(rb_eTypeError, "uninitialize GLib::Object");

    return holder->gobj;
}

void
rbgobj_init_object_class(VALUE klass)
{
    rbgobj_define_property_accessors(klass);
}

/**********************************************************************/

static void
gobj_mark(gpointer ptr)
{
    GObject* gobj = ptr;
    guint n_properties;
    GParamSpec** properties;
    guint i;

    properties = g_object_class_list_properties(G_OBJECT_GET_CLASS(gobj), &n_properties);

    for (i = 0; i < n_properties; i++) {
        GParamSpec* pspec = properties[i];
        GType value_type = G_PARAM_SPEC_VALUE_TYPE(pspec);
        if (!G_TYPE_IS_OBJECT(value_type)) continue;
        if (!(pspec->flags & G_PARAM_READABLE)) continue;
        if (G_TYPE_IS_OBJECT(pspec->owner_type)) {
            const RGObjClassInfo *owner_cinfo =
                rbgobj_class_info_lookup_by_gtype(pspec->owner_type);
            if (owner_cinfo && (owner_cinfo->flags & RBGOBJ_DEFINED_BY_RUBY)) {
                /* We can't get properties of a GObject defined by Ruby
                 * in GC becaue we need to call a Ruby method to get a
                 * property. It may allocates some objects in GC. It's
                 * not allowed.
                 *
                 * Anyway, we don't need to mark properties here
                 * because properties must be referred from a GObject
                 * defined by Ruby. */
                continue;
            }
        }
        /* FIXME: exclude types that doesn't have identity. */

        {
            GValue gval = G_VALUE_INIT;
            g_value_init(&gval, value_type);
            g_object_get_property(gobj, pspec->name, &gval);
            rbgobj_gc_mark_gvalue(&gval);
            g_value_unset(&gval);
        }
    }

    g_free(properties);
}

typedef struct {
    VALUE rb_properties;
    GObjectClass* gclass;
    guint n_properties;
    const char **names;
    GValue* values;
    guint index;
} rbgobj_new_data;

static int
rbgobj_new_set_property(VALUE key,
                        VALUE value,
                        VALUE rb_data)
{
    rbgobj_new_data *data = (rbgobj_new_data *)rb_data;

    guint index = data->index;
    if (index >= data->n_properties)
       rb_raise(rb_eArgError, "too many properties");

    const char *name;
    if (SYMBOL_P(key)) {
        name = rb_id2name(SYM2ID(key));
    } else {
        name = StringValueCStr(key);
    }

    GParamSpec *pspec = g_object_class_find_property(data->gclass, name);
    if (!pspec)
        rb_raise(rb_eArgError, "No such property: %s", name);

    data->names[index] = pspec->name;
    g_value_init(&(data->values[index]), G_PARAM_SPEC_VALUE_TYPE(pspec));
    rbgobj_rvalue_to_gvalue(value, &(data->values[index]));

    data->index++;

    return ST_CONTINUE;
}

static VALUE
rbgobj_new_body(VALUE rb_data)
{
    rbgobj_new_data *data = (rbgobj_new_data *)rb_data;
    rb_hash_foreach(data->rb_properties, rbgobj_new_set_property, rb_data);
    return (VALUE)g_object_new_with_properties(G_TYPE_FROM_CLASS(data->gclass),
                                               data->n_properties,
                                               data->names,
                                               data->values);
}

static VALUE
rbgobj_new_ensure(VALUE rb_data)
{
    rbgobj_new_data *data = (rbgobj_new_data *)rb_data;
    guint i;
    for (i = 0; i < data->n_properties; i++) {
        g_value_unset(&(data->values[i]));
    }
    g_type_class_unref(data->gclass);
    return Qnil;
}

static GObject *
rbgobj_gobject_new(int argc, VALUE *argv, GType gtype)
{
    if (!g_type_is_a(gtype, G_TYPE_OBJECT))
        rb_raise(rb_eArgError,
                 "type \"%s\" is not descendant of GObject",
                 g_type_name(gtype));

    VALUE rb_properties = Qnil;
#ifdef RB_SCAN_ARGS_LAST_HASH_KEYWORDS
    /* TODO: Can we use rb_get_kwargs()? */
    rb_scan_args_kw(RB_SCAN_ARGS_LAST_HASH_KEYWORDS, argc, argv,
                    "0:", &rb_properties);
#else
    /* We can remove this when we drop support for AlmaLinux 8 that
     * ships Ruby 2.5 as the default Ruby. */
    rb_scan_args(argc, argv, "01", &rb_properties);
#endif

    GObject *gobject;
    if (NIL_P(rb_properties)) {
        gobject = g_object_new_with_properties(gtype, 0, NULL, NULL);
    } else {
        rbgobj_new_data data;
        data.rb_properties = rb_properties;
        data.gclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
        data.n_properties = RHASH_SIZE(rb_properties);
        data.names = NULL;
        data.values = NULL;
        data.names = ALLOCA_N(const char *, data.n_properties);
        data.values = ALLOCA_N(GValue, data.n_properties);
        guint i;
        GValue empty_value = G_VALUE_INIT;
        for (i = 0; i < data.n_properties; i++) {
            data.names[i] = NULL;
            data.values[i] = empty_value;
        }
        data.index = 0;

        gobject = (GObject *)rb_ensure(&rbgobj_new_body, (VALUE)(&data),
                                       &rbgobj_new_ensure, (VALUE)(&data));
    }

    if (!gobject)
        rb_raise(rb_eRuntimeError, "g_object_new_with_properties() failed");

    return gobject;
}

static VALUE
rg_s_new_bang(int argc, VALUE *argv, VALUE self)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    GObject* gobj;
    VALUE result;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class",
                 rb_class2name(self));

    gobj = rbgobj_gobject_new(argc, argv, cinfo->gtype);
    result = GOBJ2RVAL(gobj);
    g_object_unref(gobj);

    return result;
}

static VALUE
rg_s_init(VALUE self)
{
    return RUBY_Qnil;
}

static VALUE
rg_s_install_property(int argc, VALUE* argv, VALUE self)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    gpointer gclass;
    GParamSpec* pspec;
    VALUE pspec_obj, prop_id;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class",
                 rb_class2name(self));

    rb_scan_args(argc, argv, "11", &pspec_obj, &prop_id);
    pspec = G_PARAM_SPEC(RVAL2GOBJ(pspec_obj));

    gclass = g_type_class_ref(cinfo->gtype);
    g_object_class_install_property(gclass,
                                    NIL_P(prop_id) ? 1 : NUM2UINT(prop_id),
                                    pspec);
    g_type_class_unref(gclass);

    /* FIXME: define accessor methods */

    return Qnil;
}

static VALUE
gobj_s_property(VALUE self, VALUE property_name)
{
    GObjectClass* oclass;
    const char* name;
    GParamSpec* prop;
    VALUE result;

    if (SYMBOL_P(property_name))
        name = rb_id2name(SYM2ID(property_name));
    else
        name = StringValuePtr(property_name);

    oclass = g_type_class_ref(CLASS2GTYPE(self));

    prop = g_object_class_find_property(oclass, name);
    if (!prop){
        g_type_class_unref(oclass);
        rb_raise(eNoPropertyError, "No such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}

static VALUE
gobj_s_properties(int argc, VALUE* argv, VALUE self)
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    guint i;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    props = g_object_class_list_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too)
            || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, rb_str_new2(props[i]->name));
    }
    g_free(props);
    g_type_class_unref(oclass);
    return ary;
}

static GHashTable *rbg_type_to_prop_setter_tables;
static GMutex rbg_type_to_prop_setter_tables_mutex;
static GHashTable *rbg_type_to_prop_getter_tables;
static GMutex rbg_type_to_prop_getter_tables_mutex;

static void
rbg_register_property_accessor(GHashTable *tables,
                               GMutex *mutex,
                               GType gtype,
                               const char *name,
                               gpointer accessor)
{
    g_mutex_lock(mutex);

    GHashTable *table = g_hash_table_lookup(tables, GUINT_TO_POINTER(gtype));
    if (!table) {
        table = g_hash_table_new(g_str_hash, g_str_equal);
        g_hash_table_insert(tables, GUINT_TO_POINTER(gtype), table);
    }

    GObjectClass *gclass = g_type_class_ref(gtype);
    GParamSpec *pspec = g_object_class_find_property(gclass, name);
    g_hash_table_insert(table,
                        (gchar *)g_param_spec_get_name(pspec),
                        accessor);
    g_type_class_unref(gclass);

    g_mutex_unlock(mutex);
}

static gpointer
rbg_get_property_accessor(GHashTable *tables,
                          GMutex *mutex,
                          GType gtype,
                          const char *name)
{
    g_mutex_lock(mutex);

    gpointer accessor = NULL;
    GHashTable *table = g_hash_table_lookup(tables, GUINT_TO_POINTER(gtype));
    if (table) {
        GObjectClass *gclass = g_type_class_ref(gtype);
        GParamSpec *pspec = g_object_class_find_property(gclass, name);
        accessor = g_hash_table_lookup(table, g_param_spec_get_name(pspec));
        g_type_class_unref(gclass);
    }

    g_mutex_unlock(mutex);

    return accessor;
}

void
rbgobj_register_property_setter(GType gtype,
                                const char *name,
                                RValueToGValueFunc func)
{
    rbg_register_property_accessor(rbg_type_to_prop_setter_tables,
                                   &rbg_type_to_prop_setter_tables_mutex,
                                   gtype,
                                   name,
                                   func);
}

void
rbgobj_register_property_getter(GType gtype, const char *name, GValueToRValueFunc func)
{
    rbg_register_property_accessor(rbg_type_to_prop_getter_tables,
                                   &rbg_type_to_prop_getter_tables_mutex,
                                   gtype,
                                   name,
                                   func);
}

static VALUE
rg_set_property(VALUE self, VALUE prop_name, VALUE val)
{
    const char* name;
    if (SYMBOL_P(prop_name))
        name = rb_id2name(SYM2ID(prop_name));
    else
        name = StringValuePtr(prop_name);

    GParamSpec* pspec =
        g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                     name);
    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);

    RValueToGValueFunc setter =
        rbg_get_property_accessor(rbg_type_to_prop_setter_tables,
                                  &rbg_type_to_prop_setter_tables_mutex,
                                  pspec->owner_type,
                                  name);
    // FIXME: use rb_ensure to call g_value_unset()
    GValue gval = G_VALUE_INIT;
    g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
    if (setter) {
        setter(val, &gval);
    } else {
        rbgobj_rvalue_to_gvalue(val, &gval);
    }
    g_object_set_property(RVAL2GOBJ(self), name, &gval);
    g_value_unset(&gval);

    G_CHILD_SET(self, rb_intern(name), val);

    return self;
}

static VALUE
rg_get_property(VALUE self, VALUE prop_name)
{
    const char* name;
    if (SYMBOL_P(prop_name))
        name = rb_id2name(SYM2ID(prop_name));
    else
        name = StringValuePtr(prop_name);

    GParamSpec *pspec =
        g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                     name);
    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);

    GValueToRValueFunc getter =
        rbg_get_property_accessor(rbg_type_to_prop_getter_tables,
                                  &rbg_type_to_prop_getter_tables_mutex,
                                  pspec->owner_type,
                                  name);
    // FIXME: use rb_ensure to call g_value_unset()
    GValue gval = G_VALUE_INIT;
    g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
    g_object_get_property(RVAL2GOBJ(self), name, &gval);
    VALUE ret = getter ? getter(&gval) : GVAL2RVAL(&gval);
    g_value_unset(&gval);

    G_CHILD_SET(self, rb_intern(name), ret);

    return ret;
}

static VALUE rg_thaw_notify(VALUE self);

static VALUE
rg_freeze_notify(VALUE self)
{
    g_object_freeze_notify(RVAL2GOBJ(self));
    if (rb_block_given_p()) {
        return rb_ensure(rb_yield, self, rg_thaw_notify, self);
    }
    return self;
}

static VALUE
rg_notify(VALUE self, VALUE property_name)
{
    g_object_notify(RVAL2GOBJ(self), StringValuePtr(property_name));
    return self;
}

static VALUE
rg_thaw_notify(VALUE self)
{
    g_object_thaw_notify(RVAL2GOBJ(self));
    return self;
}

static VALUE
rg_destroyed_p(VALUE self)
{
    gobj_holder* holder;

    TypedData_Get_Struct(self, gobj_holder, &rg_glib_object_type, holder);

    return CBOOL2RVAL(holder->destroyed);
}

static VALUE
rg_inspect(VALUE self)
{
    gobj_holder* holder;

    TypedData_Get_Struct(self, gobj_holder, &rg_glib_object_type, holder);

    if (holder->destroyed) {
        return rb_sprintf("#<%" PRIsVALUE ":%p destroyed>",
                          rb_obj_class(self), (void *)self);
    } else {
        return rb_sprintf("#<%" PRIsVALUE ":%p ptr=%p>",
                          rb_obj_class(self), (void *)self, holder->gobj);
    }
}

static VALUE
rg_unref(VALUE self)
{
    gobj_holder* holder;

    TypedData_Get_Struct(self, gobj_holder, &rg_glib_object_type, holder);

    if (holder->destroyed)
        rb_raise(rb_eTypeError, "destroyed GLib::Object");
    if (!holder->gobj)
        rb_raise(rb_eTypeError, "uninitialize GLib::Object");

    holder_unref(holder);

    return self;
}

static VALUE
rg_type_name(VALUE self)
{
    return CSTR2RVAL(G_OBJECT_TYPE_NAME(RVAL2GOBJ(self)));
}

typedef struct {
    VALUE transform_from_callback;
    VALUE transform_to_callback;
    VALUE self;
} RGBindPropertyCallbackData;

static gboolean
rg_bind_property_transform_to_callback(G_GNUC_UNUSED GBinding *binding,
                                       const GValue *from_value,
                                       GValue *to_value,
                                       gpointer user_data)
{
    RGBindPropertyCallbackData *data = (RGBindPropertyCallbackData *)user_data;
    VALUE proc;
    VALUE rb_from_value;
    VALUE rb_to_value;

    proc = data->transform_to_callback;
    if (NIL_P(proc))
        return FALSE;

    rb_from_value = rbgobj_gvalue_to_rvalue(from_value);
    rb_to_value = rbgobj_gvalue_to_rvalue(to_value);
    rb_to_value = rb_funcall(proc, rb_intern("call"), 1, rb_from_value);
    rbgobj_rvalue_to_gvalue(rb_to_value, to_value);
    return TRUE;
}

static gboolean
rg_bind_property_transform_from_callback(G_GNUC_UNUSED GBinding *binding,
                                         const GValue *from_value,
                                         GValue *to_value,
                                         gpointer user_data)
{
    RGBindPropertyCallbackData *data = (RGBindPropertyCallbackData *)user_data;
    VALUE proc;
    VALUE rb_from_value;
    VALUE rb_to_value;

    proc = data->transform_from_callback;
    if (NIL_P(proc))
        return FALSE;

    rb_from_value = rbgobj_gvalue_to_rvalue(from_value);
    rb_to_value = rbgobj_gvalue_to_rvalue(to_value);
    rb_to_value = rb_funcall(proc, rb_intern("call"), 1, rb_from_value);
    rbgobj_rvalue_to_gvalue(rb_to_value, to_value);
    return TRUE;
}

static void
rg_destroy_bind_property_full_data(gpointer user_data)
{
    RGBindPropertyCallbackData *data = (RGBindPropertyCallbackData *)user_data;
    xfree(data);
}

static VALUE
rg_bind_property(gint argc, VALUE *argv, VALUE self)
{
    VALUE rb_source_property;
    VALUE rb_target;
    VALUE rb_target_property;
    VALUE rb_flags;
    VALUE rb_options;
    VALUE rb_transform_to;
    VALUE rb_transform_from;

    gpointer source;
    const gchar *source_property;
    gpointer target;
    const gchar *target_property;
    GBindingFlags flags;
    GBinding *binding;
    GBindingTransformFunc transform_to = NULL;
    GBindingTransformFunc transform_from = NULL;
    VALUE rb_binding;

    rb_scan_args(argc, argv, "41", &rb_source_property, &rb_target,
                 &rb_target_property, &rb_flags, &rb_options);

    rbg_scan_options(rb_options,
                     "transform_to", &rb_transform_to,
                     "transform_from", &rb_transform_from,
                     NULL);

    source = RVAL2GOBJ(self);
    source_property = RVAL2CSTR(rb_source_property);
    target = RVAL2GOBJ(rb_target);
    target_property = RVAL2CSTR(rb_target_property);
    flags = RVAL2GBINDINGFLAGS(rb_flags);

    if (!NIL_P(rb_transform_to)) {
        transform_to = rg_bind_property_transform_to_callback;
    }

    if (!NIL_P(rb_transform_from)) {
        transform_from = rg_bind_property_transform_from_callback;
    }

    if (transform_to || transform_from) {
        RGBindPropertyCallbackData *data;
        data = RB_ALLOC(RGBindPropertyCallbackData);
        data->self = self;
        data->transform_to_callback = rb_transform_to;
        data->transform_from_callback = rb_transform_from;
        binding = g_object_bind_property_full(source, source_property,
                                              target, target_property,
                                              flags, transform_to,
                                              transform_from,
                                              (gpointer)data,
                                              rg_destroy_bind_property_full_data);
        rb_binding = GOBJ2RVAL(binding);
        if (!NIL_P(rb_transform_to)) {
            rbgobj_object_add_relative(rb_binding, rb_transform_to);
        }
        if (!NIL_P(rb_transform_from)) {
            rbgobj_object_add_relative(rb_binding, rb_transform_from);
        }
    } else {
        binding = g_object_bind_property(source, source_property,
                                         target, target_property,
                                         flags);
        rb_binding = GOBJ2RVAL(binding);
    }

    return rb_binding;
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GType gtype;
    GObject* gobj;

    gtype = CLASS2GTYPE(CLASS_OF(self));
    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError,
                 "initializing abstract class: %s",
                 RBG_INSPECT(CLASS_OF(self)));
    }

    gobj = rbgobj_gobject_new(argc, argv, gtype);

    G_INITIALIZE(self, gobj);

    rb_funcall(self, rb_intern("initialize_post"), 0);

    return Qnil;
}

static VALUE
rg_initialize_post(VALUE self)
{
    return Qnil;
}

static VALUE
gobj_ref_count(VALUE self)
{
    gobj_holder* holder;
    TypedData_Get_Struct(self, gobj_holder, &rg_glib_object_type, holder);
    return INT2NUM(holder->gobj ? holder->gobj->ref_count : 0);
}

static VALUE
rg_floating_p(VALUE self)
{
    gobj_holder* holder;
    TypedData_Get_Struct(self, gobj_holder, &rg_glib_object_type, holder);
    if (holder->gobj) {
        return CBOOL2RVAL(g_object_is_floating(holder->gobj));
    } else {
        return Qfalse;
    }
}

/**********************************************************************/

static GQuark q_ruby_setter;
static GQuark q_ruby_getter;

// FIXME: use rb_protect
static void
get_prop_func(GObject* object,
              G_GNUC_UNUSED guint property_id,
              GValue* value,
              GParamSpec* pspec)
{
    ID ruby_getter = (ID)g_param_spec_get_qdata(pspec, q_ruby_getter);
    if (!ruby_getter) {
        gchar* name = g_strdup(g_param_spec_get_name(pspec));
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_getter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_getter, (gpointer)ruby_getter);
        g_free(name);
    }

    {
        VALUE ret = rb_funcall(GOBJ2RVAL(object), ruby_getter, 0);
        rbgobj_rvalue_to_gvalue(ret, value);
    }
}

// FIXME: use rb_protect
static void
set_prop_func(GObject* object,
              G_GNUC_UNUSED guint property_id,
              const GValue* value,
              GParamSpec* pspec)
{
    ID ruby_setter = (ID)g_param_spec_get_qdata(pspec, q_ruby_setter);
    if (!ruby_setter) {
        gchar* name = g_strconcat(g_param_spec_get_name(pspec), "=", NULL);
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_setter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_setter, (gpointer)ruby_setter);
        g_free(name);
    }

    rb_funcall(GOBJ2RVAL(object), ruby_setter, 1, GVAL2RVAL(value));
}

void
rbgobj_class_init_func(gpointer g_class, G_GNUC_UNUSED gpointer class_data)
{
    GObjectClass* g_object_class = G_OBJECT_CLASS(g_class);

    g_object_class->set_property = set_prop_func;
    g_object_class->get_property = get_prop_func;

    VALUE rb_class = GTYPE2CLASS(G_TYPE_FROM_CLASS(g_class));
    rb_funcall(rb_class, rb_intern("init"), 0);
}

void
rbgobj_register_type(VALUE klass, VALUE type_name, GClassInitFunc class_init)
{
    GType parent_type;

    {
        const RGObjClassInfo *cinfo = rbgobj_lookup_class(klass);
        if (cinfo->klass == klass)
            rb_raise(rb_eTypeError,
                     "already registered class: <%s>",
                     RBG_INSPECT(klass));
    }

    {
        VALUE superclass = rb_funcall(klass, rb_intern("superclass"), 0);
        const RGObjClassInfo *cinfo = rbgobj_lookup_class(superclass);
        if (cinfo->klass != superclass)
            rb_raise(rb_eTypeError,
                     "super class must be registered to GLib: <%s>",
                     RBG_INSPECT(superclass));
        parent_type = cinfo->gtype;
    }

    if (NIL_P(type_name)) {
        VALUE klass_name = rb_funcall(klass, rb_intern("name"), 0);

        if (strlen(StringValueCStr(klass_name)) == 0)
            rb_raise(rb_eTypeError,
                     "can't determine type name: <%s>",
                     RBG_INSPECT(klass));

        type_name = rb_funcall(klass_name, rb_intern("gsub"),
                               2,
                               rb_str_new_cstr("::"),
                               rb_str_new_cstr(""));
    }

    {
        GTypeQuery query;
        g_type_query(parent_type, &query);

        GType type = g_type_register_static_simple(parent_type,
                                                   StringValueCStr(type_name),
                                                   query.class_size,
                                                   class_init,
                                                   query.instance_size,
                                                   NULL,
                                                   0);
        if (type == G_TYPE_INVALID) {
            rb_raise(rb_eArgError,
                     "failed to register type: <%s>",
                     StringValueCStr(type_name));
        }

        rbgobj_register_class(klass, type, TRUE, TRUE);

        {
            RGObjClassInfo *cinfo = (RGObjClassInfo *)rbgobj_lookup_class(klass);
            cinfo->flags |= RBGOBJ_DEFINED_BY_RUBY;
        }

        {
            GType parent = g_type_parent(type);
            const RGObjClassInfo *cinfo =  GTYPE2CINFO(parent);
            VALUE initialize_module;

            initialize_module = rb_define_module_under(klass,
                                                       RubyGObjectHookModule);
            if (!(cinfo->flags & RBGOBJ_DEFINED_BY_RUBY)) {
                rb_define_method(initialize_module,
                                 "initialize", rg_initialize, -1);
            }

            rb_include_module(klass, initialize_module);
        }
    }
}

static VALUE
rg_s_type_register(int argc, VALUE *argv, VALUE self)
{
    VALUE type_name;

    rb_scan_args(argc, argv, "01", &type_name);

    rbgobj_register_type(self, type_name, rbgobj_class_init_func);

    return Qnil;
}

/**********************************************************************/

void
Init_gobject_gobject(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(G_TYPE_OBJECT, "Object", rbg_mGLib(),
                                                  gobj_mark, NULL);

#ifdef G_TYPE_INITIALLY_UNOWNED
    G_DEF_CLASS(G_TYPE_INITIALLY_UNOWNED, "InitiallyUnowned", rbg_mGLib());
#endif

    RUBY_GOBJECT_OBJ_KEY = g_quark_from_static_string("__ruby_gobject_object__");

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rbgobj_object_alloc_func);
    RG_DEF_SMETHOD_BANG(new, -1);

    RG_DEF_SMETHOD(init, 0);

    rb_define_singleton_method(RG_TARGET_NAMESPACE, "property", gobj_s_property, 1);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "properties", gobj_s_properties, -1);
    RG_DEF_SMETHOD(install_property, -1);
    q_ruby_getter = g_quark_from_static_string("__ruby_getter");
    q_ruby_setter = g_quark_from_static_string("__ruby_setter");

    RG_DEF_METHOD(set_property, 2);
    RG_DEF_METHOD(get_property, 1);
    RG_DEF_METHOD(freeze_notify, 0);
    rb_undef_method(RG_TARGET_NAMESPACE, "notify");
    RG_DEF_METHOD(notify, 1);
    RG_DEF_METHOD(thaw_notify, 0);
    RG_DEF_METHOD_P(destroyed, 0);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_PRIVATE_METHOD(initialize_post, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ref_count", gobj_ref_count, 0); /* for debugging */
    RG_DEF_METHOD_P(floating, 0); /* for debugging */
    RG_DEF_METHOD(unref, 0);
    RG_DEF_METHOD(inspect, 0);
    RG_DEF_METHOD(type_name, 0);

    RG_DEF_METHOD(bind_property, -1);
    G_DEF_CLASS(G_TYPE_BINDING_FLAGS, "BindingFlags", rbg_mGLib());

    eNoPropertyError = rb_define_class_under(rbg_mGLib(), "NoPropertyError",
                                             rb_eNameError);

    rbg_type_to_prop_setter_tables =
        g_hash_table_new_full(g_direct_hash,
                              g_direct_equal,
                              NULL,
                              (GDestroyNotify)g_hash_table_unref);
    g_mutex_init(&rbg_type_to_prop_setter_tables_mutex);
    rbg_type_to_prop_getter_tables =
        g_hash_table_new_full(g_direct_hash,
                              g_direct_equal,
                              NULL,
                              (GDestroyNotify)g_hash_table_unref);
    g_mutex_init(&rbg_type_to_prop_getter_tables_mutex);

    /* subclass */
    RG_DEF_SMETHOD(type_register, -1);
}
