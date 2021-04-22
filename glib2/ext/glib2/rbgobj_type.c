/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2021  Ruby-GNOME Project Team
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

#ifndef rb_cMutex
static VALUE rb_cMutex;
#endif
static VALUE lookup_class_mutex;

static ID id_gtype;
static ID id_lock;
static ID id_new;
static ID id_superclass;
static ID id_unlock;
static GHashTable *gtype_to_cinfo;
static VALUE klass_to_cinfo;

static GHashTable* dynamic_gtype_list;
typedef struct {
    const gchar* name;
    VALUE module;
    RGMarkFunc mark;
    RGFreeFunc free;
    int flags; /* RGObjClassFlag */
} RGObjClassInfoDynamic;

typedef struct {
    GType gtype;
    const gchar *name;
    VALUE module;
    VALUE parent;
} RGObjDefineClassData;

static void
cinfo_mark(void *data)
{
    RGObjClassInfo *cinfo = data;
    rb_gc_mark(cinfo->klass);
}

static void
cinfo_free(void *data)
{
    RGObjClassInfo *cinfo = data;
    xfree(cinfo->name);
    xfree(cinfo->data_type);
}

static RGObjClassInfo *
rbgobj_class_info_define_without_lock(GType gtype,
                                      const gchar *name,
                                      VALUE module,
                                      VALUE parent);

static void
rbgobj_class_info_fill_name(RGObjClassInfo *cinfo)
{
    VALUE rb_name;

    if (cinfo->name) {
        return;
    }

    if (!RB_TYPE_P(cinfo->klass, RUBY_T_CLASS)) {
        return;
    }

    rb_name = rb_funcall(cinfo->klass, rb_intern("name"), 0);
    if (NIL_P(rb_name)) {
        return;
    }

    cinfo->name = RB_ALLOC_N(char, RSTRING_LEN(rb_name) + 1);
    memcpy(cinfo->name, RSTRING_PTR(rb_name), RSTRING_LEN(rb_name));
    cinfo->name[RSTRING_LEN(rb_name)] = '\0';
    cinfo->data_type->wrap_struct_name = cinfo->name;
}

static rb_data_type_t *
rbgobj_class_info_create_data_type(VALUE klass)
{
    rb_data_type_t *data_type;

    data_type = RB_ZALLOC(rb_data_type_t);
    data_type->function.dmark = cinfo_mark;
    data_type->function.dfree = cinfo_free;
    if (RB_TYPE_P(klass, RUBY_T_CLASS) && klass != rb_cObject) {
        VALUE p = RCLASS_SUPER(klass);
        while (p != rb_cObject) {
            if (RB_TYPE_P(p, RUBY_T_DATA) && RTYPEDDATA_P(p)) {
                data_type->parent = RTYPEDDATA_TYPE(p);
                break;
            }
            p = RCLASS_SUPER(p);
        }
    }
    data_type->flags = RUBY_TYPED_FREE_IMMEDIATELY;

    return data_type;
}

static RGObjClassInfo *
rbgobj_class_info_register_without_lock(GType gtype, VALUE klass)
{
    rb_data_type_t *data_type;
    RGObjClassInfo *cinfo;
    GType fundamental_type;
    RGObjClassInfoDynamic *cinfod;
    void *gclass = NULL;
    VALUE c;

    data_type = rbgobj_class_info_create_data_type(klass);
    c = TypedData_Make_Struct(rb_cObject, RGObjClassInfo, data_type, cinfo);
    cinfo->klass = klass;
    cinfo->gtype = gtype;
    cinfo->mark  = NULL;
    cinfo->free  = NULL;
    cinfo->flags = 0;
    cinfo->name = NULL;
    cinfo->data_type = data_type;
    rbgobj_class_info_fill_name(cinfo);

    fundamental_type = G_TYPE_FUNDAMENTAL(gtype);
    switch (fundamental_type){
      case G_TYPE_BOXED:
        rb_define_alloc_func(cinfo->klass, rbgobj_boxed_alloc_func);
        break;
      case G_TYPE_OBJECT:
        rb_define_alloc_func(cinfo->klass, rbgobj_object_alloc_func);
        break;
      case G_TYPE_ENUM:
        rb_define_alloc_func(cinfo->klass, rbgobj_enum_alloc_func);
        break;
      case G_TYPE_FLAGS:
        rb_define_alloc_func(cinfo->klass, rbgobj_flags_alloc_func);
        break;
      default:
        break;
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
            iface_cinfo = rbgobj_class_info_lookup_by_gtype(interfaces[i]);
            if (!iface_cinfo) {
                iface_cinfo =
                    rbgobj_class_info_define_without_lock(interfaces[i],
                                                          NULL,
                                                          rb_cObject,
                                                          Qnil);
            }
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
get_superclass(GType gtype, VALUE module)
{
    VALUE super_class;

    if (rbgobj_convert_get_superclass(gtype, &super_class))
        return super_class;

    switch (gtype) {
      case G_TYPE_PARAM:
      case G_TYPE_OBJECT:
        return cInstantiatable;
      case G_TYPE_BOXED:
      case G_TYPE_POINTER:
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
              cinfo_super = rbgobj_class_info_lookup_by_gtype(parent_type);
              if (!cinfo_super) {
                  cinfo_super = rbgobj_class_info_define_without_lock(parent_type,
                                                                      NULL,
                                                                      module,
                                                                      Qnil);
              }
              return cinfo_super->klass;
          }
      }
    }
}

