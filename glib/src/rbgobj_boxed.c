/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_boxed.c -

  $Author: sakai $
  $Date: 2002/07/28 11:34:21 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

VALUE rbgobj_cBoxed;

/**********************************************************************/

typedef struct {
    gpointer boxed;
    GType type;
} boxed_holder;

static void
boxed_free(boxed_holder* p)
{
    g_boxed_free(p->type, p->boxed);
    free(p);
}

/**********************************************************************/

gpointer
rbgobj_boxed_get(self)
    VALUE self;
{
    boxed_holder* holder;
    Data_Get_Struct(self, boxed_holder, holder);
    return holder->boxed;
}

VALUE
rbgobj_make_boxed(p, gtype)
    gpointer p;
    GType gtype;
{
    boxed_holder* holder;
    VALUE result;

    result = Data_Make_Struct(rbgobj_boxed_class(gtype), boxed_holder,
                              NULL, boxed_free, holder);
    holder->type  = gtype;
    holder->boxed = g_boxed_copy(gtype, p);

    return result;
}

/**********************************************************************/

static VALUE gtype_to_class;

VALUE
rbgobj_boxed_class(gtype)
    GType gtype;
{
    VALUE type = INT2NUM(gtype);
    VALUE klass;

    if (!G_TYPE_IS_BOXED(gtype))
        rb_raise(rb_eRuntimeError, "%s is not a subtype of GBoxed",
                 g_type_name(gtype));

    klass = rb_hash_aref(gtype_to_class, type);

    if (NIL_P(klass)){
        VALUE parent = rbgobj_boxed_class(g_type_parent(gtype));
        klass = rb_class_new(parent);
        rb_hash_aset(gtype_to_class, type, klass);
        rb_iv_set(klass, "@gtype", type);
        return klass;
    }

    return klass;
}

GType
rbgobj_boxed_class_gtype(klass)
    VALUE klass;
{
    return NUM2INT(rb_iv_get(klass, "@gtype"));
}

/**********************************************************************/

static VALUE
boxed_to_ruby(const GValue* from)
{
    gpointer boxed = g_value_dup_boxed(from);
    boxed_holder* holder;
    GType gtype = G_VALUE_TYPE(from);
    VALUE result;

    result = Data_Make_Struct(rbgobj_boxed_class(gtype), boxed_holder,
                              NULL, boxed_free, holder);
    holder->type  = gtype;
    holder->boxed = boxed;

    return result;
}

static void
boxed_from_ruby(VALUE from, GValue* to)
{
    boxed_holder* holder;
    Data_Get_Struct(from, boxed_holder, holder);

    if (g_type_is_a(holder->type, G_VALUE_TYPE(to)))
        g_value_set_boxed(to, holder->boxed);
    else
        rb_raise(rb_eRuntimeError, "%s is not a subtype of %s",
                 g_type_name(holder->type),
                 g_type_name(G_VALUE_TYPE(to)));
}

/**********************************************************************/

void
Init_gobject_gboxed()
{
    rb_global_variable(&gtype_to_class);
    gtype_to_class = rb_hash_new();

    rbgobj_cBoxed = rb_define_class_under(mGLib, "Boxed", rb_cData);
    rb_hash_aset(gtype_to_class, INT2NUM(G_TYPE_BOXED), rbgobj_cBoxed);    
    rb_iv_set(rbgobj_cBoxed, "@gtype", INT2NUM(G_TYPE_BOXED));

    rbgobj_register_g2r_func(G_TYPE_BOXED, boxed_to_ruby);
    rbgobj_register_r2g_func(rbgobj_cBoxed, boxed_from_ruby);
}
