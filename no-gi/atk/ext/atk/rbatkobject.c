/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cObject
#define _SELF(s) RVAL2ATKOBJECT(s)

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
rg_n_accessible_children(VALUE self)
{
    return INT2NUM(atk_object_get_n_accessible_children(_SELF(self)));
}

static VALUE
rg_ref_accessible_child(VALUE self, VALUE i)
{
    return GOBJ2RVAL(atk_object_ref_accessible_child(_SELF(self), NUM2INT(i)));
}

static VALUE
rg_ref_relation_set(VALUE self)
{
    return GOBJ2RVAL(atk_object_ref_relation_set(_SELF(self)));
}

/* Use properties accessible_* 
AtkLayer    atk_object_get_layer            (AtkObject *accessible);
gint        atk_object_get_mdi_zorder       (AtkObject *accessible);
AtkRole     atk_object_get_role             (AtkObject *accessible);
*/

static VALUE
rg_ref_state_set(VALUE self)
{
    return GOBJ2RVAL(atk_object_ref_state_set(_SELF(self)));
}

static VALUE
rg_index_in_parent(VALUE self)
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
rg_notify_state_change(VALUE self, VALUE state, VALUE value)
{
    atk_object_notify_state_change(_SELF(self), 
                                   RVAL2ATKSTATETYPE(state),
                                   RVAL2CBOOL(value));
    return self;
}

/* I don't have a good idea to implement this method. Any idea?
void        atk_object_initialize           (AtkObject *accessible,
                                             gpointer data);
*/

#ifdef HAVE_ATK_OBJECT_ADD_RELATIONSHIP
static VALUE
rg_add_relationship(VALUE self, VALUE relationship, VALUE target)
{
    return CBOOL2RVAL(atk_object_add_relationship(
                          _SELF(self), 
                          RVAL2ATKRELATIONTYPE(relationship),
                          _SELF(target)));
}
#endif

#ifdef HAVE_ATK_OBJECT_REMOVE_RELATIONSHIP
static VALUE
rg_remove_relationship(VALUE self, VALUE relationship, VALUE target)
{
    return CBOOL2RVAL(atk_object_remove_relationship(
                          _SELF(self), 
                          RVAL2ATKRELATIONTYPE(relationship),
                          _SELF(target)));
}
#endif

void
Init_atk_object(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_OBJECT, "Object", mAtk);

    RG_DEF_METHOD(n_accessible_children, 0);
    RG_DEF_METHOD(ref_accessible_child, 1);
    RG_DEF_METHOD(ref_relation_set, 0);
    RG_DEF_METHOD(ref_state_set, 0);
    RG_DEF_METHOD(index_in_parent, 0);
    RG_DEF_METHOD(notify_state_change, 2);
#ifdef HAVE_ATK_OBJECT_ADD_RELATIONSHIP
    RG_DEF_METHOD(add_relationship, 2);
#endif
#ifdef HAVE_ATK_OBJECT_REMOVE_RELATIONSHIP
    RG_DEF_METHOD(remove_relationship, 2);
#endif

    Init_atk_object_role(RG_TARGET_NAMESPACE);

    /* AtkLayer */
    G_DEF_CLASS(ATK_TYPE_LAYER, "Layer", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, ATK_TYPE_LAYER, "ATK_");
}
