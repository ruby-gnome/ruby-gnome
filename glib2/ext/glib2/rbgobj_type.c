/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_type.c -

  $Author: sakai $
  $Date: 2007/07/16 03:35:53 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002-2009  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "rbgprivate.h"

/**********************************************************************/
/* Type Mapping */

#ifdef RBGOBJ_USE_DLPTR
#include "dl.h"
#endif

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
rbgobj_lookup_class(klass)
    VALUE klass;
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
#ifdef RBGOBJ_USE_DLPTR
        return rb_cDLPtrData;
#else
        return rb_cData;
#endif
      case G_TYPE_ENUM:
      case G_TYPE_FLAGS:
        return rb_cObject;
      default:
      {
          const RGObjClassInfo *cinfo_super;
	  GType parent_type;

	  parent_type = g_type_parent(gtype);
	  cinfo_super = rbgobj_lookup_class_by_gtype_without_lock(parent_type,
								  Qnil,
								  TRUE);
          return cinfo_super->klass;
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
        int i;

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
rbgobj_lookup_class_by_gtype_ensure(VALUE data)
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
rbgobj_define_class(gtype, name, module, mark, free, parent)
    GType gtype;
    const gchar* name;
    VALUE module;
    void* mark;
    void* free;
    VALUE parent;
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
rbgobj_define_class_dynamic(gtype_name, name, module, mark, free)
    const gchar* gtype_name;
    const gchar* name;
    VALUE module;
    void* mark;
    void* free;
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
Init_typemap()
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

VALUE rbgobj_cType;
static ID id_gtype;

VALUE
rbgobj_gtype_new(gtype)
    GType gtype;
{
    VALUE result = rb_obj_alloc(rbgobj_cType);
    VALUE arg = ULONG2NUM(gtype);
    rb_obj_call_init(result, 1, &arg);
    return result;
}

GType
rbgobj_gtype_get(self)
    VALUE self;
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(self, rbgobj_cType))) {
        return NUM2ULONG(rb_ivar_get(self, id_gtype));
    } else {
        return CLASS2GTYPE(self);
    }
    rb_raise(rb_eTypeError, "Not a GLib::Type");
}

static VALUE
type_initialize(self, type)
    VALUE self, type;
{
    GType gtype;

    if (RVAL2CBOOL(rb_obj_is_kind_of(type, rb_cInteger))) {
        gtype = NUM2UINT(type);
        // XXX
        if (!g_type_name(gtype))
            gtype = G_TYPE_INVALID;
    } else {
        gtype = g_type_from_name(StringValuePtr(type));
    }

    if (G_TYPE_INVALID == gtype)
        rb_raise(rb_eArgError, "invalid type");

    rb_ivar_set(self, id_gtype, UINT2NUM(gtype));
 
    return Qnil;
}

static VALUE
type_inspect(self)
    VALUE self;
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
type_compare(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
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
type_eq(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(a == b);
    }
}

static VALUE
type_lt_eq(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(a, b));
    }
}

static VALUE
type_gt_eq(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(b, a));
    }
}

static VALUE
type_lt(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(a, b) && a != b);
    }
}

static VALUE
type_gt(self, other)
    VALUE self, other;
{
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, rbgobj_cType)))
        return Qnil;
    else {
        GType a = rbgobj_gtype_get(self);
        GType b = rbgobj_gtype_get(other);
        return CBOOL2RVAL(g_type_is_a(b, a) && a != b);
    }
}

static VALUE
type_to_int(self)
    VALUE self;
{
    return rb_ivar_get(self, id_gtype);
}

static VALUE
type_to_class(self)
    VALUE self;
{
    return GTYPE2CLASS(rbgobj_gtype_get(self));
}

static VALUE
type_fundamental(self)
    VALUE self;
{
    return rbgobj_gtype_new(G_TYPE_FUNDAMENTAL(rbgobj_gtype_get(self)));
}

static VALUE
type_is_fundamental(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_FUNDAMENTAL(rbgobj_gtype_get(self)));
}

static VALUE
type_is_derived(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVED(rbgobj_gtype_get(self)));
}

static VALUE
type_is_interface(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_INTERFACE(rbgobj_gtype_get(self)));
}

static VALUE
type_is_classed(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_CLASSED(rbgobj_gtype_get(self)));
}

static VALUE
type_is_instantiatable(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_INSTANTIATABLE(rbgobj_gtype_get(self)));
}

static VALUE
type_is_derivable(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_DERIVABLE(rbgobj_gtype_get(self)));
}

static VALUE
type_is_deep_derivable(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_DEEP_DERIVABLE(rbgobj_gtype_get(self)));
}

static VALUE
type_is_abstract(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_ABSTRACT(rbgobj_gtype_get(self)));
}

static VALUE
type_is_value_abstract(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_ABSTRACT(rbgobj_gtype_get(self)));
}

static VALUE
type_is_value_type(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_IS_VALUE_TYPE(rbgobj_gtype_get(self)));
}

static VALUE
type_has_value_table(self)
    VALUE self;
{
    return CBOOL2RVAL(G_TYPE_HAS_VALUE_TABLE(rbgobj_gtype_get(self)));
}

static VALUE
type_name(self)
    VALUE self;
{
    return rb_str_new2(g_type_name(rbgobj_gtype_get(self)));
}

static VALUE
type_parent(self)
    VALUE self;
{
    GType parent = g_type_parent(rbgobj_gtype_get(self));
    return parent ? rbgobj_gtype_new(parent) : Qnil;
}

static VALUE
type_depth(self)
    VALUE self;
{
    return UINT2NUM(g_type_depth(rbgobj_gtype_get(self)));
}

