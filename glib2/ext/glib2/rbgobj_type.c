/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2013  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE rbgobj_cType

/**********************************************************************/
/* Type Mapping */

static VALUE rb_cMutex;
static VALUE lookup_class_mutex;

static ID id_new;
static ID id_superclass;
static ID id_lock;
static ID id_unlock;
static GHashTable *gtype_to_cinfo;
static VALUE klass_to_cinfo;

static GHashTable* dynamic_gtype_list;
typedef struct {
    const gchar* name;
    VALUE module;
    void (*mark)(gpointer);
    void (*free)(gpointer);
    int flags; /* RGObjClassFlag */
} RGObjClassInfoDynamic;

typedef struct {
    VALUE parent;
    GType gtype;
    gboolean create_class;
} RGObjClassByGtypeData;

static void
cinfo_mark(RGObjClassInfo* cinfo)
{
    rb_gc_mark(cinfo->klass);
}

const RGObjClassInfo *
rbgobj_lookup_class(VALUE klass)
{
    VALUE data = rb_hash_aref(klass_to_cinfo, klass);
    if (!NIL_P(data)){
        RGObjClassInfo* cinfo;
        Data_Get_Struct(data, RGObjClassInfo, cinfo);
        return cinfo;
    }

    if (TYPE(klass) == T_CLASS) {
        VALUE super;
        if (FL_TEST(klass, FL_SINGLETON)) {
            super = rb_class_real(klass);
        } else {
            super = rb_funcall(klass, id_superclass, 0);
        }
        return rbgobj_lookup_class(super);
    }

    rb_raise(rb_eRuntimeError, "can't get gobject class information");
}

static const RGObjClassInfo *rbgobj_lookup_class_by_gtype_without_lock(GType gtype,
                                                                       VALUE parent,
                                                                       gboolean create_class);

static VALUE
get_superclass(GType gtype)
{
    VALUE super_class;

    if (rbgobj_convert_get_superclass(gtype, &super_class))
        return super_class;

    switch (gtype) {
      case G_TYPE_PARAM:
      case G_TYPE_OBJECT:
        return cInstantiatable;
      case G_TYPE_BOXED:
        return rb_cObject;
      case G_TYPE_POINTER:
        return rb_cData;
      case G_TYPE_ENUM:
      case G_TYPE_FLAGS:
        return rb_cObject;
      default:
      {
          GType parent_type;

          parent_type = g_type_parent(gtype);
          if (parent_type == G_TYPE_INVALID) {
              return cInstantiatable;
          } else {
              const RGObjClassInfo *cinfo_super;
              cinfo_super =
                  rbgobj_lookup_class_by_gtype_without_lock(parent_type,
                                                            Qnil,
                                                            TRUE);
              return cinfo_super->klass;
          }
      }
    }
}

