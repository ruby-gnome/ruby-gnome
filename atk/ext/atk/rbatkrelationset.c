/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkrelationset.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_RELATION_SET(RVAL2GOBJ(s)))

static VALUE
rbatkrelset_initialize(VALUE self)
{
    G_INITIALIZE(self, atk_relation_set_new());
    return Qnil;
}

static VALUE
rbatkrelset_contains(VALUE self, VALUE relationship)
{
    return CBOOL2RVAL(atk_relation_set_contains(
                          _SELF(self),
                          RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE)));
}

static VALUE
rbatkrelset_remove(VALUE self, VALUE relation)
{
    atk_relation_set_remove(_SELF(self), ATK_RELATION(RVAL2GOBJ(relation)));
    return self;
}

static VALUE
rbatkrelset_add(VALUE self, VALUE relation)
{
    atk_relation_set_add(_SELF(self), ATK_RELATION(RVAL2GOBJ(relation)));
    return self;
}

static VALUE
rbatkrelset_get_n_relations(VALUE self)
{
    return INT2NUM(atk_relation_set_get_n_relations(_SELF(self)));
}

static VALUE
rbatkrelset_get_relation(VALUE self, VALUE i)
{
    if (rb_obj_is_kind_of(i, GTYPE2CLASS(ATK_TYPE_RELATION_TYPE))){
        return GOBJ2RVAL(atk_relation_set_get_relation_by_type(
                             _SELF(self),
                             RVAL2GENUM(i, ATK_TYPE_RELATION_TYPE)));
    } else {
        return GOBJ2RVAL(atk_relation_set_get_relation(_SELF(self), NUM2INT(i)));
    }
}

#if ATK_CHECK_VERSION(1,9,0)
static VALUE
rbatkrelset_add_relation(VALUE self, VALUE relationship, VALUE obj)
{
    atk_relation_set_add_relation_by_type(_SELF(self), 
                                          RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE), 
                                          ATK_OBJECT(RVAL2GOBJ(obj)));
    return self;
}
#endif

void
Init_atk_relation_set()
{
    VALUE rel = G_DEF_CLASS(ATK_TYPE_RELATION_SET, "RelationSet", mAtk);

    rb_define_method(rel, "initialize", rbatkrelset_initialize, 0);
    rb_define_method(rel, "contains?", rbatkrelset_contains, 1);
    rb_define_method(rel, "remove", rbatkrelset_remove, 1);
    rb_define_method(rel, "add", rbatkrelset_add, 1);
    rb_define_method(rel, "n_relations", rbatkrelset_get_n_relations, 0);
    rb_define_method(rel, "get_relation", rbatkrelset_get_relation, 1);
#if ATK_CHECK_VERSION(1,9,0)
    rb_define_method(rel, "add_relation", rbatkrelset_add_relation, 2);
#endif
}
