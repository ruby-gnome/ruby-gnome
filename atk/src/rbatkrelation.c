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
rbatkrel_s_type_register(self, name)
    VALUE self, name;
{
    return GENUM2RVAL(atk_relation_type_register(RVAL2CSTR(name)), ATK_TYPE_RELATION_TYPE);
}

/* We don't need them
G_CONST_RETURN gchar* atk_relation_type_get_name
                                            (AtkRelationType type);
*/

static VALUE
rbatkrelation_s_for_name(self, name)
    VALUE self, name;
{
    return GENUM2RVAL(atk_relation_type_for_name(RVAL2CSTR(name)), ATK_TYPE_RELATION_TYPE);
}


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

#if ATK_CHECK_VERSION(1,9,0)
static VALUE
rbatkrel_add_target(self, obj)
    VALUE self, obj;
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
    rb_define_method(rel, "relation_type", rbatkrel_get_relation_type, 0);
    rb_define_method(rel, "target", rbatkrel_get_target, 0);
#if ATK_CHECK_VERSION(1,9,0)
    rb_define_method(rel, "add_target", rbatkrel_add_target, 1);
#endif

    /* AtkRelationType */
    type = G_DEF_CLASS(ATK_TYPE_RELATION_TYPE, "Type", rel);
    rb_define_singleton_method(type, "for_name", rbatkrelation_s_for_name, 1);
    G_DEF_CONSTANTS(rel, ATK_TYPE_RELATION_TYPE, "ATK_");
}
