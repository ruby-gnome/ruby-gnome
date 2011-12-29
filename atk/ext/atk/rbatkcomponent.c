/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#define RG_TARGET_NAMESPACE mComponent
#define _SELF(s) (RVAL2ATKCOMPONENT(s))

/*
static void
focus_handler(AtkObject *aobj, gboolean bool)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2,
                           GOBJ2RVAL(aobj), CBOOL2RVAL(bool));
}
static VALUE
rg_add_focus_handler(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return UINT2NUM(atk_component_add_focus_handler(_SELF(self), focus_handler));
}
*/

static VALUE
rg_contains_p(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    return CBOOL2RVAL(atk_component_contains(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             RVAL2ATKCOORDTYPE(coord_type)));
}

static VALUE
rg_get_extents(VALUE self, VALUE coord_type)
{
    gint x, y, width, height; 
    atk_component_get_extents(_SELF(self), &x, &y, &width, &height,
                              RVAL2ATKCOORDTYPE(coord_type));
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

#ifdef HAVE_ATK_COMPONENT_GET_LAYER
static VALUE
rg_layer(VALUE self)
{
    return ATKLAYER2RVAL(atk_component_get_layer(_SELF(self)));
}
#endif

#ifdef HAVE_ATK_COMPONENT_GET_MDI_ZORDER
static VALUE
rg_mdi_zorder(VALUE self)
{
    return INT2NUM(atk_component_get_mdi_zorder(_SELF(self)));
}

#endif
static VALUE
rg_position(VALUE self, VALUE coord_type)
{
    gint x, y;
    atk_component_get_position(_SELF(self), &x, &y,
                               RVAL2ATKCOORDTYPE(coord_type));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_size(VALUE self)
{
    gint width, height;
    atk_component_get_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_grab_focus(VALUE self)
{
    return CBOOL2RVAL(atk_component_grab_focus(_SELF(self)));
}

static VALUE
rg_ref_accessible_at_point(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    return GOBJ2RVAL(atk_component_ref_accessible_at_point(
                         _SELF(self),
                         NUM2INT(x), NUM2INT(y),
                         RVAL2ATKCOORDTYPE(coord_type)));
}

static VALUE
rg_remove_focus_handler(VALUE self, VALUE handler_id)
{
    atk_component_remove_focus_handler(_SELF(self), NUM2UINT(handler_id));
    return self;
}

static VALUE
rg_set_extents(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE coord_type)
{
    gboolean ret = atk_component_set_extents(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             NUM2INT(width), NUM2INT(height),
                                             RVAL2ATKCOORDTYPE(coord_type));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set extents");
    return self;
}

static VALUE
rg_set_position(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    gboolean ret = atk_component_set_position(_SELF(self),
                                              NUM2INT(x), NUM2INT(y),
                                              RVAL2ATKCOORDTYPE(coord_type));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the position");
    return self;
}

static VALUE
rg_set_size(VALUE self, VALUE width, VALUE height)
{
    gboolean ret = atk_component_set_size(_SELF(self),
                                          NUM2INT(width), NUM2INT(height));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the size");
    return self;
}

#if ATK_CHECK_VERSION(1,12,0)
static VALUE
rg_alpha(VALUE self)
{
    return rb_float_new(atk_component_get_alpha(_SELF(self)));
}
#endif

void
Init_atk_component(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_COMPONENT, "Component", mAtk);
/*
    RG_DEF_METHOD(add_focus_handler, 0);
*/
    RG_DEF_METHOD_P(contains, 3);
    RG_DEF_METHOD(get_extents, 1);
#ifdef HAVE_ATK_COMPONENT_GET_LAYER
    RG_DEF_METHOD(layer, 0);
#endif
#ifdef HAVE_ATK_COMPONENT_GET_MDI_ZORDER
    RG_DEF_METHOD(mdi_zorder, 0);
#endif
    RG_DEF_METHOD(position, 1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(grab_focus, 0);
    RG_DEF_METHOD(ref_accessible_at_point, 3);
    RG_DEF_METHOD(remove_focus_handler, 1);
    RG_DEF_METHOD(set_extents, 5);
    RG_DEF_METHOD(set_position, 2);
    RG_DEF_METHOD(set_size, 2);
#if ATK_CHECK_VERSION(1,12,0)
    RG_DEF_METHOD(alpha, 0);
#endif
}
