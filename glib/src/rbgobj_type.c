/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_type.c -

  $Author: sakai $
  $Date: 2002/08/08 15:18:08 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"
#include "st.h"

static ID id_new;
static ID id_superclass;
static VALUE gtype_to_cinfo;
static VALUE klass_to_cinfo;

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

    if (TYPE(klass) == T_CLASS)
        return rbgobj_lookup_class(rb_funcall(klass, id_superclass, 0));

    rb_raise(rb_eRuntimeError, "can't get gobject class infomation");    
}

static VALUE
interface_s_get_gtype(interface)
    VALUE interface;
{
    return rbgobj_gtype_new(rbgobj_lookup_class(interface)->gtype);
}

const RGObjClassInfo *
rbgobj_lookup_class_by_gtype(gtype)
    GType gtype;
{
    RGObjClassInfo* cinfo;
    VALUE c = rb_hash_aref(gtype_to_cinfo, INT2NUM(gtype));

    if (!NIL_P(c)){
        Data_Get_Struct(c, RGObjClassInfo, cinfo);
    } else {
        c = Data_Make_Struct(rb_cData, RGObjClassInfo, cinfo_mark, free, cinfo);

        switch (G_TYPE_FUNDAMENTAL(gtype)){
          case G_TYPE_PARAM:
            if (gtype == G_TYPE_PARAM){
                cinfo->klass = rb_funcall(rb_cClass, id_new, 0);
            } else {
                const RGObjClassInfo* cinfo_super
                    = rbgobj_lookup_class_by_gtype(g_type_parent(gtype));
                cinfo->klass = rb_funcall(rb_cClass, id_new, 1, cinfo_super->klass);
            }
            break;

          case G_TYPE_OBJECT:
            if (gtype == G_TYPE_OBJECT){
                cinfo->klass = rb_funcall(rb_cClass, id_new, 0);
            } else {
                const RGObjClassInfo* cinfo_super
                    = rbgobj_lookup_class_by_gtype(g_type_parent(gtype));
                cinfo->klass = rb_funcall(rb_cClass, id_new, 1, cinfo_super->klass);
            }
            break;

          case G_TYPE_INTERFACE:
            cinfo->klass = rb_module_new();
            rb_define_singleton_method(cinfo->klass, "gtype", interface_s_get_gtype, 0);
            break;

          default:
            /* we should raise exception? */
            fprintf(stderr,
                    "%s: %s's fundamental type %s isn't supported\n",
                    "rbgobj_lookup_class_by_gtype",
                    g_type_name(gtype),
                    g_type_name(G_TYPE_FUNDAMENTAL(gtype)));
            return NULL;
        }

        cinfo->gtype = gtype;
        cinfo->mark  = NULL;
        cinfo->free  = NULL;

        rb_hash_aset(klass_to_cinfo, cinfo->klass, c);
        rb_hash_aset(gtype_to_cinfo, INT2NUM(gtype), c);

        if (G_TYPE_FUNDAMENTAL(gtype) == G_TYPE_OBJECT){
            GType* interfaces = NULL;
            guint n_interfaces = 0;
            int i;

            interfaces = g_type_interfaces(gtype, &n_interfaces);
            for (i = 0; i < n_interfaces; i++){
                rb_include_module(
                    cinfo->klass,
                    rbgobj_lookup_class_by_gtype(interfaces[i])->klass);
            }
            g_free(interfaces);

            rbgobj_define_signal_constants(cinfo->klass);
            rbgobj_define_property_accessors(cinfo->klass);
        }
    }

    return cinfo;
}

VALUE
rbgobj_define_class(gtype, name, module, mark, free)
	GType gtype;
	gchar* name;
	VALUE module;
	void* mark;
	void* free;
{
        RGObjClassInfo* cinfo = (RGObjClassInfo*)rbgobj_lookup_class_by_gtype(gtype);
        cinfo->mark = mark;
        cinfo->free = free;
        rb_define_const(module, name, cinfo->klass);
        return cinfo->klass;
}

/**********************************************************************/

VALUE rbgobj_cType;
static ID id_gtype;

VALUE
rbgobj_gtype_new(gtype)
    GType gtype;
{
    VALUE result;

    if (G_TYPE_INVALID == gtype)
        rb_raise(rb_eArgError, "invalid type");
    /* FIXME: more check */

    result = rb_obj_alloc(rbgobj_cType);
    rb_ivar_set(result, id_gtype, UINT2NUM(gtype));
    return result;
}

