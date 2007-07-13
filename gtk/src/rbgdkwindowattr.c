/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkwindowattr.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

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

#define _SELF(w) ((GdkWindowAttr*)RVAL2BOXED(w, GDK_TYPE_WINDOW_ATTR))

static VALUE
attr_initialize(self, width, height, wclass, window_type)
    VALUE self;
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
attr_get_ ## name (self)\
    VALUE self;\
{\
    return CSTR2RVAL(_SELF(self)->name);\
}\
static VALUE \
attr_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RVAL2CSTR(val);\
    return self;\
}

#define ATTR_INT(name)\
static VALUE \
attr_get_ ## name (self)\
    VALUE self;\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
attr_set_ ## name (self, val)\
    VALUE self, val;\
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
attr_get_wclass(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->wclass, GDK_TYPE_WINDOW_CLASS);
}
static VALUE
attr_set_wclass(self, val)
    VALUE self, val;
{
    _SELF(self)->wclass = RVAL2GENUM(val, GDK_TYPE_WINDOW_CLASS);
    return self;
}

static VALUE
attr_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->visual);
}
static VALUE
attr_set_visual(self, val)
    VALUE self, val;
{
    _SELF(self)->visual = GDK_VISUAL(RVAL2GOBJ(val));
    return self;
}

static VALUE
attr_get_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->colormap);
}
static VALUE
attr_set_colormap(self, val)
    VALUE self, val;
{
    _SELF(self)->colormap = GDK_COLORMAP(RVAL2GOBJ(val));
    return self;
}

static VALUE
attr_get_window_type(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->window_type, GDK_TYPE_WINDOW_TYPE);
}
static VALUE
attr_set_window_type(self, val)
    VALUE self, val;
{
    _SELF(self)->window_type = RVAL2GENUM(val, GDK_TYPE_WINDOW_TYPE);
    return self;
}

static VALUE
attr_get_cursor(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->cursor, GDK_TYPE_CURSOR);
}
static VALUE
attr_set_cursor(self, val)
    VALUE self, val;
{
    _SELF(self)->cursor = (GdkCursor*)(RVAL2BOXED(val, GDK_TYPE_CURSOR));
    return self;
}

ATTR_STR(wmclass_name);
ATTR_STR(wmclass_class);

static VALUE
attr_get_override_redirect(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->override_redirect);
}
static VALUE
attr_set_override_redirect(self, val)
    VALUE self, val;
{
    _SELF(self)->override_redirect = RVAL2CBOOL(val);
    return self;
}

void 
Init_gtk_gdk_windowattr()
{
    VALUE gWindowAttr = G_DEF_CLASS(GDK_TYPE_WINDOW_ATTR, "WindowAttr", mGdk);

    rb_define_method(gWindowAttr, "initialize", attr_initialize, 4);
    rb_define_method(gWindowAttr, "title", attr_get_title, 0);
    rb_define_method(gWindowAttr, "set_title", attr_set_title, 1);
    rb_define_method(gWindowAttr, "event_mask", attr_get_event_mask, 0);
    rb_define_method(gWindowAttr, "set_event_mask", attr_set_event_mask, 1);
    rb_define_method(gWindowAttr, "x", attr_get_x, 0);
    rb_define_method(gWindowAttr, "set_x", attr_set_x, 1);
    rb_define_method(gWindowAttr, "y", attr_get_y, 0);
    rb_define_method(gWindowAttr, "set_y", attr_set_y, 1);
    rb_define_method(gWindowAttr, "width", attr_get_width, 0);
    rb_define_method(gWindowAttr, "set_width", attr_set_width, 1);
    rb_define_method(gWindowAttr, "height", attr_get_height, 0);
    rb_define_method(gWindowAttr, "set_height", attr_set_height, 1);
    rb_define_method(gWindowAttr, "wclass", attr_get_wclass, 0);
    rb_define_method(gWindowAttr, "set_wclass", attr_set_wclass, 1);
    rb_define_method(gWindowAttr, "visual", attr_get_visual, 0);
    rb_define_method(gWindowAttr, "set_visual", attr_set_visual, 1);
    rb_define_method(gWindowAttr, "colormap", attr_get_colormap, 0);
    rb_define_method(gWindowAttr, "set_colormap", attr_set_colormap, 1);
    rb_define_method(gWindowAttr, "window_type", attr_get_window_type, 0);
    rb_define_method(gWindowAttr, "set_window_type", attr_set_window_type, 1);
    rb_define_method(gWindowAttr, "cursor", attr_get_cursor, 0);
    rb_define_method(gWindowAttr, "set_cursor", attr_set_cursor, 1);
    rb_define_method(gWindowAttr, "wmclass_name", attr_get_wmclass_name, 0);
    rb_define_method(gWindowAttr, "set_wmclass_name", attr_set_wmclass_name, 1);
    rb_define_method(gWindowAttr, "wmclass_class", attr_get_wmclass_class, 0);
    rb_define_method(gWindowAttr, "set_wmclass_class", attr_set_wmclass_class, 1);
    rb_define_method(gWindowAttr, "override_redirect", attr_get_override_redirect, 0);
    rb_define_method(gWindowAttr, "set_override_redirect", attr_set_override_redirect, 1);

    G_DEF_SETTERS(gWindowAttr);

}