static RGObjClassInfo *
rbgobj_class_info_define_without_lock(GType gtype,
                                      const gchar *name,
                                      VALUE module,
                                      VALUE parent)
{
    GType fundamental_type;
    VALUE klass;

    fundamental_type = G_TYPE_FUNDAMENTAL(gtype);
    switch (fundamental_type){
    case G_TYPE_POINTER:
    case G_TYPE_BOXED:
    case G_TYPE_PARAM:
    case G_TYPE_OBJECT:
    case G_TYPE_ENUM:
    case G_TYPE_FLAGS:
        if (NIL_P(parent)) parent = get_superclass(gtype, module);
        klass = rb_funcall(rb_cClass, id_new, 1, parent);
        break;

    case G_TYPE_INTERFACE:
        klass = rb_module_new();
        break;

    default:
      if (NIL_P(parent)) parent = get_superclass(gtype, module);
      if (NIL_P(parent)) {
          fprintf(stderr,
                  "rbgobj_class_info_define: "
                  "Unsupported fundamental type: <%s>(%s)\n",
                  g_type_name(fundamental_type),
                  g_type_name(gtype));
          return NULL;
      }
      klass = rb_funcall(rb_cClass, id_new, 1, parent);
    }

    if (name) {
        rb_define_const(module, name, klass);
    }
    return rbgobj_class_info_register_without_lock(gtype, klass);
}

static VALUE
rbgobj_class_info_define_body(VALUE data)
{
    RGObjDefineClassData *cdata = (RGObjDefineClassData *)data;
    const RGObjClassInfo *cinfo;

    cinfo = rbgobj_class_info_define_without_lock(cdata->gtype,
                                                  cdata->name,
                                                  cdata->module,
                                                  cdata->parent);
    return (VALUE)cinfo;
}

static VALUE
rbgobj_class_info_define_ensure(G_GNUC_UNUSED VALUE data)
{
    rb_funcall(lookup_class_mutex, id_unlock, 0);
    return Qundef;
}

RGObjClassInfo *
rbgobj_class_info_define(GType gtype,
                         const gchar *name,
                         VALUE module,
                         VALUE parent)
{
    RGObjDefineClassData data;

    data.gtype = gtype;
    data.name = name;
    data.module = module;
    data.parent = parent;

    rb_funcall(lookup_class_mutex, id_lock, 0);
    return (RGObjClassInfo *)rb_ensure(rbgobj_class_info_define_body,
                                       (VALUE)&data,
                                       rbgobj_class_info_define_ensure,
                                       (VALUE)&data);
}

/* deprecated */
const RGObjClassInfo *
rbgobj_lookup_class(VALUE klass)
{
    return rbgobj_class_info_lookup(klass);
}