static VALUE
type_next_base(leaf_type, root_type)
    VALUE leaf_type, root_type;
{
    GType ret = g_type_next_base(rbgobj_gtype_get(leaf_type),
                                 rbgobj_gtype_get(root_type));
    return ret ? rbgobj_gtype_new(ret) : Qnil;
}

static VALUE
type_is_a(self, is_a_type)
    VALUE self, is_a_type;
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
type_children(self)
    VALUE self;
{
    guint n_children;
    GType* types;
    VALUE result;
    int i;

    types = g_type_children(rbgobj_gtype_get(self), &n_children);
    result = rb_ary_new2(n_children);
    for (i = 0; i < n_children; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(types[i]));
    g_free(types);

    return result;
}

static VALUE
type_interfaces(self)
    VALUE self;
{
    guint n_interfaces;
    GType* types;
    VALUE result;
    int i;

    types = g_type_interfaces(rbgobj_gtype_get(self), &n_interfaces);
    result = rb_ary_new2(n_interfaces);
    for (i = 0; i < n_interfaces; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(types[i]));
    g_free(types);

    return result;
}

static VALUE
type_class_size(self)
    VALUE self;
{
    GTypeQuery query;
    g_type_query(rbgobj_gtype_get(self), &query);
    return UINT2NUM(query.class_size);
}

static VALUE
type_instance_size(self)
    VALUE self;
{
    GTypeQuery query;
    g_type_query(rbgobj_gtype_get(self), &query);
    return UINT2NUM(query.instance_size);
}


static inline
void _def_fundamental_type(VALUE ary, GType gtype, const char* name)
{
    VALUE c = rbgobj_gtype_new(gtype);
    rb_define_const(rbgobj_cType, name, c);
    rb_ary_push(ary, c);
}

static void
Init_type()
{
    rb_cMutex = rb_const_get(rb_cObject, rb_intern("Mutex"));
    id_lock = rb_intern("lock");
    id_unlock = rb_intern("unlock");
    lookup_class_mutex = rb_funcall(rb_cMutex, id_new, 0);
    rb_iv_set(mGLib, "lookup_class_mutex", lookup_class_mutex);

    dynamic_gtype_list = g_hash_table_new(g_str_hash, g_str_equal);
    id_gtype = rb_intern("__gobject_gtype__");

    rbgobj_cType = rb_define_class_under(mGLib, "Type", rb_cObject);

    rb_define_alias(CLASS_OF(rbgobj_cType), "[]", "new");
    rb_define_method(rbgobj_cType, "initialize", type_initialize, 1);
    rb_define_method(rbgobj_cType, "inspect", type_inspect, 0);
    rb_define_method(rbgobj_cType, "<=>", type_compare, 1);
    rb_define_method(rbgobj_cType, "==", type_eq, 1);
    rb_define_method(rbgobj_cType, "<=", type_lt_eq, 1);
    rb_define_method(rbgobj_cType, ">=", type_gt_eq, 1);
    rb_define_method(rbgobj_cType, "<", type_lt, 1);
    rb_define_method(rbgobj_cType, ">", type_gt, 1);
    rb_define_method(rbgobj_cType, "eql?", type_eq, 1);
    rb_define_method(rbgobj_cType, "hash", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_i", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_int", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_class", type_to_class, 0);

    rb_define_method(rbgobj_cType, "fundamental", type_fundamental, 0);
    rb_define_method(rbgobj_cType, "fundamental?", type_is_fundamental, 0);
    rb_define_method(rbgobj_cType, "derived?", type_is_derived, 0);
    rb_define_method(rbgobj_cType, "interface?", type_is_interface, 0);
    rb_define_method(rbgobj_cType, "classed?", type_is_classed, 0);
    rb_define_method(rbgobj_cType, "instantiatable?", type_is_instantiatable, 0);
    rb_define_method(rbgobj_cType, "derivable?", type_is_derivable, 0);
    rb_define_method(rbgobj_cType, "deep_derivable?", type_is_deep_derivable, 0);
    rb_define_method(rbgobj_cType, "abstract?", type_is_abstract, 0);
    rb_define_method(rbgobj_cType, "value_abstract?", type_is_value_abstract, 0);
    rb_define_method(rbgobj_cType, "value_type?", type_is_value_type, 0);
    rb_define_method(rbgobj_cType, "has_value_table", type_has_value_table, 0);

    rb_define_method(rbgobj_cType, "name", type_name, 0);
    rb_define_method(rbgobj_cType, "to_s", type_name, 0);
    rb_define_method(rbgobj_cType, "parent", type_parent, 0);
    rb_define_method(rbgobj_cType, "depth", type_depth, 0);
    rb_define_method(rbgobj_cType, "next_base", type_next_base, 1);
    rb_define_method(rbgobj_cType, "type_is_a?", type_is_a, 1);
    rb_define_method(rbgobj_cType, "children", type_children, 0);
    rb_define_method(rbgobj_cType, "interfaces", type_interfaces, 0);
    rb_define_method(rbgobj_cType, "class_size", type_class_size, 0);
    rb_define_method(rbgobj_cType, "instance_size", type_instance_size, 0);

    {
    VALUE ary = rb_ary_new();
    rb_define_const(rbgobj_cType, "FUNDAMENTAL_MAX", INT2FIX(G_TYPE_FUNDAMENTAL_MAX));
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
    rb_define_const(rbgobj_cType, "FUNDAMENTAL_TYPES", ary); /* FIXME: better name */
    }
}

/**********************************************************************/

void Init_gobject_gtype()
{
    g_type_init();
    Init_typemap();
    Init_type();
}
