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

#include "global.h"

#define RG_TARGET_NAMESPACE cKeymap
#define _SELF(s) GDK_KEYMAP(RVAL2GOBJ(s))

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
  return GOBJ2RVAL(gdk_keymap_get_default());
}
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_s_for_display(G_GNUC_UNUSED VALUE self, VALUE display)
{
  return GOBJ2RVAL(gdk_keymap_get_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display))));
}
#endif
static VALUE
rg_lookup_key(VALUE self, VALUE keycode, VALUE group, VALUE level)
{
  GdkKeymapKey key;

  key.keycode = NUM2UINT(keycode);
  key.group = NUM2INT(group);
  key.level = NUM2INT(level);

  return INT2NUM(gdk_keymap_lookup_key(_SELF(self), &key));
}

static VALUE
rg_translate_keyboard_state(VALUE self, VALUE hardware_keycode, VALUE state, VALUE group)
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
rg_get_entries_for_keyval(VALUE self, VALUE keyval)
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
rg_get_entries_for_keycode(VALUE self, VALUE hardware_keycode)
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
rg_direction(VALUE self)
{
  return GENUM2RVAL(gdk_keymap_get_direction(_SELF(self)), PANGO_TYPE_DIRECTION);
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_have_bidi_layouts_p(VALUE self)
{
    return CBOOL2RVAL(gdk_keymap_have_bidi_layouts(_SELF(self)));
}
#endif

void 
Init_gtk_gdk_keymap(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_KEYMAP, "Keymap", mGdk);

    RG_DEF_SMETHOD(default, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_SMETHOD(for_display, 0);
#endif
    RG_DEF_METHOD(lookup_key, 3);
    RG_DEF_METHOD(translate_keyboard_state, 3);
    RG_DEF_METHOD(get_entries_for_keyval, 1);
    RG_DEF_METHOD(get_entries_for_keycode, 1);
    RG_DEF_METHOD(direction, 0);
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD_P(have_bidi_layouts, 0);
#endif

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkKeymapX11", "KeymapX11", mGdk);
#endif

}
