/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkrelation.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"
#include "rbatkprivate.h"

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

struct rval2atkobjects_args {
    VALUE ary;
    long n;
    AtkObject **result;
};

static VALUE
rval2atkobjects_body(VALUE value)
{
    long i;
    struct rval2atkobjects_args *args = (struct rval2atkobjects_args *)value;

    for (i = 0; i < args->n; i++)
            args->result[i] = ATK_OBJECT(RVAL2GOBJ(RARRAY_PTR(args->ary)[i]));

    return Qnil;
}

static VALUE
rval2atkobjects_rescue(VALUE value)
{
    g_free(((struct rval2atkobjects_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static VALUE
rbatkrel_initialize(VALUE self, VALUE targets, VALUE rbrelationship)
{
    AtkRelationType relationship = RVAL2GENUM(rbrelationship, ATK_TYPE_RELATION_TYPE);
    struct rval2atkobjects_args args;
    AtkRelation *relation;

    args.ary = rb_ary_to_ary(targets);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(AtkObject *, args.n + 1);

    rb_rescue(rval2atkobjects_body, (VALUE)&args,
              rval2atkobjects_rescue, (VALUE)&args);

    relation = atk_relation_new(args.result, args.n, relationship);

    g_free(args.result);

    G_INITIALIZE(self, relation);

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
