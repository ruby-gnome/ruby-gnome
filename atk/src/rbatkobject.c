/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkobject.c -

  $Author: mutoh $
  $Date: 2004/03/05 15:33:48 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_OBJECT(RVAL2GOBJ(s)))

/* Use properties accessible_* 
G_CONST_RETURN gchar* atk_object_get_name   (AtkObject *accessible);
G_CONST_RETURN gchar* atk_object_get_description
                                            (AtkObject *accessible);
AtkObject*  atk_object_get_parent           (AtkObject *accessible);
                                            (AtkObject *accessible);
*/

static VALUE
rbatkobj_get_n_accessible_children(self)
    VALUE self;
{
    return INT2NUM(atk_object_get_n_accessible_children(_SELF(self)));
}

static VALUE
rbatkobj_ref_accessible_child(self, i)
    VALUE self, i;
{
    return GOBJ2RVAL(atk_object_ref_accessible_child(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatkobj_ref_releation_set(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_object_ref_relation_set(_SELF(self)));
}

/* Use properties accessible_* 
AtkLayer    atk_object_get_layer            (AtkObject *accessible);
gint        atk_object_get_mdi_zorder       (AtkObject *accessible);
AtkRole     atk_object_get_role             (AtkObject *accessible);
*/

static VALUE
rbatkobj_ref_state_set(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_object_ref_state_set(_SELF(self)));
}

static VALUE
rbatkobj_get_index_in_parent(self)
    VALUE self;
{
    return INT2NUM(atk_object_get_index_in_parent(_SELF(self)));
}

/*    
guint       atk_object_connect_property_change_handler
                                            (AtkObject *accessible,
                                             AtkPropertyChangeHandler *handler);
void        atk_object_remove_property_change_handler
                                            (AtkObject *accessible,
                                             guint handler_id);
void        atk_object_notify_state_change(_SELF(self), 
                                             AtkState state,
                                             gboolean value);

void        atk_object_initialize           (AtkObject *accessible,
                                             gpointer data);
*/

#ifdef HAVE_ATK_OBJECT_ADD_RELATIONSHIP
static VALUE
rbatkobj_add_relationship(self, relationship, target)
    VALUE self, relationship, target;
{
    return CBOOL2RVAL(atk_object_add_relationship(
                          _SELF(self), 
                          RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE),
                          _SELF(target)));
}
#endif

#ifdef HAVE_ATK_OBJECT_REMOVE_RELATIONSHIP
static VALUE
rbatkobj_remove_relationship(self, relationship, target)
    VALUE self, relationship, target;
{
    return CBOOL2RVAL(atk_object_remove_relationship(
                          _SELF(self), 
                          RVAL2GENUM(relationship, ATK_TYPE_RELATION_TYPE),
                          _SELF(target)));
}
#endif

/* We don't need them.
G_CONST_RETURN gchar* atk_role_get_name     (AtkRole role);
G_CONST_RETURN gchar* atk_role_get_localized_name
                                            (AtkRole role);
AtkRole     atk_role_for_name               (const gchar *name);
*/

void
Init_atk_object()
{
    VALUE obj = G_DEF_CLASS(ATK_TYPE_OBJECT, "Object", mAtk);

    rb_define_method(obj, "n_accessible_children", rbatkobj_get_n_accessible_children, 0);
    rb_define_method(obj, "ref_accessible_child", rbatkobj_ref_accessible_child, 1);
    rb_define_method(obj, "ref_relation_set", rbatkobj_ref_releation_set, 0);
    rb_define_method(obj, "ref_state_set", rbatkobj_ref_state_set, 0);
    rb_define_method(obj, "index_in_parent", rbatkobj_get_index_in_parent, 0);
#ifdef HAVE_ATK_OBJECT_ADD_RELATIONSHIP
    rb_define_method(obj, "add_relationship", rbatkobj_add_relationship, 2);
#endif
#ifdef HAVE_ATK_OBJECT_REMOVE_RELATIONSHIP
    rb_define_method(obj, "remove_relationship", rbatkobj_remove_relationship, 2);
#endif

    /* AtkRole */
    G_DEF_CLASS(ATK_TYPE_ROLE, "Role", obj);
    G_DEF_CONSTANTS(obj, ATK_TYPE_ROLE, "ATK_");

    /* AtkLayer */
    G_DEF_CLASS(ATK_TYPE_LAYER, "Layer", obj);
    G_DEF_CONSTANTS(obj, ATK_TYPE_LAYER, "ATK_");
}
