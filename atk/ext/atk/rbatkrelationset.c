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
Init_atk_relation_set(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_RELATION_SET, "RelationSet", mAtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", rbatkrelset_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "contains?", rbatkrelset_contains, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove", rbatkrelset_remove, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add", rbatkrelset_add, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "n_relations", rbatkrelset_get_n_relations, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "get_relation", rbatkrelset_get_relation, 1);
#if ATK_CHECK_VERSION(1,9,0)
    rb_define_method(RG_TARGET_NAMESPACE, "add_relation", rbatkrelset_add_relation, 2);
#endif
}