static const RGObjClassInfo *
rbgobj_lookup_class_by_gtype_without_lock(GType gtype, VALUE parent,
                                          gboolean create_class)
{
    GType fundamental_type;
    RGObjClassInfo* cinfo;
    RGObjClassInfoDynamic* cinfod;
    void* gclass = NULL;
    VALUE c;

    if (gtype == G_TYPE_INVALID)
        return NULL;

    cinfo = g_hash_table_lookup(gtype_to_cinfo, GUINT_TO_POINTER(gtype));
    if (cinfo)
        return cinfo;

    if (!create_class)
        return NULL;

    c = Data_Make_Struct(rb_cData, RGObjClassInfo, cinfo_mark, NULL, cinfo);
    cinfo->gtype = gtype;
    cinfo->mark  = NULL;
    cinfo->free  = NULL;
    cinfo->flags = 0;

    fundamental_type = G_TYPE_FUNDAMENTAL(gtype);
    switch (fundamental_type){
    case G_TYPE_POINTER:
    case G_TYPE_BOXED:
    case G_TYPE_PARAM:
    case G_TYPE_OBJECT:
    case G_TYPE_ENUM:
    case G_TYPE_FLAGS:
        if (NIL_P(parent)) parent = get_superclass(gtype);
        cinfo->klass = rb_funcall(rb_cClass, id_new, 1, parent);
        break;

    case G_TYPE_INTERFACE:
        cinfo->klass = rb_module_new();
        break;

    default:
      if (NIL_P(parent)) parent = get_superclass(gtype);
      if (NIL_P(parent)) {
          fprintf(stderr,
                  "%s: %s's fundamental type %s isn't supported\n",
                  "rbgobj_lookup_class_by_gtype",
                  g_type_name(gtype),
                  g_type_name(fundamental_type));
          return NULL;
      }
      cinfo->klass = rb_funcall(rb_cClass, id_new, 1, parent);
    }

    cinfod = (RGObjClassInfoDynamic *)g_hash_table_lookup(dynamic_gtype_list,
                                                          g_type_name(gtype));
    if (cinfod){
        cinfo->mark = cinfod->mark;
        cinfo->free = cinfod->free;
        rb_define_const(cinfod->module, cinfod->name, cinfo->klass);
    }

    rb_hash_aset(klass_to_cinfo, cinfo->klass, c);
    g_hash_table_insert(gtype_to_cinfo, GUINT_TO_POINTER(gtype), cinfo);

    if (G_TYPE_IS_CLASSED(gtype))
        gclass = g_type_class_ref(gtype);

    if (G_TYPE_IS_INSTANTIATABLE(gtype) || G_TYPE_IS_INTERFACE(gtype))
        rbgobj_define_action_methods(cinfo->klass);

    if (G_TYPE_IS_INSTANTIATABLE(gtype)){
        GType* interfaces = NULL;
        guint n_interfaces = 0;
        guint i;

        interfaces = g_type_interfaces(gtype, &n_interfaces);
        for (i = 0; i < n_interfaces; i++){
            const RGObjClassInfo *iface_cinfo;
            iface_cinfo =
            rbgobj_lookup_class_by_gtype_without_lock(interfaces[i],
                                                      Qnil,
                                                      TRUE);
            rb_include_module(cinfo->klass, iface_cinfo->klass);
        }
        g_free(interfaces);
    }

    if (!rbgobj_convert_type_init_hook(gtype, cinfo->klass)) {
        switch (fundamental_type) {
          case G_TYPE_OBJECT:
            rbgobj_init_object_class(cinfo->klass);
            break;
          case G_TYPE_ENUM:
            rbgobj_init_enum_class(cinfo->klass);
            break;
          case G_TYPE_FLAGS:
            rbgobj_init_flags_class(cinfo->klass);
            break;
          case G_TYPE_INTERFACE:
            rbgobj_init_interface(cinfo->klass);
            break;
          default:
            rbgobj_convert_type_init_hook(fundamental_type, cinfo->klass);
            break;
        }
    }

    if (gclass)
        g_type_class_unref(gclass);

    return cinfo;
}

static VALUE
rbgobj_lookup_class_by_gtype_body(VALUE data)
{
    RGObjClassByGtypeData *cdata = (RGObjClassByGtypeData *)data;
    const RGObjClassInfo *cinfo;

    cinfo = rbgobj_lookup_class_by_gtype_without_lock(cdata->gtype,
                                                      cdata->parent,
                                                      cdata->create_class);
    return (VALUE)cinfo;
}

static VALUE
rbgobj_lookup_class_by_gtype_ensure(G_GNUC_UNUSED VALUE data)
{
    rb_funcall(lookup_class_mutex, id_unlock, 0);
    return Qundef;
}

const RGObjClassInfo *
rbgobj_lookup_class_by_gtype(GType gtype, VALUE parent)
{
    return rbgobj_lookup_class_by_gtype_full(gtype, parent, TRUE);
}

const RGObjClassInfo *
rbgobj_lookup_class_by_gtype_full(GType gtype, VALUE parent,
                                  gboolean create_class)
{
    RGObjClassByGtypeData data;

    data.gtype = gtype;
    data.parent = parent;
    data.create_class = create_class;

    if (create_class) {
        rb_funcall(lookup_class_mutex, id_lock, 0);
        return (RGObjClassInfo *)rb_ensure(rbgobj_lookup_class_by_gtype_body,
                                           (VALUE)&data,
                                           rbgobj_lookup_class_by_gtype_ensure,
                                           (VALUE)&data);
    } else {
        return rbgobj_lookup_class_by_gtype_without_lock(gtype, parent,
                                                         create_class);
    }
}

