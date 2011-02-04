/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkcomponent.c -

  $Author: sakai $
  $Date: 2007/07/08 02:51:52 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_COMPONENT(RVAL2GOBJ(s)))

/*
static void
focus_handler(AtkObject *aobj, gboolean bool)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2,
                           GOBJ2RVAL(aobj), CBOOL2RVAL(bool));
}
static VALUE
comp_add_focus_handler(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return UINT2NUM(atk_component_add_focus_handler(_SELF(self), focus_handler));
}
*/

static VALUE
comp_contains(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    return CBOOL2RVAL(atk_component_contains(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE)));
}

static VALUE
comp_get_extents(VALUE self, VALUE coord_type)
{
    gint x, y, width, height; 
    atk_component_get_extents(_SELF(self), &x, &y, &width, &height,
                              RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

#ifdef HAVE_ATK_COMPONENT_GET_LAYER
static VALUE
comp_get_layer(VALUE self)
{
    return GENUM2RVAL(atk_component_get_layer(_SELF(self)), ATK_TYPE_LAYER);
}
#endif

#ifdef HAVE_ATK_COMPONENT_GET_MDI_ZORDER
static VALUE
comp_get_mdi_zorder(VALUE self)
{
    return INT2NUM(atk_component_get_mdi_zorder(_SELF(self)));
}

#endif
static VALUE
comp_get_position(VALUE self, VALUE coord_type)
{
    gint x, y;
    atk_component_get_position(_SELF(self), &x, &y,
                               RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
comp_get_size(VALUE self)
{
    gint width, height;
    atk_component_get_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

static VALUE
comp_grab_focus(VALUE self)
{
    return CBOOL2RVAL(atk_component_grab_focus(_SELF(self)));
}

static VALUE
comp_ref_accessible_at_point(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    return GOBJ2RVAL(atk_component_ref_accessible_at_point(
                         _SELF(self),
                         NUM2INT(x), NUM2INT(y),
                         RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE)));
}

static VALUE
comp_remove_focus_handler(VALUE self, VALUE handler_id)
{
    atk_component_remove_focus_handler(_SELF(self), NUM2UINT(handler_id));
    return self;
}

static VALUE
comp_set_extents(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE coord_type)
{
    gboolean ret = atk_component_set_extents(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             NUM2INT(width), NUM2INT(height),
                                             RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set extents");
    return self;
}

static VALUE
comp_set_position(VALUE self, VALUE x, VALUE y, VALUE coord_type)
{
    gboolean ret = atk_component_set_position(_SELF(self),
                                              NUM2INT(x), NUM2INT(y),
                                              RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the position");
    return self;
}

static VALUE
comp_set_size(VALUE self, VALUE width, VALUE height)
{
    gboolean ret = atk_component_set_size(_SELF(self),
                                          NUM2INT(width), NUM2INT(height));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the size");
    return self;
}

#if ATK_CHECK_VERSION(1,12,0)
static VALUE
comp_get_alpha(VALUE self)
{
    return rb_float_new(atk_component_get_alpha(_SELF(self)));
}
#endif

void
Init_atk_component()
{
    VALUE comp = G_DEF_INTERFACE(ATK_TYPE_COMPONENT, "Component", mAtk);
/*
    rb_define_method(comp, "add_focus_handler", comp_add_focus_handler, 0);
*/
    rb_define_method(comp, "contains?", comp_contains, 3);
    rb_define_method(comp, "get_extents", comp_get_extents, 1);
#ifdef HAVE_ATK_COMPONENT_GET_LAYER
    rb_define_method(comp, "layer", comp_get_layer, 0);
#endif
#ifdef HAVE_ATK_COMPONENT_GET_MDI_ZORDER
    rb_define_method(comp, "mdi_zorder", comp_get_mdi_zorder, 0);
#endif
    rb_define_method(comp, "position", comp_get_position, 1);
    rb_define_method(comp, "size", comp_get_size, 0);
    rb_define_method(comp, "grab_focus", comp_grab_focus, 0);
    rb_define_method(comp, "ref_accessible_at_point", comp_ref_accessible_at_point, 3);
    rb_define_method(comp, "remove_focus_handler", comp_remove_focus_handler, 1);
    rb_define_method(comp, "set_extents", comp_set_extents, 5);
    rb_define_method(comp, "set_position", comp_set_position, 2);
    rb_define_method(comp, "set_size", comp_set_size, 2);
#if ATK_CHECK_VERSION(1,12,0)
    rb_define_method(comp, "alpha", comp_get_alpha, 0);
#endif
}