GType
rbgobj_gtype_get(self)
    VALUE self;
{
    if (!RTEST(rb_obj_is_kind_of(self, rbgobj_cType)))
        rb_raise(rb_eTypeError, "Not a GLib::Type");
    return NUM2ULONG(rb_ivar_get(self, id_gtype));
}

static VALUE
type_s_new(klass, type)
    VALUE klass, type;
{
    GType gtype;

    if (RTEST(rb_obj_is_kind_of(type, rb_cInteger))) {
        gtype = NUM2UINT(type);
    } else {
        StringValue(type);
        gtype = g_type_from_name(StringValuePtr(type));
    }
 
    return rbgobj_gtype_new(gtype);
}

static VALUE
type_inspect(self)
    VALUE self;
{
    GType gtype = rbgobj_gtype_get(self);
    gchar* str;
    VALUE result;

    str = g_strdup_printf("#<GLib::Type %ld name=\"%s\">",
                          gtype, g_type_name(gtype));
    result = rb_str_new2(str);
    g_free(str);

    return result;
}

static VALUE
type_eq(self, other)
    VALUE self, other;
{
    if (RTEST(rb_obj_is_kind_of(other, rbgobj_cType)) &&
        rbgobj_gtype_get(self) == rbgobj_gtype_get(other))
        return Qtrue;
    else
        return Qfalse;
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
    return G_TYPE_IS_FUNDAMENTAL(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_derived(self)
    VALUE self;
{
    return G_TYPE_IS_DERIVED(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_interface(self)
    VALUE self;
{
    return G_TYPE_IS_INTERFACE(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_classed(self)
    VALUE self;
{
    return G_TYPE_IS_CLASSED(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_instantiatable(self)
    VALUE self;
{
    return G_TYPE_IS_INSTANTIATABLE(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_derivable(self)
    VALUE self;
{
    return G_TYPE_IS_DERIVABLE(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_deep_derivable(self)
    VALUE self;
{
    return G_TYPE_IS_DEEP_DERIVABLE(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_abstract(self)
    VALUE self;
{
    return G_TYPE_IS_ABSTRACT(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_value_abstract(self)
    VALUE self;
{
    return G_TYPE_IS_VALUE_ABSTRACT(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_is_value_type(self)
    VALUE self;
{
    return G_TYPE_IS_VALUE_ABSTRACT(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
}

static VALUE
type_has_value_table(self)
    VALUE self;
{
    return G_TYPE_HAS_VALUE_TABLE(rbgobj_gtype_get(self)) ? Qtrue : Qfalse;
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
    return g_type_is_a(rbgobj_gtype_get(self), rbgobj_gtype_get(is_a_type)) ? Qtrue : Qfalse;
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
    id_gtype = rb_intern("__gobject_gtype__");

    rbgobj_cType = rb_define_class_under(mGLib, "Type", rb_cData);

    rb_define_singleton_method(rbgobj_cType, "new", type_s_new, 1);
    rb_define_method(rbgobj_cType, "inspect", type_inspect, 0);
    rb_define_method(rbgobj_cType, "==", type_eq, 1);
    rb_define_method(rbgobj_cType, "eql?", type_eq, 1);
    rb_define_method(rbgobj_cType, "hash", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_i", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_int", type_to_int, 0);
    rb_define_method(rbgobj_cType, "to_class", type_to_class, 0);

    rb_define_method(rbgobj_cType, "fundamental", type_fundamental, 0);
    rb_define_method(rbgobj_cType, "fundamental?", type_is_fundamental, 0);
    rb_define_method(rbgobj_cType, "derived?", type_is_derived, 0);
    rb_define_method(rbgobj_cType, "interfaced?", type_is_interface, 0);
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

/*
 * Init
 */
void Init_gobject_gtype()
{
    g_type_init();

    id_new = rb_intern("new");
    id_superclass = rb_intern("superclass");

    rb_global_variable(&gtype_to_cinfo);
    rb_global_variable(&klass_to_cinfo);
    gtype_to_cinfo = rb_hash_new();
    klass_to_cinfo = rb_hash_new();

    Init_type();
}