VALUE
rbgobj_gtype_to_ruby_class(GType gtype)
{
    const RGObjClassInfo *cinfo;

    cinfo = GTYPE2CINFO(gtype);
    return cinfo ? cinfo->klass : Qnil;
}

VALUE
rbgobj_define_class(GType gtype, const gchar *name, VALUE module, void *mark, void *free, VALUE parent)
{
    RGObjClassInfo* cinfo;
    if (gtype == 0)
        rb_bug("rbgobj_define_class: Invalid gtype [%s]\n", name);

    cinfo = (RGObjClassInfo*)rbgobj_lookup_class_by_gtype(gtype, parent);
    cinfo->mark = mark;
    cinfo->free = free;
    rb_define_const(module, name, cinfo->klass);
    return cinfo->klass;
}

VALUE
rbgobj_define_class_dynamic(const gchar *gtype_name, const gchar *name, VALUE module, void *mark, void *free)
{
    RGObjClassInfoDynamic* cinfo;
    cinfo = (RGObjClassInfoDynamic*)g_new(RGObjClassInfoDynamic, 1);
    cinfo->name = name;
    cinfo->module = module;
    cinfo->mark = mark;
    cinfo->free = free;
    g_hash_table_insert(dynamic_gtype_list, (void*)gtype_name, (void*)cinfo);
    return Qnil;
}

void
rbgobj_register_class(VALUE klass,
                      GType gtype,
                      gboolean klass2gtype,
                      gboolean gtype2klass)
{
    RGObjClassInfo* cinfo = NULL;
    VALUE c = Qnil;

    if (klass2gtype)
        c = Data_Make_Struct(rb_cData, RGObjClassInfo, cinfo_mark, NULL, cinfo);
    if (gtype2klass && !cinfo)
        cinfo = g_new(RGObjClassInfo, 1);

    if (cinfo) {
        cinfo->klass = klass;
        cinfo->gtype = gtype;
        cinfo->mark  = NULL;
        cinfo->free  = NULL;
        cinfo->flags = 0;
    }

    if (klass2gtype)
        rb_hash_aset(klass_to_cinfo, cinfo->klass, c);

    if (gtype2klass)
        g_hash_table_insert(gtype_to_cinfo, GUINT_TO_POINTER(gtype), cinfo);
}

#define _register_fundamental_klass_to_gtype(klass, gtype) \
    rbgobj_register_class(klass, gtype, TRUE, FALSE)

#define _register_fundamental_gtype_to_klass(gtype,klass) \
    rbgobj_register_class(klass, gtype, FALSE, TRUE)

static void
init_typemap(void)
{
    id_new = rb_intern("new");
    id_superclass = rb_intern("superclass");

    gtype_to_cinfo = g_hash_table_new(g_direct_hash, g_direct_equal);
    rb_global_variable(&klass_to_cinfo);
    klass_to_cinfo = rb_hash_new();

    _register_fundamental_klass_to_gtype(rb_cFixnum, G_TYPE_LONG);
    _register_fundamental_klass_to_gtype(rb_cFloat, G_TYPE_DOUBLE);
    _register_fundamental_klass_to_gtype(rb_cInteger, G_TYPE_LONG);
    _register_fundamental_klass_to_gtype(rb_cString, G_TYPE_STRING);
    _register_fundamental_klass_to_gtype(rb_cSymbol, G_TYPE_STRING);
    _register_fundamental_klass_to_gtype(Qnil, G_TYPE_NONE);
    _register_fundamental_klass_to_gtype(rb_cNilClass, G_TYPE_NONE);
    _register_fundamental_klass_to_gtype(rb_cTrueClass, G_TYPE_BOOLEAN);
    _register_fundamental_klass_to_gtype(rb_cFalseClass, G_TYPE_BOOLEAN);
    _register_fundamental_klass_to_gtype(Qtrue, G_TYPE_BOOLEAN);
    _register_fundamental_klass_to_gtype(Qfalse, G_TYPE_BOOLEAN);
    _register_fundamental_klass_to_gtype(rb_cObject, RBGOBJ_TYPE_RUBY_VALUE);

    _register_fundamental_gtype_to_klass(G_TYPE_UINT, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_FLOAT, rb_cFloat);
    _register_fundamental_gtype_to_klass(G_TYPE_DOUBLE, rb_cFloat);
    _register_fundamental_gtype_to_klass(G_TYPE_INT64, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_UINT64, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_INT, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_LONG, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_CHAR, rb_cFixnum);
    _register_fundamental_gtype_to_klass(G_TYPE_UCHAR, rb_cFixnum);
    _register_fundamental_gtype_to_klass(G_TYPE_STRING, rb_cString);
    _register_fundamental_gtype_to_klass(G_TYPE_ULONG, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_NONE, rb_cNilClass);
    _register_fundamental_gtype_to_klass(G_TYPE_BOOLEAN, rb_cTrueClass);
}

