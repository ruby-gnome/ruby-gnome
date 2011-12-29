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
#define _SELF(s) (RVAL2ATKSTATESET(s))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, atk_state_set_new());
    return Qnil;
}

static VALUE
rg_empty_p(VALUE self)
{
    return CBOOL2RVAL(atk_state_set_is_empty(_SELF(self)));
}

static VALUE
rg_add_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_add_state(_SELF(self), 
                                              RVAL2ATKSTATETYPE(type)));
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
            args->result[i] = RVAL2ATKSTATETYPE(RARRAY_PTR(args->ary)[i]);

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
rg_add_states(VALUE self, VALUE rbtypes)
{
    AtkStateSet *set = _SELF(self);
    long n;
    AtkStateType *types = RVAL2ATKSTATETYPES(rbtypes, &n);

    atk_state_set_add_states(set, types, n);

    g_free(types);

    return self;
}

static VALUE
rg_clear_states(VALUE self)
{
    atk_state_set_clear_states(_SELF(self));
    return self;
}

static VALUE
rg_contains_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_contains_state(_SELF(self), 
                                                   RVAL2ATKSTATETYPE(type)));
}

static VALUE
rg_contains_states(VALUE self, VALUE rbtypes)
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
rg_remove_state(VALUE self, VALUE type)
{
    return CBOOL2RVAL(atk_state_set_remove_state(_SELF(self), 
                                                 RVAL2ATKSTATETYPE(type)));
}

static VALUE
rg_and(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_and_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rg_or(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_or_sets(_SELF(self), _SELF(compare_set)));
}

static VALUE
rg_xor(VALUE self, VALUE compare_set)
{
    return GOBJ2RVAL(atk_state_set_xor_sets(_SELF(self), _SELF(compare_set)));
}

void
Init_atk_state_set(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_STATE_SET, "StateSet", mAtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD_P(empty, 0);
    RG_DEF_METHOD(add_state, 1);
    RG_DEF_METHOD(add_states, 1);
    RG_DEF_METHOD(clear_states, 0);
    RG_DEF_METHOD(contains_state, 1);
    RG_DEF_METHOD(contains_states, 1);
    RG_DEF_METHOD(remove_state, 1);
    RG_DEF_METHOD(and, 1);
    RG_DEF_ALIAS("&", "and");
    RG_DEF_METHOD(or, 1);
    RG_DEF_ALIAS("|", "or");
    RG_DEF_METHOD(xor, 1);
    RG_DEF_ALIAS("^", "xor");
}
