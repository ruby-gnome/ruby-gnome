/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkrelationset.c -

  $Author: mutoh $
  $Date: 2003/12/08 16:32:09 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_RELATION_SET(RVAL2GOBJ(s)))

static VALUE
rbatkrelset_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, atk_relation_set_new());
    return Qnil;
}

static VALUE
rbatkrelset_contains(self, relationship)
    VALUE self, relationship;
{
    return CBOOL2RVAL(atk_relation_set_contains(
                          _SELF(self),
                          RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE)));
}

static VALUE
rbatkrelset_remove(self, relation)
    VALUE self, relation;
{
    atk_relation_set_remove(_SELF(self), ATK_RELATION(RVAL2GOBJ(relation)));
    return self;
}

static VALUE
rbatkrelset_add(self, relation)
    VALUE self, relation;
{
    atk_relation_set_add(_SELF(self), ATK_RELATION(RVAL2GOBJ(relation)));
    return self;
}

static VALUE
rbatkrelset_get_n_relations(self)
    VALUE self;
{
    return INT2NUM(atk_relation_set_get_n_relations(_SELF(self)));
}

static VALUE
rbatkrelset_get_relation(self, i)
    VALUE self, i;
{
    if (rb_obj_is_kind_of(i, GTYPE2CLASS(ATK_TYPE_RELATION_TYPE))){
        return GOBJ2RVAL(atk_relation_set_get_relation_by_type(
                             _SELF(self),
                             RVAL2GENUM(i, ATK_TYPE_RELATION_TYPE)));
    } else {
        return GOBJ2RVAL(atk_relation_set_get_relation(_SELF(self), NUM2INT(i)));
    }
}

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

}