/**********************************************************************/
/* GLib::Type */

VALUE RG_TARGET_NAMESPACE;
static ID id_gtype;

VALUE
rbgobj_gtype_new(GType gtype)
{
    VALUE result = rb_obj_alloc(RG_TARGET_NAMESPACE);
    VALUE arg = ULONG2NUM(gtype);
    rb_obj_call_init(result, 1, &arg);
    return result;
}

GType
rbgobj_gtype_get(VALUE self)
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(self, RG_TARGET_NAMESPACE))) {
        return NUM2ULONG(rb_ivar_get(self, id_gtype));
    } else {
        return CLASS2GTYPE(self);
    }
    rb_raise(rb_eTypeError, "Not a GLib::Type");
}

static VALUE
rg_initialize(VALUE self, VALUE type)
{
    GType gtype;

    if (RVAL2CBOOL(rb_obj_is_kind_of(type, rb_cInteger))) {
        gtype = NUM2ULONG(type);
        if (!g_type_name(gtype))
            gtype = G_TYPE_INVALID;
    } else {
        gtype = g_type_from_name(StringValuePtr(type));
    }

    if (G_TYPE_INVALID == gtype)
        rb_raise(rb_eArgError, "invalid type");

    rb_ivar_set(self, id_gtype, ULONG2NUM(gtype));

    return Qnil;
}

static VALUE
rg_inspect(VALUE self)
{
    GType gtype = rbgobj_gtype_get(self);
    gchar* str;
    VALUE result;

    str = g_strdup_printf("GLib::Type[\"%s\"]", g_type_name(gtype));
    result = rb_str_new2(str);
    g_free(str);

    return result;
}

static VALUE
rg_operator_type_compare(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);

        if (a==b)
            return INT2FIX(0);
        else if (g_type_is_a(a,b))
            return INT2FIX(-1);
        else if (g_type_is_a(b,a))
            return INT2FIX(1);
        else
            return Qnil;
    }
}

static VALUE
rg_operator_type_eq(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(a == b);
    }
}

static VALUE
rg_operator_type_lt_eq(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(a, b));
    }
}

static VALUE
rg_operator_type_gt_eq(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(b, a));
    }
}

static VALUE
rg_operator_type_lt(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(a, b) && a != b);
    }
}

static VALUE
rg_operator_type_gt(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(b, a) && a != b);
    }
}

static VALUE
rg_hash(VALUE self)
{
    return rb_ivar_get(self, id_gtype);
}

static VALUE
rg_to_class(VALUE self)
{
    return GTYPE2CLASS(rbgobj_gtype_get(self));
}

static VALUE
rg_fundamental(VALUE self)
{
    return rbgobj_gtype_new(G_TYPE_FUNDAMENTAL(rbgobj_gtype_get(self)));
}

static VALUE
rg_fundamental_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_FUNDAMENTAL(rbgobj_gtype_get(self)));
}

static VALUE
rg_derived_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVED(rbgobj_gtype_get(self)));
}

static VALUE
rg_interface_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_INTERFACE(rbgobj_gtype_get(self)));
}

static VALUE
rg_classed_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_CLASSED(rbgobj_gtype_get(self)));
}

static VALUE
rg_instantiatable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_INSTANTIATABLE(rbgobj_gtype_get(self)));
}

static VALUE
rg_derivable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVABLE(rbgobj_gtype_get(self)));
}

