/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_boxed.c -

  $Author: sakai $
  $Date: 2002/09/01 13:19:21 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static void
boxed_mark(boxed_holder* p)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class_by_gtype(p->type);
    if (cinfo && cinfo->mark)
        cinfo->mark(p->boxed);
}

static void
boxed_free(boxed_holder* p)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class_by_gtype(p->type);
    if (cinfo && cinfo->free)
        cinfo->free(p->boxed);
    g_boxed_free(p->type, p->boxed);
    free(p);
}

/**********************************************************************/
static VALUE
rbgobj_boxed_s_allocate(klass)
	VALUE klass;
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(klass);
    if (cinfo->gtype == G_TYPE_BOXED)
        rb_raise(rb_eTypeError, "abstract class");

	return rbgobj_create_object(klass);
}
#ifdef HAVE_OBJECT_ALLOCATE
#define rbgobj_boxed_s_new rb_class_new_instance
#else
static VALUE
rbgobj_boxed_s_new(argc, argv, klass)
    int argc;
    VALUE* argv;
    VALUE klass;
{
	VALUE obj = rbgobj_boxed_s_allocate(klass);
	rb_obj_call_init(obj, argc ,argv);
    return obj;
}
#endif

VALUE
rbgobj_boxed_create(klass)
    VALUE klass;
{
    boxed_holder* holder;

	const RGObjClassInfo *cinfo = rbgobj_lookup_class(klass);
    VALUE result = Data_Make_Struct(klass, boxed_holder, 
							  boxed_mark, boxed_free, holder);
    holder->type = cinfo->gtype;
    holder->boxed = (gpointer)NULL;

    return result;
}

void
rbgobj_boxed_initialize(obj, boxed)
    VALUE obj;
    gpointer boxed;
{ 
    boxed_holder* holder;
    Data_Get_Struct(obj, boxed_holder, holder);
    holder->boxed = g_boxed_copy(holder->type, boxed);

    rb_ivar_set(obj, id_relatives, Qnil);
}

gpointer
rbgobj_boxed_get(self)
    VALUE self;
{
    boxed_holder* holder;
    Data_Get_Struct(self, boxed_holder, holder);

    if (!holder->boxed)
        rb_raise(rb_eArgError, "uninitialize %s", rb_class2name(CLASS_OF(self)));

    return holder->boxed;
}

VALUE
rbgobj_make_boxed(p, gtype)
    gpointer p;
    GType gtype;
{
    boxed_holder* holder;
    VALUE result;

    result = Data_Make_Struct(GTYPE2CLASS(gtype), boxed_holder,
                              boxed_mark, boxed_free, holder);
    holder->type  = gtype;
    holder->boxed = g_boxed_copy(gtype, p);

    return result;
}

/**********************************************************************/

static VALUE
boxed_to_ruby(const GValue* from)
{
    gpointer boxed = g_value_dup_boxed(from);
    boxed_holder* holder;
    GType gtype = G_VALUE_TYPE(from);
    VALUE result;

    result = Data_Make_Struct(GTYPE2CLASS(gtype), boxed_holder,
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
    VALUE gBoxed = G_DEF_CLASS(G_TYPE_BOXED, "Boxed", mGLib);
    rbgobj_register_g2r_func(G_TYPE_BOXED, boxed_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_BOXED, boxed_from_ruby);

    rb_define_singleton_method(gBoxed, "allocate", rbgobj_boxed_s_allocate, 0);
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(gBoxed, "new", rbgobj_boxed_s_new, -1);
#endif
}