const RGObjClassInfo *
rbgobj_class_info_lookup(VALUE klass)
{
    VALUE data = rb_hash_aref(klass_to_cinfo, klass);
    if (!NIL_P(data)) {
        RGObjClassInfo *cinfo;
        if (RTYPEDDATA_P(data)) {
            TypedData_Get_Struct(data,
                                 RGObjClassInfo,
                                 RTYPEDDATA_TYPE(data),
                                 cinfo);
        } else {
            Data_Get_Struct(data, RGObjClassInfo, cinfo);
        }
        return cinfo;
    }

    if (TYPE(klass) == T_CLASS) {
        VALUE super;
        super = rb_funcall(klass, id_superclass, 0);
        return rbgobj_class_info_lookup(super);
    }

    rb_raise(rb_eRuntimeError, "can't get gobject class information");
    return NULL;
}

const RGObjClassInfo *
rbgobj_class_info_lookup_by_gtype(GType gtype)
{
    if (gtype == G_TYPE_INVALID)
        return NULL;

    return g_hash_table_lookup(gtype_to_cinfo, GUINT_TO_POINTER(gtype));
}

/* deprecated */
const RGObjClassInfo *
rbgobj_lookup_class_by_gtype(GType gtype, VALUE parent)
{
    return rbgobj_lookup_class_by_gtype_full(gtype, parent, TRUE);
}

/* deprecated */
const RGObjClassInfo *
rbgobj_lookup_class_by_gtype_full(GType gtype, VALUE parent,
                                  gboolean create_class)
{
    const RGObjClassInfo *info;

    if (gtype == G_TYPE_INVALID)
        return NULL;

    info = rbgobj_class_info_lookup_by_gtype(gtype);
    if (info)
        return info;

    if (!create_class)
        return NULL;

    return rbgobj_class_info_define(gtype, NULL, rb_cObject, parent);
}

VALUE
rbgobj_gtype_to_ruby_class(GType gtype)
{
    const RGObjClassInfo *cinfo;

    cinfo = GTYPE2CINFO(gtype);
    return cinfo ? cinfo->klass : Qnil;
}

GType
rbgobj_gtype_from_ruby(VALUE rb_gtype)
{
    if (NIL_P(rb_gtype)) {
        return G_TYPE_NONE;
    } else if (RB_TYPE_P(rb_gtype, RUBY_T_STRING)) {
        GType gtype;
        gtype = g_type_from_name(RVAL2CSTR(rb_gtype));
        if (gtype == G_TYPE_INVALID) {
            rb_raise(rb_eArgError,
                     "unknown GType name: <%s>",
                     RVAL2CSTR(rb_gtype));
        }
        return gtype;
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_gtype, RG_TARGET_NAMESPACE))) {
        return NUM2SIZET(rb_ivar_get(rb_gtype, id_gtype));
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_gtype, rb_cClass))) {
        return CLASS2GTYPE(rb_gtype);
    } else {
        return NUM2SIZET(rb_to_int(rb_gtype));
    }
}

VALUE
rbgobj_define_class(GType gtype, const gchar *name, VALUE module,
                    RGMarkFunc mark, RGFreeFunc free, VALUE parent)
{
    RGObjClassInfo *cinfo;

    if (gtype == G_TYPE_INVALID)
        rb_bug("rbgobj_define_class: Invalid GType: <%s>\n", name);

    cinfo = (RGObjClassInfo *)rbgobj_class_info_lookup_by_gtype(gtype);
    if (cinfo) {
        if (!rb_const_defined_at(module, rb_intern(name))) {
            rb_define_const(module, name, cinfo->klass);
            rbgobj_class_info_fill_name(cinfo);
        }
    } else {
        cinfo = rbgobj_class_info_define(gtype, name, module, parent);
    }
    cinfo->mark = mark;
    cinfo->free = free;
    return cinfo->klass;
}

void
rbgobj_register_mark_func(GType gtype, RGMarkFunc mark)
{
    RGObjClassInfo *cinfo;

    cinfo =
        (RGObjClassInfo *)rbgobj_lookup_class_by_gtype_full(gtype, Qnil, FALSE);
    if (!cinfo) {
        rb_raise(rb_eArgError,
                 "rbgobj_register_free_func(): no class is defined: <%s>",
                 g_type_name(gtype));
    }
    cinfo->mark = mark;
}

void
rbgobj_register_free_func(GType gtype, RGFreeFunc free)
{
    RGObjClassInfo *cinfo;

    cinfo =
        (RGObjClassInfo *)rbgobj_lookup_class_by_gtype_full(gtype, Qnil, FALSE);
    if (!cinfo) {
        rb_raise(rb_eArgError,
                 "rbgobj_register_free_func(): no class is defined: <%s>",
                 g_type_name(gtype));
    }
    cinfo->free = free;
}