static VALUE
rg_deep_derivable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DEEP_DERIVABLE(rbgobj_gtype_get(self)));
}

static VALUE
rg_abstract_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_ABSTRACT(rbgobj_gtype_get(self)));
}

static VALUE
rg_value_abstract_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_ABSTRACT(rbgobj_gtype_get(self)));
}

static VALUE
rg_value_type_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_TYPE(rbgobj_gtype_get(self)));
}

static VALUE
rg_has_value_table(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_HAS_VALUE_TABLE(rbgobj_gtype_get(self)));
}

static VALUE
rg_name(VALUE self)
{
    return rb_str_new2(g_type_name(rbgobj_gtype_get(self)));
}

static VALUE
rg_parent(VALUE self)
{
    GType parent = g_type_parent(rbgobj_gtype_get(self));
    return parent ? rbgobj_gtype_new(parent) : Qnil;
}

static VALUE
rg_depth(VALUE self)
{
    return UINT2NUM(g_type_depth(rbgobj_gtype_get(self)));
}

static VALUE
rg_next_base(VALUE leaf_type, VALUE root_type)
{
    GType ret = g_type_next_base(rbgobj_gtype_get(leaf_type),
                                 rbgobj_gtype_get(root_type));
    return ret ? rbgobj_gtype_new(ret) : Qnil;
}

static VALUE
rg_type_is_a_p(VALUE self, VALUE is_a_type)
{
    return CBOOL2RVAL(g_type_is_a(rbgobj_gtype_get(self), rbgobj_gtype_get(is_a_type)));
}

#if 0
gpointer              g_type_class_ref               (GType            type);
gpointer              g_type_class_peek              (GType            type);
void                  g_type_class_unref             (gpointer         g_class);
gpointer              g_type_class_peek_parent       (gpointer         g_class);
gpointer              g_type_interface_peek          (gpointer         instance_class,
                              GType            iface_type);
gpointer              g_type_interface_peek_parent   (gpointer         g_iface);
#endif 

static VALUE
rg_children(VALUE self)
{
    guint n_children;
    GType* types;
    VALUE result;
    guint i;

    types = g_type_children(rbgobj_gtype_get(self), &n_children);
    result = rb_ary_new2(n_children);
    for (i = 0; i < n_children; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(types[i]));
    g_free(types);

    return result;
}

static VALUE
rg_interfaces(VALUE self)
{
    guint n_interfaces;
    GType* types;
    VALUE result;
    guint i;

    types = g_type_interfaces(rbgobj_gtype_get(self), &n_interfaces);
    result = rb_ary_new2(n_interfaces);
    for (i = 0; i < n_interfaces; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(types[i]));
    g_free(types);

    return result;
}

static VALUE
rg_class_size(VALUE self)
{
    GTypeQuery query;
    g_type_query(rbgobj_gtype_get(self), &query);
    return UINT2NUM(query.class_size);
}

static VALUE
rg_instance_size(VALUE self)
{
    GTypeQuery query;
    g_type_query(rbgobj_gtype_get(self), &query);
    return UINT2NUM(query.instance_size);
}

static inline void
_def_fundamental_type(VALUE ary, GType gtype, const char* name)
{
    VALUE c = rbgobj_gtype_new(gtype);
    rb_define_const(RG_TARGET_NAMESPACE, name, c);
    rb_ary_push(ary, c);
}

/**********************************************************************/

