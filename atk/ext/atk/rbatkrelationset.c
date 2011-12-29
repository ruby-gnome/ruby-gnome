/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE cRelationSet
#define _SELF(s) (RVAL2ATKRELATIONSET(s))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, atk_relation_set_new());
    return Qnil;
}

static VALUE
rg_contains_p(VALUE self, VALUE relationship)
{
    return CBOOL2RVAL(atk_relation_set_contains(
                          _SELF(self),
                          RVAL2ATKRELATIONTYPE(relationship)));
}

static VALUE
rg_remove(VALUE self, VALUE relation)
{
    atk_relation_set_remove(_SELF(self), RVAL2ATKRELATION(relation));
    return self;
}

static VALUE
rg_add(VALUE self, VALUE relation)
{
    atk_relation_set_add(_SELF(self), RVAL2ATKRELATION(relation));
    return self;
}

static VALUE
rg_n_relations(VALUE self)
{
    return INT2NUM(atk_relation_set_get_n_relations(_SELF(self)));
}

static VALUE
rg_get_relation(VALUE self, VALUE i)
{
    if (rb_obj_is_kind_of(i, GTYPE2CLASS(ATK_TYPE_RELATION_TYPE))){
        return GOBJ2RVAL(atk_relation_set_get_relation_by_type(
                             _SELF(self),
                             RVAL2ATKRELATIONTYPE(i)));
    } else {
        return GOBJ2RVAL(atk_relation_set_get_relation(_SELF(self), NUM2INT(i)));
    }
}

#if ATK_CHECK_VERSION(1,9,0)
static VALUE
rg_add_relation(VALUE self, VALUE relationship, VALUE obj)
{
    atk_relation_set_add_relation_by_type(_SELF(self), 
                                          RVAL2ATKRELATIONTYPE(relationship), 
                                          RVAL2ATKOBJECT(obj));
    return self;
}
#endif

void
Init_atk_relation_set(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_RELATION_SET, "RelationSet", mAtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD_P(contains, 1);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(add, 1);
    RG_DEF_METHOD(n_relations, 0);
    RG_DEF_METHOD(get_relation, 1);
#if ATK_CHECK_VERSION(1,9,0)
    RG_DEF_METHOD(add_relation, 2);
#endif
}
