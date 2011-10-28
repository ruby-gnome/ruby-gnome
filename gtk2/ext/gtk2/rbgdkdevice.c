/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cDevice
#define _SELF(self) (GDK_DEVICE(RVAL2GOBJ(self)))

/* Methods */
static VALUE
device_s_list(G_GNUC_UNUSED VALUE self)
{
    /* Owned by GDK */
    return GLIST2ARY(gdk_devices_list());
}

static VALUE
device_s_set_extension_events(G_GNUC_UNUSED VALUE self, VALUE window, VALUE mask, VALUE mode)
{
    gdk_input_set_extension_events(GDK_WINDOW(RVAL2GOBJ(window)),
                                   NUM2INT(mask), FIX2INT(mode));
    return Qnil;
}


static VALUE
device_set_source(VALUE self, VALUE source)
{
    gdk_device_set_source(_SELF(self), RVAL2GENUM(source, GDK_TYPE_INPUT_SOURCE));
    return self;
}

static VALUE
device_set_mode(VALUE self, VALUE mode)
{
    return CBOOL2RVAL(gdk_device_set_mode(_SELF(self), RVAL2GENUM(mode, GDK_TYPE_INPUT_MODE)));
}

static VALUE
device_set_key(VALUE self, VALUE index, VALUE keyval, VALUE modifiers)
{
    gdk_device_set_key(_SELF(self), NUM2UINT(index), NUM2UINT(keyval),
                       RVAL2GFLAGS(modifiers, GDK_TYPE_MODIFIER_TYPE));
    return self;
}

static VALUE
device_set_axis_use(VALUE self, VALUE index, VALUE use)
{
    gdk_device_set_axis_use(_SELF(self), NUM2UINT(index), 
                            RVAL2GENUM(use, GDK_TYPE_AXIS_USE));
    return self;
}

static VALUE
device_s_get_core_pointer(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_device_get_core_pointer());
}

static VALUE
device_get_state(VALUE self, VALUE window)
{
    gdouble axes[2];
    GdkModifierType mask;

    gdk_device_get_state(_SELF(self), GDK_WINDOW(RVAL2GOBJ(window)),
                         axes, &mask);
    return rb_ary_new3(3, rb_float_new(axes[0]), rb_float_new(axes[1]),
                       GFLAGS2RVAL(mask, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
device_get_history(VALUE self, VALUE window, VALUE start, VALUE stop)
{
    gboolean ret;
    GdkTimeCoord** events;
    gint i, n_events;
    VALUE ary = Qnil;
    ret = gdk_device_get_history(_SELF(self),
                                 GDK_WINDOW(RVAL2GOBJ(window)),
                                 NUM2UINT(start), NUM2UINT(stop),
                                 &events, &n_events);
    if (ret){
        ary = rb_ary_new();
        for (i = 0; i < n_events; i++){
            rb_ary_push(ary, BOXED2RVAL(events, GDK_TYPE_TIME_COORD));
        }
        gdk_device_free_history(events, n_events);
    }
    return ary;
}

static VALUE
device_get_axis(VALUE self, VALUE rbaxes, VALUE rbuse)
{
    GdkDevice *device = _SELF(self);
    GdkAxisUse use = RVAL2GENUM(rbuse, GDK_TYPE_AXIS_USE);
    long n;
    gdouble *axes = RVAL2GDOUBLES(rbaxes, n);
    gint device_n_axes = gdk_device_get_n_axes(device);
    gdouble value;
    gboolean found;

    if (n != device_n_axes)
        rb_raise(rb_eArgError,
                 "unexpected number of axes: %ld != %d",
                 n, device_n_axes);

    found = gdk_device_get_axis(device, axes, use, &value);

    g_free(axes);

    return found ? DBL2NUM(value) : Qnil;
}

/* Accessor */
static VALUE
device_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
device_source(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->source, GDK_TYPE_INPUT_SOURCE);
}

static VALUE
device_mode(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->mode, GDK_TYPE_INPUT_MODE);
}

static VALUE
device_has_cursor(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->has_cursor);
}

static VALUE
device_axes(VALUE self)
{
    gint i;
    VALUE ary = rb_ary_new();
    GdkDeviceAxis* axes = _SELF(self)->axes;

    for (i = 0; i < _SELF(self)->num_axes; i++){
        rb_ary_push(ary, rb_ary_new3(3, GENUM2RVAL(axes[i].use, GDK_TYPE_AXIS_USE), 
                                     rb_float_new(axes[i].min), rb_float_new(axes[i].max)));
    }
    return ary;
}

static VALUE
device_keys(VALUE self)
{
    gint i;
    VALUE ary = rb_ary_new();
    GdkDeviceKey* keys = _SELF(self)->keys;

    for (i = 0; i < _SELF(self)->num_keys; i++){
        rb_ary_push(ary, rb_ary_new3(2, UINT2NUM(keys[i].keyval),
                                     GFLAGS2RVAL(keys[i].modifiers, GDK_TYPE_MODIFIER_TYPE)));
    }
    return ary;
}

void 
Init_gtk_gdk_device(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_DEVICE, "Device", mGdk);

    rb_define_singleton_method(RG_TARGET_NAMESPACE, "list", device_s_list, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "core_pointer", device_s_get_core_pointer, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "set_extension_events", device_s_set_extension_events, 3);

    rb_define_method(RG_TARGET_NAMESPACE, "set_source", device_set_source, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_mode", device_set_mode, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_key", device_set_key, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "set_axis_use", device_set_axis_use, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "get_state", device_get_state, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_history", device_get_history, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "get_axis", device_get_axis, 2);

    rb_define_method(RG_TARGET_NAMESPACE, "name", device_name, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "source", device_source, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "mode", device_mode, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "has_cursor?", device_has_cursor, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "axes", device_axes, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "keys", device_keys, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GdkInputSource */
    G_DEF_CLASS(GDK_TYPE_INPUT_SOURCE, "InputSource", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_INPUT_SOURCE, "GDK_");

    /* GdkInputMode */
    G_DEF_CLASS(GDK_TYPE_INPUT_MODE, "InputMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_INPUT_MODE, "GDK_");
    
    /* GdkAxisUse */
    G_DEF_CLASS(GDK_TYPE_AXIS_USE, "AxisUse", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_AXIS_USE, "GDK_");

    /* GdkExtensionMode */
    G_DEF_CLASS(GDK_TYPE_EXTENSION_MODE, "ExtensionMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_EXTENSION_MODE, "GDK_");

}
