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

#include "global.h"

/*****************************************/
static GdkWindowAttr*
attr_copy(const GdkWindowAttr* win)
{
    GdkWindowAttr* new_win;
    g_return_val_if_fail (win != NULL, NULL);
    new_win = g_new(GdkWindowAttr, 1);
    *new_win = *win;
    return new_win;
}

GType
gdk_windowattr_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GdkWindowAttr",
                    (GBoxedCopyFunc)attr_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/

#define RG_TARGET_NAMESPACE cWindowAttr
#define _SELF(w) ((GdkWindowAttr*)RVAL2BOXED(w, GDK_TYPE_WINDOW_ATTR))

static VALUE
rg_initialize(VALUE self, VALUE width, VALUE height, VALUE wclass,
        VALUE window_type)
{
    GdkWindowAttr w;
    w.width = NUM2INT(width);
    w.height = NUM2INT(height);
    w.wclass = RVAL2GENUM(wclass, GDK_TYPE_WINDOW_CLASS);
    w.window_type = RVAL2GENUM(window_type, GDK_TYPE_WINDOW_TYPE);
    G_INITIALIZE(self, &w);
    return Qnil;
}

#define ATTR_STR(name)\
static VALUE \
attr_get_ ## name (VALUE self)\
{\
    return CSTR2RVAL(_SELF(self)->name);\
}\
static VALUE \
attr_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = (gchar *)RVAL2CSTR(val);\
    return self;\
}

#define ATTR_INT(name)\
static VALUE \
attr_get_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
attr_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val);\
    return self;\
}

ATTR_STR(title);
ATTR_INT(event_mask);
ATTR_INT(x);
ATTR_INT(y);
ATTR_INT(width);
ATTR_INT(height);

static VALUE
rg_wclass(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->wclass, GDK_TYPE_WINDOW_CLASS);
}
static VALUE
rg_set_wclass(VALUE self, VALUE val)
{
    _SELF(self)->wclass = RVAL2GENUM(val, GDK_TYPE_WINDOW_CLASS);
    return self;
}

static VALUE
rg_visual(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->visual);
}
static VALUE
rg_set_visual(VALUE self, VALUE val)
{
    _SELF(self)->visual = GDK_VISUAL(RVAL2GOBJ(val));
    return self;
}

static VALUE
rg_colormap(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->colormap);
}
static VALUE
rg_set_colormap(VALUE self, VALUE val)
{
    _SELF(self)->colormap = GDK_COLORMAP(RVAL2GOBJ(val));
    return self;
}

static VALUE
rg_window_type(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->window_type, GDK_TYPE_WINDOW_TYPE);
}
static VALUE
rg_set_window_type(VALUE self, VALUE val)
{
    _SELF(self)->window_type = RVAL2GENUM(val, GDK_TYPE_WINDOW_TYPE);
    return self;
}

static VALUE
rg_cursor(VALUE self)
{
    return BOXED2RVAL(_SELF(self)->cursor, GDK_TYPE_CURSOR);
}
static VALUE
rg_set_cursor(VALUE self, VALUE val)
{
    _SELF(self)->cursor = (GdkCursor*)(RVAL2BOXED(val, GDK_TYPE_CURSOR));
    return self;
}

ATTR_STR(wmclass_name);
ATTR_STR(wmclass_class);

static VALUE
rg_override_redirect(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->override_redirect);
}
static VALUE
rg_set_override_redirect(VALUE self, VALUE val)
{
    _SELF(self)->override_redirect = RVAL2CBOOL(val);
    return self;
}

void 
Init_gtk_gdk_windowattr(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_WINDOW_ATTR, "WindowAttr", mGdk);

    RG_DEF_METHOD(initialize, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "title", attr_get_title, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_title", attr_set_title, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "event_mask", attr_get_event_mask, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_event_mask", attr_set_event_mask, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "x", attr_get_x, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x", attr_set_x, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "y", attr_get_y, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y", attr_set_y, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "width", attr_get_width, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_width", attr_set_width, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "height", attr_get_height, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_height", attr_set_height, 1);
    RG_DEF_METHOD(wclass, 0);
    RG_DEF_METHOD(set_wclass, 1);
    RG_DEF_METHOD(visual, 0);
    RG_DEF_METHOD(set_visual, 1);
    RG_DEF_METHOD(colormap, 0);
    RG_DEF_METHOD(set_colormap, 1);
    RG_DEF_METHOD(window_type, 0);
    RG_DEF_METHOD(set_window_type, 1);
    RG_DEF_METHOD(cursor, 0);
    RG_DEF_METHOD(set_cursor, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "wmclass_name", attr_get_wmclass_name, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_wmclass_name", attr_set_wmclass_name, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "wmclass_class", attr_get_wmclass_class, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_wmclass_class", attr_set_wmclass_class, 1);
    RG_DEF_METHOD(override_redirect, 0);
    RG_DEF_METHOD(set_override_redirect, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

}
