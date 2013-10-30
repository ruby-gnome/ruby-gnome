/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
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

#include "rbgdk3private.h"

#define RG_TARGET_NAMESPACE cDevice
#define _SELF(self) (RVAL2GDKDEVICE(self))

/* deprecated
static VALUE
rg_s_list(G_GNUC_UNUSED VALUE self)
{
    return GOBJGLIST2RVAL(gdk_devices_list());
}

static VALUE
rg_s_set_extension_events(G_GNUC_UNUSED VALUE self, VALUE window, VALUE mask, VALUE mode)
{
    gdk_input_set_extension_events(RVAL2GDKWINDOW(window),
                                   NUM2INT(mask), FIX2INT(mode));
    return Qnil;
}

static VALUE
rg_s_core_pointer(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_device_get_core_pointer());
}

static VALUE
rg_set_source(VALUE self, VALUE source)
{
    gdk_device_set_source(_SELF(self), RVAL2GDKINPUTSOURCE(source));
    return self;
}
*/

static VALUE
rg_set_mode(VALUE self, VALUE mode)
{
    return CBOOL2RVAL(gdk_device_set_mode(_SELF(self), RVAL2GDKINPUTMODE(mode)));
}

static VALUE
rg_set_key(VALUE self, VALUE index, VALUE keyval, VALUE modifiers)
{
    gdk_device_set_key(_SELF(self), NUM2UINT(index), NUM2UINT(keyval),
                       RVAL2GDKMODIFIERTYPE(modifiers));
    return self;
}

static VALUE
rg_set_axis_use(VALUE self, VALUE index, VALUE use)
{
    gdk_device_set_axis_use(_SELF(self), NUM2UINT(index),
                            RVAL2GDKAXISUSE(use));
    return self;
}

static VALUE
rg_get_state(VALUE self, VALUE window)
{
    gdouble axes[2];
    GdkModifierType mask;

    gdk_device_get_state(_SELF(self), RVAL2GDKWINDOW(window),
                         axes, &mask);
    return rb_ary_new3(3, rb_float_new(axes[0]), rb_float_new(axes[1]),
                       GDKMODIFIERTYPE2RVAL(mask));
}

static VALUE
rg_get_history(VALUE self, VALUE window, VALUE start, VALUE stop)
{
    gboolean ret;
    GdkTimeCoord** events;
    gint i, n_events;
    VALUE ary = Qnil;
    ret = gdk_device_get_history(_SELF(self),
                                 RVAL2GDKWINDOW(window),
                                 NUM2UINT(start), NUM2UINT(stop),
                                 &events, &n_events);
    if (ret){
        ary = rb_ary_new();
        for (i = 0; i < n_events; i++){
            rb_ary_push(ary, GDKTIMECOORD2RVAL(events));
        }
        gdk_device_free_history(events, n_events);
    }
    return ary;
}

static VALUE
rg_get_axis(VALUE self, VALUE rbaxes, VALUE rbuse)
{
    GdkDevice *device = _SELF(self);
    GdkAxisUse use = RVAL2GDKAXISUSE(rbuse);
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

static VALUE
rg_mode(VALUE self)
{
    return GDKINPUTMODE2RVAL(gdk_device_get_mode(_SELF(self)));
}

static VALUE
rg_get_window_at_position(G_GNUC_UNUSED VALUE self)
{
    gint x, y;
    GdkWindow *win = gdk_device_get_window_at_position(_SELF(self), &x ,&y);
    return rb_ary_new3(3, GOBJ2RVAL(win), INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_ungrab(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_time;
    guint32 time = GDK_CURRENT_TIME;

    rb_scan_args(argc, argv, "01", &rb_time);
    if (!NIL_P(rb_time)) {
        time = NUM2UINT(GDK_CURRENT_TIME);
    }
    gdk_device_ungrab(_SELF(self), time);
    return self;
}

static VALUE
rg_get_position(VALUE self)
{
    GdkScreen *screen;
    int x, y;
    gdk_device_get_position(_SELF(self),  &screen, &x, &y);
    return rb_ary_new3(3, GOBJ2RVAL(screen), INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_warp(VALUE self, VALUE screen, VALUE x, VALUE y)
{
    gdk_device_warp(_SELF(self), RVAL2GDKSCREEN(screen), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_grab(VALUE self, VALUE win, VALUE grab_ownership, VALUE owner_events, VALUE event_mask, VALUE cursor, VALUE time)
{
    return GDKGRABSTATUS2RVAL(gdk_device_grab(_SELF(self),
                                              RVAL2GDKWINDOW(win),
                                              RVAL2GDKGRABOWNERSHIP(grab_ownership),
                                              RVAL2CBOOL(owner_events),
                                              RVAL2GDKEVENTMASK(event_mask),
                                              NIL_P(cursor) ? NULL : RVAL2GDKCURSOR(cursor),
                                              NUM2INT(time)));
}

/* deprecated
static VALUE
rg_axes(VALUE self)
{
    gint i;
    VALUE ary = rb_ary_new();
    GdkDeviceAxis* axes = _SELF(self)->axes;

    for (i = 0; i < _SELF(self)->num_axes; i++){
        rb_ary_push(ary, rb_ary_new3(3, GDKAXISUSE2RVAL(axes[i].use),
                                     rb_float_new(axes[i].min), rb_float_new(axes[i].max)));
    }
    return ary;
}

static VALUE
rg_keys(VALUE self)
{
    gint i;
    VALUE ary = rb_ary_new();
    GdkDeviceKey* keys = _SELF(self)->keys;

    for (i = 0; i < _SELF(self)->num_keys; i++){
        rb_ary_push(ary, rb_ary_new3(2, UINT2NUM(keys[i].keyval),
                                     GDKMODIFIERTYPE2RVAL(keys[i].modifiers)));
    }
    return ary;
}
*/

void
Init_gdk_device(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_DEVICE, "Device", mGdk);

/* deprecated
    RG_DEF_SMETHOD(list, 0);
    RG_DEF_SMETHOD(set_extension_events, 3);
    RG_DEF_SMETHOD(core_pointer, 0);

    RG_DEF_METHOD(set_source, 1);
*/
    RG_DEF_METHOD(set_mode, 1);
    RG_DEF_METHOD(set_key, 3);
    RG_DEF_METHOD(set_axis_use, 2);
    RG_DEF_METHOD(get_state, 1);
    RG_DEF_METHOD(get_history, 3);
    RG_DEF_METHOD(get_axis, 2);
    RG_DEF_METHOD(mode, 0);
    RG_DEF_METHOD(get_window_at_position, 0);
    RG_DEF_METHOD(ungrab, -1);
    RG_DEF_METHOD(get_position, 0);
    RG_DEF_METHOD(warp, 3);
    RG_DEF_METHOD(grab, 6);

/* deprecated
    RG_DEF_METHOD(axes, 0);
    RG_DEF_METHOD(keys, 0);
*/

    G_DEF_CLASS(GDK_TYPE_INPUT_SOURCE, "InputSource", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_INPUT_MODE, "InputMode", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_AXIS_USE, "AxisUse", RG_TARGET_NAMESPACE);
}