VALUE
rbgobj_define_class_dynamic(const gchar *gtype_name, const gchar *name,
                            VALUE module, RGMarkFunc mark, RGFreeFunc free)
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
    rb_data_type_t *data_type = NULL;
    RGObjClassInfo *cinfo = NULL;
    VALUE c = Qnil;

    if (klass2gtype) {
        data_type = rbgobj_class_info_create_data_type(klass);
        c = TypedData_Make_Struct(rb_cObject, RGObjClassInfo, data_type, cinfo);
    }
    if (gtype2klass && !cinfo)
        cinfo = g_new(RGObjClassInfo, 1);

    if (cinfo) {
        cinfo->klass = klass;
        cinfo->gtype = gtype;
        cinfo->mark  = NULL;
        cinfo->free  = NULL;
        cinfo->flags = 0;
        cinfo->data_type = data_type;
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
#ifndef RUBY_INTEGER_UNIFICATION
    _register_fundamental_klass_to_gtype(rb_cFixnum, G_TYPE_LONG);
#endif
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
#ifdef RUBY_INTEGER_UNIFICATION
    _register_fundamental_gtype_to_klass(G_TYPE_CHAR, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_UCHAR, rb_cInteger);
#else
    _register_fundamental_gtype_to_klass(G_TYPE_CHAR, rb_cFixnum);
    _register_fundamental_gtype_to_klass(G_TYPE_UCHAR, rb_cFixnum);
#endif
    _register_fundamental_gtype_to_klass(G_TYPE_STRING, rb_cString);
    _register_fundamental_gtype_to_klass(G_TYPE_ULONG, rb_cInteger);
    _register_fundamental_gtype_to_klass(G_TYPE_NONE, rb_cNilClass);
    _register_fundamental_gtype_to_klass(G_TYPE_BOOLEAN, rb_cTrueClass);
}

/**********************************************************************/
/* GLib::Type */

VALUE RG_TARGET_NAMESPACE;

VALUE
rbgobj_gtype_new(GType gtype)
{
    VALUE result = rb_obj_alloc(RG_TARGET_NAMESPACE);
    VALUE arg = SIZET2NUM(gtype);
    rb_obj_call_init(result, 1, &arg);
    return result;
}

/* Deprecated. Use rbgobj_gtype_from_ruby() instead. */
GType
rbgobj_gtype_get(VALUE rb_gtype)
{
    return rbgobj_gtype_from_ruby(rb_gtype);
}

static VALUE
rg_s_try_convert(VALUE self, VALUE value)
{
    ID id_new;
    CONST_ID(id_new, "new");

    if (NIL_P(value))
        return Qnil;

    if (RVAL2CBOOL(rb_obj_is_kind_of(value, RG_TARGET_NAMESPACE)))
        return value;

    if (RVAL2CBOOL(rb_obj_is_kind_of(value, rb_cInteger))) {
        GType gtype = NUM2ULONG(value);
        if (!g_type_name(gtype))
            return Qnil;
        return rb_funcall(self, id_new, 1, value);
    }

    if (RB_TYPE_P(value, RUBY_T_SYMBOL)) {
        value = rb_sym2str(value);
    }

    if (RB_TYPE_P(value, RUBY_T_STRING)) {
        GType gtype = g_type_from_name(RVAL2CSTR(value));
        if (gtype == G_TYPE_INVALID) {
            return Qnil;
        }
        return rb_funcall(self, id_new, 1, value);
    }

    if (RVAL2CBOOL(rb_obj_is_kind_of(value, rb_cClass))) {
        for (; !NIL_P(value); value = rb_funcall(value, id_superclass, 0)) {
            RGObjClassInfo *cinfo;
            VALUE data = rb_hash_aref(klass_to_cinfo, value);

            if (NIL_P(data))
                continue;

            if (RTYPEDDATA_P(data)) {
                TypedData_Get_Struct(data,
                                     RGObjClassInfo,
                                     RTYPEDDATA_TYPE(data),
                                     cinfo);
            } else {
                Data_Get_Struct(data, RGObjClassInfo, cinfo);
            }
            return rb_funcall(self, id_new, 1, ULONG2NUM(cinfo->gtype));
        }
        return Qnil;
    }

    return Qnil;
}

