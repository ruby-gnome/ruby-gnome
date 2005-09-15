/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkobject.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_OBJECT(RVAL2GOBJ(s)))

/* How can I implement this? Since 1.10
AtkObject*  atk_implementor_ref_accessible  (AtkImplementor *implementor);
*/

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
/* Use properties accessible_* 
void        atk_object_set_name             (AtkObject *accessible,
                                             const gchar *name);
void        atk_object_set_description      (AtkObject *accessible,
                                             const gchar *description);
void        atk_object_set_parent           (AtkObject *accessible,
                                             AtkObject *parent);
void        atk_object_set_role             (AtkObject *accessible,
                                             AtkRole role);
*/

/*    
guint       atk_object_connect_property_change_handler
                                            (AtkObject *accessible,
                                             AtkPropertyChangeHandler *handler);
void        atk_object_remove_property_change_handler
                                            (AtkObject *accessible,
                                             guint handler_id);
*/

static VALUE
rbatkobj_notify_state_change(self, state, value)
    VALUE self, state, value;
{
    atk_object_notify_state_change(_SELF(self), 
                                   RVAL2GENUM(state, ATK_TYPE_STATE_TYPE),
                                   RTEST(value));
    return self;
}

/* I don't have a good idea to implement this method. Any idea?
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

/* We don't need this.
G_CONST_RETURN gchar* atk_role_get_name     (AtkRole role);
*/

static VALUE
rbatkrole_get_localized_name(self)
    VALUE self;
{
#ifdef HAVE_ATK_ROLE_GET_LOCALIZED_NAME
    return CSTR2RVAL(atk_role_get_localized_name(RVAL2GENUM(self, ATK_TYPE_ROLE)));
#else
    rb_warning("not supported in this version of ATK.");
    return Qnil;
#endif
}

static VALUE
rbatkrole_s_for_name(self, name)
    VALUE self, name;
{
    return GENUM2RVAL(atk_role_for_name(RVAL2CSTR(name)), ATK_TYPE_ROLE);
}

void
Init_atk_object()
{
    VALUE obj = G_DEF_CLASS(ATK_TYPE_OBJECT, "Object", mAtk);
    VALUE role;

    rb_define_method(obj, "n_accessible_children", rbatkobj_get_n_accessible_children, 0);
    rb_define_method(obj, "ref_accessible_child", rbatkobj_ref_accessible_child, 1);
    rb_define_method(obj, "ref_relation_set", rbatkobj_ref_releation_set, 0);
    rb_define_method(obj, "ref_state_set", rbatkobj_ref_state_set, 0);
    rb_define_method(obj, "index_in_parent", rbatkobj_get_index_in_parent, 0);
    rb_define_method(obj, "notify_state_change", rbatkobj_notify_state_change, 2);
#ifdef HAVE_ATK_OBJECT_ADD_RELATIONSHIP
    rb_define_method(obj, "add_relationship", rbatkobj_add_relationship, 2);
#endif
#ifdef HAVE_ATK_OBJECT_REMOVE_RELATIONSHIP
    rb_define_method(obj, "remove_relationship", rbatkobj_remove_relationship, 2);
#endif

    /* AtkRole */
    role = G_DEF_CLASS(ATK_TYPE_ROLE, "Role", obj);
    rb_define_method(role, "localized_name", rbatkrole_get_localized_name, 0);
    rb_define_singleton_method(role, "for_name", rbatkrole_s_for_name, 1);
    G_DEF_CONSTANTS(obj, ATK_TYPE_ROLE, "ATK_");

    /* AtkLayer */
    G_DEF_CLASS(ATK_TYPE_LAYER, "Layer", obj);
    G_DEF_CONSTANTS(obj, ATK_TYPE_LAYER, "ATK_");
}
