/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_boxed.c -

  $Author: sakai $
  $Date: 2002/08/13 17:56:41 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

typedef struct {
    gpointer boxed;
    GType type;
} boxed_holder;

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
    G_DEF_CLASS(G_TYPE_BOXED, "Boxed", mGLib);
    rbgobj_register_g2r_func(G_TYPE_BOXED, boxed_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_BOXED, boxed_from_ruby);
}
