/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkrelation.c -

  $Author: mutoh $
  $Date: 2003/12/08 16:32:09 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_RELATION(RVAL2GOBJ(s)))

static VALUE
rbatkrel_s_type_register(self, name)
    VALUE self, name;
{
    return GENUM2RVAL(atk_relation_type_register(RVAL2CSTR(name)), ATK_TYPE_RELATION_TYPE);
}

/* We don't need them
G_CONST_RETURN gchar* atk_relation_type_get_name
                                            (AtkRelationType type);
AtkRelationType atk_relation_type_for_name  (const gchar *name);
*/

static VALUE
rbatkrel_initialize(self, targets, relationship)
    VALUE self, targets, relationship;
{
    gint i;
    gint len = RARRAY(targets)->len;
    AtkObject* objects = g_new(AtkObject, len);

    for (i = 0; i < len; i++) {
        objects = (AtkObject*)RARRAY(targets)->ptr[i];
        objects++;
    }

    G_INITIALIZE(self, atk_relation_new(&objects, len, 
                                        RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE)));
    g_free(objects);

    return Qnil;
}

static VALUE
rbatkrel_get_relation_type(self)
    VALUE self;
{
    return GENUM2RVAL(atk_relation_get_relation_type(_SELF(self)), ATK_TYPE_RELATION_TYPE);
}

static VALUE
rbatkrel_get_target(self)
    VALUE self;
{
    gint i;
    GPtrArray* garray = atk_relation_get_target(_SELF(self));
    VALUE ary = rb_ary_new();

    for (i = 0; i < garray->len; i++){
        rb_ary_push(ary, GOBJ2RVAL((AtkObject*)g_ptr_array_index(garray, i)));
    }
    g_ptr_array_free(garray, TRUE);
    return ary;
}

void
Init_atk_relation()
{
    VALUE rel = G_DEF_CLASS(ATK_TYPE_RELATION, "Relation", mAtk);
    rb_define_singleton_method(rel, "type_register", rbatkrel_s_type_register, 1);
    rb_define_method(rel, "initialize", rbatkrel_initialize, 2);
    rb_define_method(rel, "relation_type", rbatkrel_get_relation_type, 0);
    rb_define_method(rel, "target", rbatkrel_get_target, 0);

    /* AtkRelationType */
    G_DEF_CLASS(ATK_TYPE_RELATION_TYPE, "Type", rel);
    G_DEF_CONSTANTS(rel, ATK_TYPE_RELATION_TYPE, "ATK_");
}
