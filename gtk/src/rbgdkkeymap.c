/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkkeymap.c -

  $Author: mutoh $
  $Date: 2004/03/05 16:24:30 $

  Copyright (C) 2003,2004 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(s) GDK_KEYMAP(RVAL2GOBJ(s))

static VALUE
gdkkeymap_s_get_default(self)
     VALUE self;
{
  return GOBJ2RVAL(gdk_keymap_get_default());
}

static VALUE
gdkkeymap_lookup_key(self, keycode, group, level)
     VALUE self, keycode, group, level;
{
  GdkKeymapKey key;

  key.keycode = NUM2UINT(keycode);
  key.group = NUM2INT(group);
  key.level = NUM2INT(level);

  return INT2NUM(gdk_keymap_lookup_key(_SELF(self), &key));
}

static VALUE
gdkkeymap_translate_keyboard_state(self, hardware_keycode, state, group)
     VALUE self, hardware_keycode, state, group;
{
  guint keyval;
  gint effective_group, level;
  GdkModifierType consumed_modifiers;
  gboolean ret;

  ret = gdk_keymap_translate_keyboard_state(_SELF(self),
                                            NUM2UINT(hardware_keycode),
                                            RVAL2GFLAGS(state, GDK_TYPE_MODIFIER_TYPE),
                                            NUM2INT(group),
                                            &keyval, &effective_group,
                                            &level, &consumed_modifiers);
  return ret ? rb_ary_new3(4, UINT2NUM(keyval), INT2NUM(effective_group),
                           INT2NUM(level), 
                           GFLAGS2RVAL(consumed_modifiers, GDK_TYPE_MODIFIER_TYPE)) : Qnil;
}

static VALUE
gdkkeymap_get_entries_for_keyval(self, keyval)
     VALUE self, keyval;
{
  GdkKeymapKey* keys;
  gint n_keys;
  gboolean ret;
  ret = gdk_keymap_get_entries_for_keyval(_SELF(self),
                                          NUM2UINT(keyval), &keys, &n_keys);

  if (ret){
    VALUE key;
    VALUE ary = rb_ary_new();
    gint i;
    for (i = 0; i < n_keys; i++){
      key = rb_ary_new3(3, UINT2NUM(keys[i].keycode), INT2NUM(keys[i].group), 
                        INT2NUM(keys[i].level));
      rb_ary_push(ary, key);
    }
    g_free(keys);
    return ary;
  } else {
    return Qnil;
  }
}

static VALUE
gdkkeymap_get_entries_for_keycode(self, hardware_keycode)
     VALUE self, hardware_keycode;
{
  GdkKeymapKey* keys;
  guint* keyvals;
  gint n_entries;
  gboolean  ret = gdk_keymap_get_entries_for_keycode(_SELF(self),
                                                     NUM2UINT(hardware_keycode),
                                                     &keys, &keyvals, &n_entries);
  if (ret){
    VALUE key;
    VALUE ary = rb_ary_new();
    gint i;
    for (i = 0; i < n_entries; i++){
      key = rb_ary_new3(4, UINT2NUM(keys[i].keycode), INT2NUM(keys[i].group), 
                        INT2NUM(keys[i].level), UINT2NUM(keyvals[i]));
      rb_ary_push(ary, key);
    }
    g_free(keys);
    g_free(keyvals);
    return ary;
  } else {
    return Qnil;
  }  
}

static VALUE
gdkkeymap_get_direction(self)
     VALUE self;
{
  return GENUM2RVAL(gdk_keymap_get_direction(_SELF(self)), PANGO_TYPE_DIRECTION);
}

void 
Init_gtk_gdk_keymap()
{
    VALUE gdkKeymap = G_DEF_CLASS(GDK_TYPE_KEYMAP, "Keymap", mGdk);

    rb_define_singleton_method(gdkKeymap, "default", gdkkeymap_s_get_default, 0);
    rb_define_method(gdkKeymap, "lookup_key", gdkkeymap_lookup_key, 3);
    rb_define_method(gdkKeymap, "translate_keyboard_state", gdkkeymap_translate_keyboard_state, 3);
    rb_define_method(gdkKeymap, "get_entries_for_keyval", gdkkeymap_get_entries_for_keyval, 1);
    rb_define_method(gdkKeymap, "get_entries_for_keycode", gdkkeymap_get_entries_for_keycode, 1);
    rb_define_method(gdkKeymap, "direction", gdkkeymap_get_direction, 0);

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkKeymapX11", "KeymapX11", mGdk);
#endif

}
