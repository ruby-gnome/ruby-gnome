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

#define RG_TARGET_NAMESPACE cStateSet
#define _SELF(s) (ATK_STATE_SET(RVAL2GOBJ(s)))

static VALUE
rbatkstateset_initialize(VALUE self)
{
    G_INITIALIZE(self, atk_state_set_new());
    return Qnil;
}

static VALUE
rbatkstateset_is_empty(VALUE self)
{
    return CBOOL2RVAL(atk_state_set_is_empty(_SELF(self)));
}

static VALUE
rbatkstateset_add_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_add_state(_SELF(self), 
                                              RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

struct rval2atkstatetype_args {
    VALUE ary;
    long n;
    AtkStateType *result;
};

static VALUE
rval2atkstatetype_body(VALUE value)
{
    long i;
    struct rval2atkstatetype_args *args = (struct rval2atkstatetype_args *)value;

    for (i = 0; i < args->n; i++)
            args->result[i] = RVAL2GENUM(RARRAY_PTR(args->ary)[i], ATK_TYPE_STATE_TYPE);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rval2atkstatetype_rescue(VALUE value)
{
    g_free(((struct rval2atkstatetype_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static AtkStateType *
rval2atkstatetype(VALUE value, long *n)
{
    struct rval2atkstatetype_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(AtkStateType, args.n + 1);

    rb_rescue(rval2atkstatetype_body, (VALUE)&args,
              rval2atkstatetype_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2ATKSTATETYPES(value, n) rval2atkstatetype(value, n)

static VALUE
rbatkstateset_add_states(VALUE self, VALUE rbtypes)
{
    AtkStateSet *set = _SELF(self);
    long n;
    AtkStateType *types = RVAL2ATKSTATETYPES(rbtypes, &n);

    atk_state_set_add_states(set, types, n);

    g_free(types);

    return self;
}

static VALUE
rbatkstateset_clear_states(VALUE self)
{
    atk_state_set_clear_states(_SELF(self));
    return self;
}

static VALUE
rbatkstateset_contains_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_contains_state(_SELF(self), 
                                                   RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

static VALUE
rbatkstateset_contains_states(VALUE self, VALUE rbtypes)
{
    AtkStateSet *set = _SELF(self);
    long n;
    AtkStateType *types = RVAL2ATKSTATETYPES(rbtypes, &n);
    gboolean result;

    result = atk_state_set_contains_states(set, types, n);

    g_free(types);

    return CBOOL2RVAL(result);
}

static VALUE
rbatkstateset_remove_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_remove_state(_SELF(self), 
                                                 RVAL2GENUM(type, ATK_TYPE_STATE_TYPE)));
}

static VALUE
rbatkstateset_and_sets(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_and_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rbatkstateset_or_sets(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_or_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rbatkstateset_xor_sets(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_xor_sets(_SELF(self), _SELF(compare_set)));
}

void
Init_atk_state_set(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_STATE_SET, "StateSet", mAtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", rbatkstateset_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "empty?", rbatkstateset_is_empty, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "add_state", rbatkstateset_add_state, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add_states", rbatkstateset_add_states, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "clear_states", rbatkstateset_clear_states, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "contains_state", rbatkstateset_contains_state, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "contains_states", rbatkstateset_contains_states, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_state", rbatkstateset_remove_state, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "and", rbatkstateset_and_sets, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "&", "and");
    rb_define_method(RG_TARGET_NAMESPACE, "or", rbatkstateset_or_sets, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "|", "or");
    rb_define_method(RG_TARGET_NAMESPACE, "xor", rbatkstateset_xor_sets, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "^", "xor");
}
