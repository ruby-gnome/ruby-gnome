/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkcomponent.c -

  $Author: mutoh $
  $Date: 2004/03/05 15:33:47 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_COMPONENT(RVAL2GOBJ(s)))

/*
static void
focus_handler(aobj, bool)
    AtkObject* aobj;
    gboolean bool;
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2,
                           GOBJ2RVAL(aobj), CBOOL2RVAL(bool));
}
static VALUE
comp_add_focus_handler(self)
     VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    return UINT2NUM(atk_component_add_focus_handler(_SELF(self), focus_handler));
}
*/

static VALUE
comp_contains(self, x, y, coord_type)
    VALUE self, x, y, coord_type;
{
    return CBOOL2RVAL(atk_component_contains(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE)));
}

static VALUE
comp_get_extents(self, coord_type)
    VALUE self, coord_type;
{
    gint x, y, width, height; 
    atk_component_get_extents(_SELF(self), &x, &y, &width, &height,
                              RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

static VALUE
comp_get_layer(self)
    VALUE self;
{
    return GENUM2RVAL(atk_component_get_layer(_SELF(self)), ATK_TYPE_LAYER);
}

static VALUE
comp_get_mdi_zorder(self)
    VALUE self;
{
    return INT2NUM(atk_component_get_mdi_zorder(_SELF(self)));
}

static VALUE
comp_get_position(self, coord_type)
    VALUE self, coord_type;
{
    gint x, y;
    atk_component_get_position(_SELF(self), &x, &y,
                               RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
comp_get_size(self)
    VALUE self;
{
    gint width, height;
    atk_component_get_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

static VALUE
comp_grab_focus(self)
    VALUE self;
{
    return CBOOL2RVAL(atk_component_grab_focus(_SELF(self)));
}

static VALUE
comp_ref_accessible_at_point(self, x, y, coord_type)
    VALUE self, x, y, coord_type;
{
    return GOBJ2RVAL(atk_component_ref_accessible_at_point(
                         _SELF(self),
                         NUM2INT(x), NUM2INT(y),
                         RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE)));
}

static VALUE
comp_remove_focus_handler(self, handler_id)
    VALUE self, handler_id;
{
    atk_component_remove_focus_handler(_SELF(self), NUM2UINT(handler_id));
    return self;
}

static VALUE
comp_set_extents(self, x, y, width, height, coord_type)
    VALUE self, x, y, width, height, coord_type;
{
    gboolean ret = atk_component_set_extents(_SELF(self),
                                             NUM2INT(x), NUM2INT(y),
                                             NUM2INT(width), NUM2INT(height),
                                             RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set extents");
    return self;
}

static VALUE
comp_set_position(self, x, y, coord_type)
    VALUE self, x, y, coord_type;
{
    gboolean ret = atk_component_set_position(_SELF(self),
                                              NUM2INT(x), NUM2INT(y),
                                              RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the position");
    return self;
}

static VALUE
comp_set_size(self, width, height)
    VALUE self, width, height;
{
    gboolean ret = atk_component_set_size(_SELF(self),
                                          NUM2INT(width), NUM2INT(height));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set the size");
    return self;
}

void
Init_atk_component()
{
    VALUE comp = G_DEF_INTERFACE(ATK_TYPE_COMPONENT, "Component", mAtk);
/*
    rb_define_method(comp, "add_focus_handler", comp_add_focus_handler, 0);
*/
    rb_define_method(comp, "contains?", comp_contains, 3);
    rb_define_method(comp, "get_extents", comp_get_extents, 1);
    rb_define_method(comp, "layer", comp_get_layer, 0);
    rb_define_method(comp, "mdi_zorder", comp_get_mdi_zorder, 0);
    rb_define_method(comp, "position", comp_get_position, 1);
    rb_define_method(comp, "size", comp_get_size, 0);
    rb_define_method(comp, "grab_focus", comp_grab_focus, 0);
    rb_define_method(comp, "ref_accessible_at_point", comp_ref_accessible_at_point, 3);
    rb_define_method(comp, "remove_focus_handler", comp_remove_focus_handler, 1);
    rb_define_method(comp, "set_extents", comp_set_extents, 5);
    rb_define_method(comp, "set_position", comp_set_position, 2);
    rb_define_method(comp, "set_size", comp_set_size, 2);

}