void
Init_gobject_gtype(void)
{
#if !GLIB_CHECK_VERSION(2, 35, 1)
    g_type_init();
#endif

    init_typemap();

    /* type */
    rb_cMutex = rb_const_get(rb_cObject, rb_intern("Mutex"));
    id_lock = rb_intern("lock");
    id_unlock = rb_intern("unlock");
    lookup_class_mutex = rb_funcall(rb_cMutex, id_new, 0);
    rb_iv_set(mGLib, "lookup_class_mutex", lookup_class_mutex);

    dynamic_gtype_list = g_hash_table_new(g_str_hash, g_str_equal);
    id_gtype = rb_intern("__gobject_gtype__");

    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Type", rb_cObject);

    rb_define_alias(CLASS_OF(RG_TARGET_NAMESPACE), "[]", "new");
    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(inspect, 0);
    RG_DEF_METHOD_OPERATOR("<=>", type_compare, 1);
    RG_DEF_METHOD_OPERATOR("==", type_eq, 1);
    RG_DEF_METHOD_OPERATOR("<=", type_lt_eq, 1);
    RG_DEF_METHOD_OPERATOR(">=", type_gt_eq, 1);
    RG_DEF_METHOD_OPERATOR("<", type_lt, 1);
    RG_DEF_METHOD_OPERATOR(">", type_gt, 1);
    RG_DEF_ALIAS("eql?", "==");
    RG_DEF_METHOD(hash, 0);
    RG_DEF_ALIAS("to_i", "hash");
    RG_DEF_ALIAS("to_int", "hash");
    RG_DEF_METHOD(to_class, 0);

    RG_DEF_METHOD(fundamental, 0);
    RG_DEF_METHOD_P(fundamental, 0);
    RG_DEF_METHOD_P(derived, 0);
    RG_DEF_METHOD_P(interface, 0);
    RG_DEF_METHOD_P(classed, 0);
    RG_DEF_METHOD_P(instantiatable, 0);
    RG_DEF_METHOD_P(derivable, 0);
    RG_DEF_METHOD_P(deep_derivable, 0);
    RG_DEF_METHOD_P(abstract, 0);
    RG_DEF_METHOD_P(value_abstract, 0);
    RG_DEF_METHOD_P(value_type, 0);
    RG_DEF_METHOD(has_value_table, 0);

    RG_DEF_METHOD(name, 0);
    RG_DEF_ALIAS("to_s", "name");
    RG_DEF_METHOD(parent, 0);
    RG_DEF_METHOD(depth, 0);
    RG_DEF_METHOD(next_base, 1);
    RG_DEF_METHOD_P(type_is_a, 1);
    RG_DEF_METHOD(children, 0);
    RG_DEF_METHOD(interfaces, 0);
    RG_DEF_METHOD(class_size, 0);
    RG_DEF_METHOD(instance_size, 0);

    {
    VALUE ary = rb_ary_new();
    rb_define_const(RG_TARGET_NAMESPACE, "FUNDAMENTAL_MAX", INT2FIX(G_TYPE_FUNDAMENTAL_MAX));
    _def_fundamental_type(ary, G_TYPE_NONE,      "NONE");
    _def_fundamental_type(ary, G_TYPE_INTERFACE, "INTERFACE");
    _def_fundamental_type(ary, G_TYPE_CHAR,      "CHAR");
    _def_fundamental_type(ary, G_TYPE_UCHAR,     "UCHAR");
    _def_fundamental_type(ary, G_TYPE_BOOLEAN,   "BOOLEAN");
    _def_fundamental_type(ary, G_TYPE_INT,       "INT");
    _def_fundamental_type(ary, G_TYPE_UINT,      "UINT");
    _def_fundamental_type(ary, G_TYPE_LONG,      "LONG");
    _def_fundamental_type(ary, G_TYPE_ULONG,     "ULONG");
    _def_fundamental_type(ary, G_TYPE_INT64,     "INT64");
    _def_fundamental_type(ary, G_TYPE_UINT64,    "UINT64");
    _def_fundamental_type(ary, G_TYPE_ENUM,      "ENUM");
    _def_fundamental_type(ary, G_TYPE_FLAGS,     "FLAGS");
    _def_fundamental_type(ary, G_TYPE_FLOAT,     "FLOAT");
    _def_fundamental_type(ary, G_TYPE_DOUBLE,    "DOUBLE");
    _def_fundamental_type(ary, G_TYPE_STRING,    "STRING");
    _def_fundamental_type(ary, G_TYPE_POINTER,   "POINTER");
    _def_fundamental_type(ary, G_TYPE_BOXED,     "BOXED");
    _def_fundamental_type(ary, G_TYPE_PARAM,     "PARAM");
    _def_fundamental_type(ary, G_TYPE_OBJECT,    "OBJECT");
    rb_define_const(RG_TARGET_NAMESPACE, "FUNDAMENTAL_TYPES", ary); /* FIXME: better name */
    }
}
