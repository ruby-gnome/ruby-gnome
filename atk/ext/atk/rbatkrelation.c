/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkrelation.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_RELATION(RVAL2GOBJ(s)))

static VALUE
rbatkrel_s_type_register(VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_relation_type_register(RVAL2CSTR(name)), ATK_TYPE_RELATION_TYPE);
}

/* We don't need them
G_CONST_RETURN gchar* atk_relation_type_get_name
                                            (AtkRelationType type);
*/

static VALUE
rbatkrelation_s_for_name(VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_relation_type_for_name(RVAL2CSTR(name)), ATK_TYPE_RELATION_TYPE);
}


static VALUE
rbatkrel_initialize(VALUE self, VALUE targets, VALUE relationship)
{
    VALUE ary = rb_ary_to_ary(targets);
    long i;
    long len = RARRAY_LEN(ary);
    AtkObject **objects = g_new(AtkObject *, len);

    for (i = 0; i < len; i++)
        objects[i] = RVAL2ATKOBJECT(RARRAY_PTR(targets)[i]);

    G_INITIALIZE(self, atk_relation_new(objects,
                                        len, 
                                        RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE)));

    g_free(objects);

    return Qnil;
}

#if ATK_CHECK_VERSION(1,9,0)
static VALUE
rbatkrel_add_target(VALUE self, VALUE obj)
{
    atk_relation_add_target(_SELF(self), ATK_OBJECT(RVAL2GOBJ(obj)));
    return self;
}
#endif

void
Init_atk_relation()
{
    VALUE rel = G_DEF_CLASS(ATK_TYPE_RELATION, "Relation", mAtk);
    VALUE type;
    rb_define_singleton_method(rel, "type_register", rbatkrel_s_type_register, 1);
    rb_define_method(rel, "initialize", rbatkrel_initialize, 2);
#if ATK_CHECK_VERSION(1,9,0)
    rb_define_method(rel, "add_target", rbatkrel_add_target, 1);
#endif

    /* AtkRelationType */
    type = G_DEF_CLASS(ATK_TYPE_RELATION_TYPE, "Type", rel);
    rb_define_singleton_method(type, "for_name", rbatkrelation_s_for_name, 1);
    G_DEF_CONSTANTS(rel, ATK_TYPE_RELATION_TYPE, "ATK_");
}