static VALUE
rg_initialize(VALUE self, VALUE type)
{
    GType gtype;

    if (RVAL2CBOOL(rb_obj_is_kind_of(type, rb_cInteger))) {
        gtype = NUM2SIZET(type);
        if (!g_type_name(gtype))
            gtype = G_TYPE_INVALID;
    } else {
        gtype = g_type_from_name(StringValuePtr(type));
    }

    if (G_TYPE_INVALID == gtype)
        rb_raise(rb_eArgError, "invalid GType");

    rb_ivar_set(self, id_gtype, SIZET2NUM(gtype));

    return Qnil;
}

static VALUE
rg_inspect(VALUE self)
{
    GType gtype = rbgobj_gtype_from_ruby(self);
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
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);

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
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);
        return CBOOL2RVAL(a == b);
    }
}

static VALUE
rg_operator_type_lt_eq(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);
        return CBOOL2RVAL(g_type_is_a(a, b));
    }
}

static VALUE
rg_operator_type_gt_eq(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);
        return CBOOL2RVAL(g_type_is_a(b, a));
    }
}

static VALUE
rg_operator_type_lt(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);
        return CBOOL2RVAL(g_type_is_a(a, b) && a != b);
    }
}

static VALUE
rg_operator_type_gt(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_from_ruby(self);
        GType b = rbgobj_gtype_from_ruby(other);
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
    return GTYPE2CLASS(rbgobj_gtype_from_ruby(self));
}

static VALUE
rg_fundamental(VALUE self)
{
    return rbgobj_gtype_new(G_TYPE_FUNDAMENTAL(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_fundamental_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_FUNDAMENTAL(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_derived_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVED(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_interface_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_INTERFACE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_classed_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_CLASSED(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_instantiatable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_INSTANTIATABLE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_derivable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVABLE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_deep_derivable_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_DEEP_DERIVABLE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_abstract_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_ABSTRACT(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_value_abstract_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_ABSTRACT(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_value_type_p(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_TYPE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_has_value_table(VALUE self)
{
    return CBOOL2RVAL(G_TYPE_HAS_VALUE_TABLE(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_name(VALUE self)
{
    return rb_str_new2(g_type_name(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_parent(VALUE self)
{
    GType parent = g_type_parent(rbgobj_gtype_from_ruby(self));
    return parent ? rbgobj_gtype_new(parent) : Qnil;
}

static VALUE
rg_depth(VALUE self)
{
    return UINT2NUM(g_type_depth(rbgobj_gtype_from_ruby(self)));
}

static VALUE
rg_next_base(VALUE leaf_type, VALUE root_type)
{
    GType ret = g_type_next_base(rbgobj_gtype_from_ruby(leaf_type),
                                 rbgobj_gtype_from_ruby(root_type));
    return ret ? rbgobj_gtype_new(ret) : Qnil;
}

static VALUE
rg_type_is_a_p(VALUE self, VALUE is_a_type)
{
    return CBOOL2RVAL(g_type_is_a(rbgobj_gtype_from_ruby(self),
                                  rbgobj_gtype_from_ruby(is_a_type)));
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

    types = g_type_children(rbgobj_gtype_from_ruby(self), &n_children);
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

    types = g_type_interfaces(rbgobj_gtype_from_ruby(self), &n_interfaces);
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
    g_type_query(rbgobj_gtype_from_ruby(self), &query);
    return UINT2NUM(query.class_size);
}

static VALUE
rg_instance_size(VALUE self)
{
    GTypeQuery query;
    g_type_query(rbgobj_gtype_from_ruby(self), &query);
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
    init_typemap();

    /* type */
#ifndef rb_cMutex
    rb_cMutex = rb_const_get(rb_cObject, rb_intern("Mutex"));
#endif
    id_lock = rb_intern("lock");
    id_unlock = rb_intern("unlock");
    lookup_class_mutex = rb_funcall(rb_cMutex, id_new, 0);
    rb_iv_set(mGLib, "lookup_class_mutex", lookup_class_mutex);

    dynamic_gtype_list = g_hash_table_new(g_str_hash, g_str_equal);
    id_gtype = rb_intern("__gobject_gtype__");

    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Type", rb_cObject);

    RG_DEF_SMETHOD(try_convert, 1);

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
