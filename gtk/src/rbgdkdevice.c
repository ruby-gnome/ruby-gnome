/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdevice.c -

  $Author: mutoh $
  $Date: 2006/06/17 06:59:32 $

  Copyright (C) 2003-2006 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(self) (GDK_DEVICE(RVAL2GOBJ(self)))

/* Methods */
static VALUE
device_s_list(self)
    VALUE self;
{
    /* Owned by GDK */
    return GLIST2ARY(gdk_devices_list());
}

static VALUE
device_s_set_extension_events(self, window, mask, mode)
    VALUE self, window, mask, mode;
{
    gdk_input_set_extension_events(GDK_WINDOW(RVAL2GOBJ(window)),
                                   NUM2INT(mask), FIX2INT(mode));
    return Qnil;
}


static VALUE
device_set_source(self, source)
    VALUE self, source;
{
    gdk_device_set_source(_SELF(self), RVAL2GENUM(source, GDK_TYPE_INPUT_SOURCE));
    return self;
}

static VALUE
device_set_mode(self, mode)
    VALUE self, mode;
{
    return CBOOL2RVAL(gdk_device_set_mode(_SELF(self), RVAL2GENUM(mode, GDK_TYPE_INPUT_MODE)));
}

static VALUE
device_set_key(self, index, keyval, modifiers)
    VALUE self, index, keyval, modifiers;
{
    gdk_device_set_key(_SELF(self), NUM2UINT(index), NUM2UINT(keyval),
                       RVAL2GFLAGS(modifiers, GDK_TYPE_MODIFIER_TYPE));
    return self;
}

static VALUE
device_set_axis_use(self, index, use)
    VALUE self, index, use;
{
    gdk_device_set_axis_use(_SELF(self), NUM2UINT(index), 
                            RVAL2GENUM(use, GDK_TYPE_AXIS_USE));
    return self;
}

static VALUE
device_s_get_core_pointer(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_device_get_core_pointer());
}

static VALUE
device_get_state(self, window)
    VALUE self, window;
{
    gdouble axes[2];
    GdkModifierType mask;

    gdk_device_get_state(_SELF(self), GDK_WINDOW(RVAL2GOBJ(window)),
                         axes, &mask);
    return rb_ary_new3(3, rb_float_new(axes[0]), rb_float_new(axes[1]),
                       GFLAGS2RVAL(mask, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
device_get_history(self, window, start, stop)
    VALUE self, window, start, stop;
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
device_get_axis(self, axes, use)
    VALUE self, axes, use;
{
    gdouble value;
    gboolean ret;
    gdouble* gaxes;
    gint i;
    gint len = RARRAY_LEN(axes);

    gaxes = g_new(gdouble, len);

    for (i = 0; i < len; i++){
        gaxes[i] = RARRAY_PTR(axes)[i];
    }

    ret = gdk_device_get_axis(_SELF(self), gaxes, RVAL2GENUM(use, GDK_TYPE_AXIS_USE),
                              &value);

    g_free(gaxes);

    return ret ? rb_float_new(value) : Qnil;
}

/* Accessor */
static VALUE
device_name(self)
    VALUE self;
{
    return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
device_source(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->source, GDK_TYPE_INPUT_SOURCE);
}

static VALUE
device_mode(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->mode, GDK_TYPE_INPUT_MODE);
}

static VALUE
device_has_cursor(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->has_cursor);
}

static VALUE
device_axes(self)
    VALUE self;
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
device_keys(self)
    VALUE self;
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
Init_gtk_gdk_device()
{
    VALUE dev = G_DEF_CLASS(GDK_TYPE_DEVICE, "Device", mGdk);

    rb_define_singleton_method(dev, "list", device_s_list, 0);
    rb_define_singleton_method(dev, "core_pointer", device_s_get_core_pointer, 0);
    rb_define_singleton_method(dev, "set_extension_events", device_s_set_extension_events, 3);

    rb_define_method(dev, "set_source", device_set_source, 1);
    rb_define_method(dev, "set_mode", device_set_mode, 1);
    rb_define_method(dev, "set_key", device_set_key, 3);
    rb_define_method(dev, "set_axis_use", device_set_axis_use, 2);
    rb_define_method(dev, "get_state", device_get_state, 1);
    rb_define_method(dev, "get_history", device_get_history, 3);
    rb_define_method(dev, "get_axis", device_get_axis, 2);

    rb_define_method(dev, "name", device_name, 0);
    rb_define_method(dev, "source", device_source, 0);
    rb_define_method(dev, "mode", device_mode, 0);
    rb_define_method(dev, "has_cursor?", device_has_cursor, 0);
    rb_define_method(dev, "axes", device_axes, 0);
    rb_define_method(dev, "keys", device_keys, 0);

    G_DEF_SETTERS(dev);

    /* GdkInputSource */
    G_DEF_CLASS(GDK_TYPE_INPUT_SOURCE, "InputSource", dev);
    G_DEF_CONSTANTS(dev, GDK_TYPE_INPUT_SOURCE, "GDK_");

    /* GdkInputMode */
    G_DEF_CLASS(GDK_TYPE_INPUT_MODE, "InputMode", dev);
    G_DEF_CONSTANTS(dev, GDK_TYPE_INPUT_MODE, "GDK_");
    
    /* GdkAxisUse */
    G_DEF_CLASS(GDK_TYPE_AXIS_USE, "AxisUse", dev);
    G_DEF_CONSTANTS(dev, GDK_TYPE_AXIS_USE, "GDK_");

    /* GdkExtensionMode */
    G_DEF_CLASS(GDK_TYPE_EXTENSION_MODE, "ExtensionMode", dev);
    G_DEF_CONSTANTS(dev, GDK_TYPE_EXTENSION_MODE, "GDK_");

}
