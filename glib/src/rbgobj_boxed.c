/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_boxed.c -

  $Author: sakai $
  $Date: 2003/02/14 18:55:51 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static GHashTable* boxed_table;

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
    
    if (p->own && p->boxed)
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
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif

static VALUE
rbgobj_boxed_s_gtype(klass)
    VALUE klass;
{
    return rbgobj_gtype_new(rbgobj_lookup_class(klass)->gtype);
}

static VALUE
rbgobj_boxed_gtype(self)
    VALUE self;
{
    return rbgobj_boxed_s_gtype(CLASS_OF(self));
}

static VALUE
rbgobj_boxed_copy(self)
    VALUE self;
{
    boxed_holder* holder1;
    boxed_holder* holder2;
    VALUE result = rbgobj_boxed_create(CLASS_OF(self));

    Data_Get_Struct(self, boxed_holder, holder1);
    Data_Get_Struct(result, boxed_holder, holder2);

    holder2->boxed = g_boxed_copy(holder1->type, holder1->boxed);
    holder2->own   = TRUE;

    return result;
}

/**********************************************************************/

VALUE
rbgobj_boxed_create(klass)
    VALUE klass;
{
    boxed_holder* holder;

    const RGObjClassInfo *cinfo = rbgobj_lookup_class(klass);
    VALUE result = Data_Make_Struct(klass, boxed_holder, 
                                    boxed_mark, boxed_free, holder);
    holder->type  = cinfo->gtype;
    holder->boxed = NULL;
    holder->own   = FALSE;

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
    holder->own   = TRUE;
}

gpointer
rbgobj_boxed_get(obj, gtype)
    VALUE obj, gtype;
{
    boxed_holder* holder;
    Data_Get_Struct(obj, boxed_holder, holder);

    if (!RTEST(rb_obj_is_kind_of(obj, GTYPE2CLASS(gtype))))
        rb_raise(rb_eArgError, "invalid argument %s (expect %s)",
                 rb_class2name(CLASS_OF(obj)), 
                 rb_class2name(GTYPE2CLASS(gtype)));

    if (!holder->boxed)
        rb_raise(rb_eArgError, "uninitialize %s", rb_class2name(CLASS_OF(obj)));

    return holder->boxed;
}

VALUE
rbgobj_make_boxed(p, gtype)
    gpointer p;
    GType gtype;
{
    VALUE result = rbgobj_boxed_create(GTYPE2CLASS(gtype));
    boxed_holder* holder;
    gboolean copy_obj = (g_hash_table_lookup(boxed_table, (gconstpointer)gtype) == NULL);

    Data_Get_Struct(result, boxed_holder, holder);
    
    if (copy_obj){
        holder->boxed = g_boxed_copy(gtype, p);
        holder->own   = TRUE;
    } else {
        holder->boxed = p;
        holder->own   = FALSE;
    }

    return result;
}

void
rbgobj_boxed_not_copy_obj(gtype)
    GType gtype;
{
    g_hash_table_insert(boxed_table, (gpointer)gtype, (gpointer)gtype);
}

/**********************************************************************/

static VALUE
boxed_to_ruby(const GValue* from)
{
    return rbgobj_make_boxed(g_value_get_boxed(from),
                             G_VALUE_TYPE(from));
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

    boxed_table = g_hash_table_new(NULL, NULL);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(gBoxed, "allocate", rbgobj_boxed_s_allocate, 0);
#else
    rb_define_alloc_func(gBoxed, rbgobj_boxed_s_allocate);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(gBoxed, "new", rbgobj_boxed_s_new, -1);
#endif

    rb_define_singleton_method(gBoxed, "gtype", rbgobj_boxed_s_gtype, 0);
    rb_define_method(gBoxed, "gtype", rbgobj_boxed_gtype, 0);
    rb_define_method(gBoxed, "copy", rbgobj_boxed_copy, 0);
    rb_define_alias(gBoxed, "clone", "copy");
}
