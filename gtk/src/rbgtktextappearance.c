/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextappearance.c -
 
  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"

/*****************************************/
static GtkTextAppearance*
app_copy(const GtkTextAppearance* app)
{
  GtkTextAppearance* new_app;
  g_return_val_if_fail (app != NULL, NULL);
  new_app = g_new(GtkTextAppearance, 1);
  *new_app = *app;
  return new_app;
}

GType
gtk_text_appearance_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkTextAppearance",
                    (GBoxedCopyFunc)app_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/

#define _SELF(t) ((GtkTextAppearance*)RVAL2BOXED(t, GTK_TYPE_TEXT_APPEARANCE))

/***********************************************/
#define ATTR_INT(name)\
static VALUE \
txt_app_int_ ## name (self)\
    VALUE self;\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
txt_app_int_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

#define ATTR_BOOL(name)\
static VALUE \
txt_app_bool_ ## name (self)\
    VALUE self;\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
txt_app_bool_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RVAL2CBOOL(val);\
    return self;\
}

#define ATTR_ENUM(name, gtype)\
static VALUE \
txt_app_enums_ ## name (self)\
    VALUE self;\
{\
    return GENUM2RVAL(_SELF(self)->name, gtype);\
}\
static VALUE \
txt_app_enums_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RVAL2GENUM(val, gtype);\
    return self;\
}

#define ATTR_GOBJ(name)\
static VALUE \
txt_app_gobj_ ## name (self)\
    VALUE self;\
{\
    VALUE val; \
    if (_SELF(self)->name == NULL) return Qnil;\
    val = GOBJ2RVAL(_SELF(self)->name);  \
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return val; \
}\
static VALUE \
txt_app_gobj_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RVAL2GOBJ(val);\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return self;\
}

#define ATTR_COLOR(name)\
static VALUE \
txt_app_color_ ## name (self)\
    VALUE self;\
{\
    VALUE val = BOXED2RVAL(&_SELF(self)->name, GDK_TYPE_COLOR);\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return val;\
}\
static VALUE \
txt_app_color_set_ ## name (self, val)\
    VALUE self, val;\
{\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    _SELF(self)->name = *(GdkColor*)RVAL2BOXED(val, GDK_TYPE_COLOR);	\
    return self;\
}

#define DEFINE_ACCESSOR(gt, type, name)			\
    rb_define_method(gt, G_STRINGIFY(name), txt_app_ ## type ## _## name, 0);\
    rb_define_method(gt, G_STRINGIFY(set_ ## name), txt_app_ ## type ## _set_## name, 1);
/***********************************************/
ATTR_COLOR(bg_color);
ATTR_COLOR(fg_color);
ATTR_GOBJ(bg_stipple);
ATTR_GOBJ(fg_stipple);

ATTR_INT(rise);
ATTR_ENUM(underline, PANGO_TYPE_UNDERLINE);
ATTR_BOOL(strikethrough);
ATTR_BOOL(draw_bg);
ATTR_BOOL(inside_selection);
ATTR_BOOL(is_text);

static VALUE
txt_app_initialize(self)
    VALUE self;
{
    GtkTextAppearance* app = ALLOC(GtkTextAppearance);
    memset(app, 0, sizeof(GtkTextAppearance));
    G_INITIALIZE(self, app);
    return Qnil;
}


void
Init_txt_appearance()
{
    VALUE gTextApp = G_DEF_CLASS(GTK_TYPE_TEXT_APPEARANCE, "TextAppearance", mGtk);

    DEFINE_ACCESSOR(gTextApp, color, bg_color);
    DEFINE_ACCESSOR(gTextApp, color, fg_color);
    DEFINE_ACCESSOR(gTextApp, gobj, bg_stipple);
    DEFINE_ACCESSOR(gTextApp, gobj, fg_stipple);
    DEFINE_ACCESSOR(gTextApp, int, rise);
    DEFINE_ACCESSOR(gTextApp, enums, underline);

    rb_define_method(gTextApp, "initialize", txt_app_initialize, 0);
    rb_define_method(gTextApp, "strikethrough?", txt_app_bool_strikethrough, 0);
    rb_define_method(gTextApp, "set_strikethrough", txt_app_bool_set_strikethrough, 1);
    rb_define_method(gTextApp, "draw_bg?", txt_app_bool_draw_bg, 0);
    rb_define_method(gTextApp, "set_draw_bg", txt_app_bool_set_draw_bg, 1);
    rb_define_method(gTextApp, "inside_selection?", txt_app_bool_inside_selection, 0);
    rb_define_method(gTextApp, "set_inside_selection", txt_app_bool_set_inside_selection, 1);
    rb_define_method(gTextApp, "text?", txt_app_bool_is_text, 0);
    rb_define_method(gTextApp, "set_text", txt_app_bool_set_is_text, 1);

    G_DEF_SETTERS(gTextApp);
}
