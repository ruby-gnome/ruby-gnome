/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2013 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

static VALUE rb_cGdkEvent;
static VALUE rb_cGdkEventAny;
static VALUE rb_cGdkEventExpose;
static VALUE rb_cGdkEventNoExpose;
static VALUE rb_cGdkEventVisibility;
static VALUE rb_cGdkEventMotion;
static VALUE rb_cGdkEventButton;
static VALUE rb_cGdkEventScroll;
static VALUE rb_cGdkEventKey;
static VALUE rb_cGdkEventCrossing;
static VALUE rb_cGdkEventFocus;
static VALUE rb_cGdkEventConfigure;
static VALUE rb_cGdkEventProperty;
static VALUE rb_cGdkEventSelection;
static VALUE rb_cGdkEventOwnerChange;
static VALUE rb_cGdkEventProximity;
static VALUE rb_cGdkEventClient;
static VALUE rb_cGdkEventDND;
static VALUE rb_cGdkEventWindowState;
static VALUE rb_cGdkEventSetting;
static VALUE rb_cGdkEventGrabBroken;

#define DEFINE_EVENT_TYPE(type_lower_case, type_upper_case)             \
static GType                                                            \
rb_gdk_event_ ## type_lower_case ## _get_type(void)                     \
{                                                                       \
    static GType type = 0;                                              \
    if (type == 0) {                                                    \
        type = g_boxed_type_register_static("GdkEvent" # type_upper_case, \
                                            (GBoxedCopyFunc)gdk_event_copy, \
                                            (GBoxedFreeFunc)gdk_event_free); \
    }                                                                   \
    return type;                                                        \
}

DEFINE_EVENT_TYPE(any, Any)
DEFINE_EVENT_TYPE(expose, Expose)
DEFINE_EVENT_TYPE(no_expose, NoExpose)
DEFINE_EVENT_TYPE(visibility, Visibility)
DEFINE_EVENT_TYPE(motion, Motion)
DEFINE_EVENT_TYPE(button, Button)
DEFINE_EVENT_TYPE(scroll, Scroll)
DEFINE_EVENT_TYPE(key, Key)
DEFINE_EVENT_TYPE(crossing, Crossing)
DEFINE_EVENT_TYPE(focus, Focus)
DEFINE_EVENT_TYPE(configure, Configure)
DEFINE_EVENT_TYPE(property, Property)
DEFINE_EVENT_TYPE(selection, Selection)
DEFINE_EVENT_TYPE(owner_change, OwnerChange)
DEFINE_EVENT_TYPE(proximity, Proximity)
DEFINE_EVENT_TYPE(client, Client)
DEFINE_EVENT_TYPE(dnd, DND)
DEFINE_EVENT_TYPE(window_state, WindowState)
DEFINE_EVENT_TYPE(setting, Setting)
DEFINE_EVENT_TYPE(grab_broken, GrabBroken)

#define GDK_TYPE_EVENT_ANY          (rb_gdk_event_any_get_type())
#define GDK_TYPE_EVENT_EXPOSE       (rb_gdk_event_expose_get_type())
#define GDK_TYPE_EVENT_NO_EXPOSE    (rb_gdk_event_no_expose_get_type())
#define GDK_TYPE_EVENT_VISIBILITY   (rb_gdk_event_visibility_get_type())
#define GDK_TYPE_EVENT_MOTION       (rb_gdk_event_motion_get_type())
#define GDK_TYPE_EVENT_BUTTON       (rb_gdk_event_button_get_type())
#define GDK_TYPE_EVENT_SCROLL       (rb_gdk_event_scroll_get_type())
#define GDK_TYPE_EVENT_KEY          (rb_gdk_event_key_get_type())
#define GDK_TYPE_EVENT_CROSSING     (rb_gdk_event_crossing_get_type())
#define GDK_TYPE_EVENT_FOCUS        (rb_gdk_event_focus_get_type())
#define GDK_TYPE_EVENT_CONFIGURE    (rb_gdk_event_configure_get_type())
#define GDK_TYPE_EVENT_PROPERTY     (rb_gdk_event_property_get_type())
#define GDK_TYPE_EVENT_SELECTION    (rb_gdk_event_selection_get_type())
#define GDK_TYPE_EVENT_OWNER_CHANGE (rb_gdk_event_owner_change_get_type())
#define GDK_TYPE_EVENT_PROXIMITY    (rb_gdk_event_proximity_get_type())
#define GDK_TYPE_EVENT_CLIENT       (rb_gdk_event_client_get_type())
#define GDK_TYPE_EVENT_DND          (rb_gdk_event_dnd_get_type())
#define GDK_TYPE_EVENT_WINDOW_STATE (rb_gdk_event_window_state_get_type())
#define GDK_TYPE_EVENT_SETTING      (rb_gdk_event_setting_get_type())
#define GDK_TYPE_EVENT_GRAB_BROKEN  (rb_gdk_event_grab_broken_get_type())

/***********************************************/

static GType
rb_gdk_event_type_to_gtype(GdkEventType event_type)
{
    GType gtype = GDK_TYPE_EVENT_ANY;

    switch (event_type) {
      case GDK_NOTHING:
      case GDK_DELETE:
      case GDK_DESTROY:
        break;
      case GDK_EXPOSE:
        gtype = GDK_TYPE_EVENT_EXPOSE;
        break;
      case GDK_MOTION_NOTIFY:
        gtype = GDK_TYPE_EVENT_MOTION;
        break;
      case GDK_BUTTON_PRESS:
      case GDK_2BUTTON_PRESS:
      case GDK_3BUTTON_PRESS:
      case GDK_BUTTON_RELEASE:
        gtype = GDK_TYPE_EVENT_BUTTON;
        break;
      case GDK_KEY_PRESS:
      case GDK_KEY_RELEASE:
        gtype = GDK_TYPE_EVENT_KEY;
        break;
      case GDK_ENTER_NOTIFY:
      case GDK_LEAVE_NOTIFY:
        gtype = GDK_TYPE_EVENT_CROSSING;
        break;
      case GDK_FOCUS_CHANGE:
        gtype = GDK_TYPE_EVENT_FOCUS;
        break;
      case GDK_CONFIGURE:
        gtype = GDK_TYPE_EVENT_CONFIGURE;
        break;
      case GDK_MAP:
      case GDK_UNMAP:
        break;
      case GDK_PROPERTY_NOTIFY:
        gtype = GDK_TYPE_EVENT_PROPERTY;
        break;
      case GDK_SELECTION_CLEAR:
      case GDK_SELECTION_REQUEST:
      case GDK_SELECTION_NOTIFY:
        gtype = GDK_TYPE_EVENT_SELECTION;
        break;
      case GDK_PROXIMITY_IN:
      case GDK_PROXIMITY_OUT:
        gtype = GDK_TYPE_EVENT_PROXIMITY;
        break;
      case GDK_DRAG_ENTER:
      case GDK_DRAG_LEAVE:
      case GDK_DRAG_MOTION:
      case GDK_DRAG_STATUS:
      case GDK_DROP_START:
      case GDK_DROP_FINISHED:
        gtype = GDK_TYPE_EVENT_DND;
        break;
      case GDK_CLIENT_EVENT:
        gtype = GDK_TYPE_EVENT_CLIENT;
        break;
      case GDK_VISIBILITY_NOTIFY:
        gtype = GDK_TYPE_EVENT_VISIBILITY;
        break;
      case GDK_NO_EXPOSE:
        gtype = GDK_TYPE_EVENT_NO_EXPOSE;
        break;
      case GDK_SCROLL:
        gtype = GDK_TYPE_EVENT_SCROLL;
        break;
      case GDK_WINDOW_STATE:
        gtype = GDK_TYPE_EVENT_WINDOW_STATE;
        break;
      case GDK_SETTING:
        gtype = GDK_TYPE_EVENT_SETTING;
        break;
      case GDK_OWNER_CHANGE:
        gtype = GDK_TYPE_EVENT_OWNER_CHANGE;
        break;
      case GDK_GRAB_BROKEN:
        gtype = GDK_TYPE_EVENT_GRAB_BROKEN;
        break;
#if GTK_CHECK_VERSION(2, 14, 0)
      case GDK_DAMAGE:
        gtype = GDK_TYPE_EVENT_EXPOSE;
        break;
#endif
#if GTK_CHECK_VERSION(2, 18, 0)
      case GDK_EVENT_LAST:
        break;
#endif
      default:
        break;
    }

    return gtype;
}

static VALUE
rb_gdk_event_type_to_class(GdkEventType event_type)
{
    VALUE klass = Qnil;

    switch (event_type) {
      case GDK_NOTHING:
      case GDK_DELETE:
      case GDK_DESTROY:
        break;
      case GDK_EXPOSE:
        klass = rb_cGdkEventExpose;
        break;
      case GDK_MOTION_NOTIFY:
        klass = rb_cGdkEventMotion;
        break;
      case GDK_BUTTON_PRESS:
      case GDK_2BUTTON_PRESS:
      case GDK_3BUTTON_PRESS:
      case GDK_BUTTON_RELEASE:
        klass = rb_cGdkEventButton;
        break;
      case GDK_KEY_PRESS:
      case GDK_KEY_RELEASE:
        klass = rb_cGdkEventKey;
        break;
      case GDK_ENTER_NOTIFY:
      case GDK_LEAVE_NOTIFY:
        klass = rb_cGdkEventCrossing;
        break;
      case GDK_FOCUS_CHANGE:
        klass = rb_cGdkEventFocus;
        break;
      case GDK_CONFIGURE:
        klass = rb_cGdkEventConfigure;
        break;
      case GDK_MAP:
      case GDK_UNMAP:
        break;
      case GDK_PROPERTY_NOTIFY:
        klass = rb_cGdkEventProperty;
        break;
      case GDK_SELECTION_CLEAR:
      case GDK_SELECTION_REQUEST:
      case GDK_SELECTION_NOTIFY:
        klass = rb_cGdkEventSelection;
        break;
      case GDK_PROXIMITY_IN:
      case GDK_PROXIMITY_OUT:
        klass = rb_cGdkEventProximity;
        break;
      case GDK_DRAG_ENTER:
      case GDK_DRAG_LEAVE:
      case GDK_DRAG_MOTION:
      case GDK_DRAG_STATUS:
      case GDK_DROP_START:
      case GDK_DROP_FINISHED:
        klass = rb_cGdkEventDND;
        break;
      case GDK_CLIENT_EVENT:
        klass = rb_cGdkEventClient;
        break;
      case GDK_VISIBILITY_NOTIFY:
        klass = rb_cGdkEventVisibility;
        break;
      case GDK_NO_EXPOSE:
        klass = rb_cGdkEventNoExpose;
        break;
      case GDK_SCROLL:
        klass = rb_cGdkEventScroll;
        break;
      case GDK_WINDOW_STATE:
        klass = rb_cGdkEventWindowState;
        break;
      case GDK_SETTING:
        klass = rb_cGdkEventSetting;
        break;
      case GDK_OWNER_CHANGE:
        klass = rb_cGdkEventOwnerChange;
        break;
      case GDK_GRAB_BROKEN:
        klass = rb_cGdkEventGrabBroken;
        break;
#if GTK_CHECK_VERSION(2, 14, 0)
      case GDK_DAMAGE:
        klass = rb_cGdkEventExpose;
        break;
#endif
#if GTK_CHECK_VERSION(2, 18, 0)
      case GDK_EVENT_LAST:
        break;
#endif
      default:
        break;
    }

    if (NIL_P(klass)) {
        klass = rb_cGdkEventAny;
    }

    return klass;
}

static GType
rb_gdk_event_to_gtype(VALUE event)
{
    VALUE klass;
    GType type = GDK_TYPE_EVENT;

    klass = rb_obj_class(event);
    if (klass == rb_cGdkEvent) {
        type = GDK_TYPE_EVENT;
    } else if (klass == rb_cGdkEventAny) {
        type = GDK_TYPE_EVENT_ANY;
    } else if (klass == rb_cGdkEventExpose) {
        type = GDK_TYPE_EVENT_EXPOSE;
    } else if (klass == rb_cGdkEventNoExpose) {
        type = GDK_TYPE_EVENT_NO_EXPOSE;
    } else if (klass == rb_cGdkEventVisibility) {
        type = GDK_TYPE_EVENT_VISIBILITY;
    } else if (klass == rb_cGdkEventMotion) {
        type = GDK_TYPE_EVENT_MOTION;
    } else if (klass == rb_cGdkEventButton) {
        type = GDK_TYPE_EVENT_BUTTON;
    } else if (klass == rb_cGdkEventScroll) {
        type = GDK_TYPE_EVENT_SCROLL;
    } else if (klass == rb_cGdkEventKey) {
        type = GDK_TYPE_EVENT_KEY;
    } else if (klass == rb_cGdkEventCrossing) {
        type = GDK_TYPE_EVENT_CROSSING;
    } else if (klass == rb_cGdkEventFocus) {
        type = GDK_TYPE_EVENT_FOCUS;
    } else if (klass == rb_cGdkEventConfigure) {
        type = GDK_TYPE_EVENT_CONFIGURE;
    } else if (klass == rb_cGdkEventProperty) {
        type = GDK_TYPE_EVENT_PROPERTY;
    } else if (klass == rb_cGdkEventSelection) {
        type = GDK_TYPE_EVENT_SELECTION;
    } else if (klass == rb_cGdkEventOwnerChange) {
        type = GDK_TYPE_EVENT_OWNER_CHANGE;
    } else if (klass == rb_cGdkEventProximity) {
        type = GDK_TYPE_EVENT_PROXIMITY;
    } else if (klass == rb_cGdkEventClient) {
        type = GDK_TYPE_EVENT_CLIENT;
    } else if (klass == rb_cGdkEventDND) {
        type = GDK_TYPE_EVENT_DND;
    } else if (klass == rb_cGdkEventWindowState) {
        type = GDK_TYPE_EVENT_WINDOW_STATE;
    } else if (klass == rb_cGdkEventSetting) {
        type = GDK_TYPE_EVENT_SETTING;
    } else if (klass == rb_cGdkEventGrabBroken) {
        type = GDK_TYPE_EVENT_GRAB_BROKEN;
    } else {
        rb_raise(rb_eArgError, "Not event object: %s", RBG_INSPECT(event));
    }

    return type;
}

VALUE
make_gdkevent(GdkEvent *event)
{
    if (!event)
        return Qnil;

    return BOXED2RVAL(event, rb_gdk_event_type_to_gtype(event->any.type));
}

GdkEvent*
get_gdkevent(VALUE event)
{
    return RVAL2BOXED(event, rb_gdk_event_to_gtype(event));
}

/***********************************************/
#define ATTR_STR(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return CSTR2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = (gchar *)RVAL2CSTR(val);\
    return self;\
}

#define ATTR_INT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return INT2NUM(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = NUM2INT(val);\
    return self;\
}

#define ATTR_UINT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return UINT2NUM(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = NUM2UINT(val);\
    return self;\
}

#define ATTR_GDKNATIVEWINDOW(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return GDKNATIVEWINDOW2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = RVAL2GDKNATIVEWINDOW(val);\
    return self;\
}

#define ATTR_FLOAT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return rb_float_new(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = NUM2DBL(val);\
    return self;\
}

#define ATTR_GOBJ(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return GOBJ2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    GdkEvent *event;\
    event = get_gdkevent(self);\
    if (event->type.name)\
      g_object_unref(event->type.name);\
    event->type.name = RVAL2GOBJ(val);\
    if (event->type.name)\
      g_object_ref(event->type.name);\
    return self;\
}

#define ATTR_BOOL(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return CBOOL2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = RVAL2CBOOL(val);\
    return self;\
}

#define ATTR_ATOM(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    GdkAtom atom = get_gdkevent(self)->type.name;\
    return BOXED2RVAL(atom, GDK_TYPE_ATOM);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = RVAL2ATOM(val);\
    return self;\
}

#define ATTR_FLAGS(type, name, gtype)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return GFLAGS2RVAL(get_gdkevent(self)->type.name, gtype);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = RVAL2GFLAGS(val, gtype);\
    return self;\
}

#define ATTR_ENUM(type, name, gtype)\
static VALUE \
gdkevent ## type ## _ ## name (VALUE self)\
{\
    return GENUM2RVAL(get_gdkevent(self)->type.name, gtype);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (VALUE self, VALUE val)\
{\
    get_gdkevent(self)->type.name = RVAL2GENUM(val, gtype);\
    return self;\
}

#define ATTR_AXES(type, gdkklass) \
static VALUE \
gdkevent ##type ## _axes(VALUE self)\
{\
    gdkklass type = get_gdkevent(self)->type;\
    return type.axes ? rb_ary_new3(2, \
                       rb_float_new(type.axes[0]),\
                       rb_float_new(type.axes[1])) : Qnil;\
} \
static VALUE \
gdkevent ## type ## _set_axes(VALUE self, VALUE x, VALUE y)\
{\
    gdkklass val = get_gdkevent(self)->type;\
    val.axes[0] = NUM2DBL(x);\
    val.axes[1] = NUM2DBL(y);\
    return self;\
}

#define DEFINE_ACCESSOR(event, type, name) \
    rb_define_method(event, G_STRINGIFY(name), gdkevent ## type ## _## name, 0);\
    rb_define_method(event, G_STRINGIFY(set_ ## name), gdkevent ## type ## _set_## name, 1);


/* initialize */
#define GDKEVENT_INIT(type, default_gtype) \
static VALUE \
gdkevent ## type ## _initialize(int argc, VALUE *argv, VALUE self)\
{\
    VALUE type;\
    GdkEventType gtype;\
\
    rb_scan_args(argc, argv, "01", &type);\
    if (NIL_P(type)){\
        gtype = default_gtype;\
    } else {\
        gtype = RVAL2GENUM(type, GDK_TYPE_EVENT_TYPE);\
    }\
\
    G_INITIALIZE(self, gdk_event_new(gtype));\
    return Qnil;\
}

#define DEFINE_INIT(event, type)                                         \
  rb_define_method(event, "initialize", gdkevent ## type ## _initialize, -1);

/***********************************************/

/* GdkEvent Singleton Methods */
static VALUE
gdkevent_s_events_pending(G_GNUC_UNUSED VALUE self)
{
    return CBOOL2RVAL(gdk_events_pending());
}

static VALUE
gdkevent_s_peek(G_GNUC_UNUSED VALUE self)
{
    return make_gdkevent(gdk_event_peek());
}

static VALUE
gdkevent_s_get(G_GNUC_UNUSED VALUE self)
{
    return make_gdkevent(gdk_event_get());
}

static VALUE
gdkevent_s_get_graphics_expose(G_GNUC_UNUSED VALUE self, VALUE window)
{
    return make_gdkevent(gdk_event_get_graphics_expose(GDK_WINDOW(RVAL2GOBJ(window))));
}

/* GdkEvent */
static VALUE
gdkevent_initialize(VALUE self, VALUE type)
{
    GdkEventType event_type;
    VALUE klass;

    event_type = RVAL2GENUM(type, GDK_TYPE_EVENT_TYPE);
    klass = rb_gdk_event_type_to_class(event_type);
    if (!RVAL2CBOOL(rb_obj_is_kind_of(self, klass))) {
        rb_raise(rb_eArgError, "Wrong event type for this class: %s: %s",
                 RBG_INSPECT(type), RBG_INSPECT(klass));
    }

    G_INITIALIZE(self, gdk_event_new(event_type));
    return Qnil;
}

static VALUE
gdkevent_type(VALUE self)
{
    return GENUM2RVAL(get_gdkevent(self)->type, GDK_TYPE_EVENT_TYPE);
}

static VALUE
gdkevent_put(VALUE self)
{
    gdk_event_put(get_gdkevent(self));
    return self;
}

/* We don't need this.
gdk_event_get_time();
gboolean    gdk_event_get_state             (GdkEvent *event,
                                             GdkModifierType *state);
*/

static VALUE
gdkevent_get_axis(VALUE self, VALUE axis_use)
{
    gdouble value;
    gboolean ret = gdk_event_get_axis(get_gdkevent(self),
                                      RVAL2GENUM(axis_use, GDK_TYPE_AXIS_USE), &value);
    return ret ? rb_float_new(value) : Qnil;
}

static VALUE
gdkevent_get_coords(VALUE self)
{
    gdouble x_win, y_win;
    gboolean ret = gdk_event_get_coords(get_gdkevent(self), &x_win, &y_win);

    return ret ? rb_ary_new3(2, rb_float_new(x_win), rb_float_new(y_win)) : Qnil;
}

static VALUE
gdkevent_get_root_coords(VALUE self)
{
    gdouble x_root, y_root;
    gboolean ret = gdk_event_get_root_coords(get_gdkevent(self), &x_root, &y_root);

    return ret ? rb_ary_new3(2, rb_float_new(x_root), rb_float_new(y_root)) : Qnil;
}

static void
handler_func(GdkEvent *event, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 1, make_gdkevent(event));
}

static VALUE
gdkevent_s_handler_set(VALUE self)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);

    gdk_event_handler_set((GdkEventFunc)handler_func, (gpointer)func, NULL);
    return self;
}

static VALUE
gdkevent_s_get_show_events(G_GNUC_UNUSED VALUE self)
{
    return CBOOL2RVAL(gdk_get_show_events());
}

static VALUE
gdkevent_s_set_show_events(VALUE self, VALUE show_events)
{
    gdk_set_show_events(RVAL2CBOOL(show_events));
    return self;
}

static VALUE
gdkevent_set_screen(VALUE self, VALUE screen)
{
    gdk_event_set_screen(get_gdkevent(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
gdkevent_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_event_get_screen(get_gdkevent(self)));
}

/*
  type: String, Integer, Gdk::Color.
 */
static VALUE
gdkevent_s_setting_get(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE name, type;
    GType gtype;
    GValue val = G_VALUE_INIT;
    gboolean ret;
    VALUE value;

    rb_scan_args(argc, argv, "11", &name, &type);
    if (NIL_P(type))
        gtype = G_TYPE_STRING;
    else
        gtype = CLASS2GTYPE(type);

    g_value_init(&val, gtype);
    ret = gdk_setting_get(RVAL2CSTR(name), &val);

    value = ret ? GVAL2RVAL(&val) : Qnil;
    g_value_unset(&val);
    return value;
}

/* GdkEventAny */
ATTR_GOBJ(any, window);
ATTR_BOOL(any, send_event);

/* GdkEventKey */
ATTR_UINT(key, time);
ATTR_FLAGS(key, state, GDK_TYPE_MODIFIER_TYPE);
ATTR_INT(key, keyval);
ATTR_UINT(key, hardware_keycode);
ATTR_UINT(key, group);

/* GdkEventButton */
ATTR_UINT(button, time);
ATTR_FLOAT(button, x);
ATTR_FLOAT(button, y);
ATTR_AXES(button, GdkEventButton);
ATTR_INT(button, button);
ATTR_FLAGS(button, state, GDK_TYPE_MODIFIER_TYPE);
ATTR_GOBJ(button, device);
ATTR_FLOAT(button, x_root);
ATTR_FLOAT(button, y_root);

/* GdkEventScroll */
ATTR_UINT(scroll, time);
ATTR_FLOAT(scroll, x);
ATTR_FLOAT(scroll, y);
ATTR_FLAGS(scroll, state, GDK_TYPE_MODIFIER_TYPE);
ATTR_ENUM(scroll, direction, GDK_TYPE_SCROLL_DIRECTION);
ATTR_GOBJ(scroll, device);
ATTR_FLOAT(scroll, x_root);
ATTR_FLOAT(scroll, y_root);
GDKEVENT_INIT(scroll, GDK_SCROLL);

/* GdkEventMotion */
ATTR_UINT(motion, time);
ATTR_FLOAT(motion, x);
ATTR_FLOAT(motion, y);
ATTR_AXES(motion, GdkEventMotion);
ATTR_FLOAT(motion, x_root);
ATTR_FLOAT(motion, y_root);
ATTR_FLAGS(motion, state, GDK_TYPE_MODIFIER_TYPE);
ATTR_BOOL(motion, is_hint);
ATTR_GOBJ(motion, device);
GDKEVENT_INIT(motion, GDK_MOTION_NOTIFY);

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkeventmotion_request_motions(VALUE self)
{
    gdk_event_request_motions(&(get_gdkevent(self)->motion));
    return self;
}
#endif

/* GdkEventExpose */
static VALUE
gdkeventexpose_area(VALUE self)
{
    return BOXED2RVAL(&get_gdkevent(self)->expose.area, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkeventexpose_set_area(VALUE self, VALUE rect)
{
    GdkRectangle* grect = (GdkRectangle*)RVAL2BOXED(rect, GDK_TYPE_RECTANGLE);
    GdkEventExpose *event = &(get_gdkevent(self)->expose);
    event->area.x = grect->x;
    event->area.y = grect->y;
    event->area.width = grect->width;
    event->area.height = grect->height;
    return self;
}

static VALUE
gdkeventexpose_region(VALUE self)
{
    return BOXED2RVAL(get_gdkevent(self)->expose.region, GDK_TYPE_REGION);
}
static VALUE
gdkeventexpose_set_region(VALUE self, VALUE region)
{
    get_gdkevent(self)->expose.region = RVAL2BOXED(region, GDK_TYPE_REGION);
    return self;
}

ATTR_INT(expose, count);
GDKEVENT_INIT(expose, GDK_EXPOSE);

/* GdkEventVisibility */
ATTR_ENUM(visibility, state, GDK_TYPE_VISIBILITY_STATE);
GDKEVENT_INIT(visibility, GDK_VISIBILITY_NOTIFY);

/* GdkEventCrossing */
ATTR_GOBJ(crossing, subwindow);
ATTR_UINT(crossing, time);
ATTR_INT(crossing, x);
ATTR_INT(crossing, y);
ATTR_FLOAT(crossing, x_root);
ATTR_FLOAT(crossing, y_root);
ATTR_ENUM(crossing, mode, GDK_TYPE_CROSSING_MODE);
ATTR_ENUM(crossing, detail, GDK_TYPE_NOTIFY_TYPE);
ATTR_BOOL(crossing, focus);
ATTR_FLAGS(crossing, state, GDK_TYPE_MODIFIER_TYPE);

/* GdkEventFocus */
ATTR_BOOL(focus_change, in);
GDKEVENT_INIT(focus_change, GDK_FOCUS_CHANGE);

/* GdkEventConfigure */
ATTR_INT(configure, x);
ATTR_INT(configure, y);
ATTR_INT(configure, width);
ATTR_INT(configure, height);
GDKEVENT_INIT(configure, GDK_CONFIGURE);

/* GdkEventProperty */
ATTR_ATOM(property, atom);
ATTR_UINT(property, time);
ATTR_ENUM(property, state, GDK_TYPE_PROPERTY_STATE);
GDKEVENT_INIT(property, GDK_PROPERTY_NOTIFY);

/* GdkEventSelection */
ATTR_ATOM(selection, selection);
ATTR_ATOM(selection, target);
ATTR_ATOM(selection, property);
ATTR_GDKNATIVEWINDOW(selection, requestor);
ATTR_INT(selection, time);

/* GdkEventDND */
ATTR_GOBJ(dnd, context);
ATTR_UINT(dnd, time);
ATTR_INT(dnd, x_root);
ATTR_INT(dnd, y_root);

/* GdkEventProximity */
ATTR_UINT(proximity, time);
ATTR_GOBJ(proximity, device);

/* GdkEventClient */
ATTR_ATOM(client, message_type);
GDKEVENT_INIT(client, GDK_CLIENT_EVENT);

static VALUE
gdkeventclient_data_format(VALUE self)
{
    return INT2NUM(get_gdkevent(self)->client.data_format);
}

static VALUE
gdkeventclient_data(VALUE self)
{
    int i;
    VALUE ary = Qnil;
    gushort format = get_gdkevent(self)->client.data_format;

    if (format == 8) {
        ary = rb_ary_new2(20);
        for (i = 0; i < 20; i++)
            rb_ary_push(ary, INT2FIX(get_gdkevent(self)->client.data.b[i]));
    } else if (format == 16) {
        ary = rb_ary_new2(10);
        for (i = 0; i<10; i++)
            rb_ary_push(ary, INT2FIX(get_gdkevent(self)->client.data.s[i]));
    } else if (format == 32){
        ary = rb_ary_new2(5);
        for (i = 0; i < 5; i++)
            rb_ary_push(ary, INT2NUM(get_gdkevent(self)->client.data.l[i]));
    } else {
        rb_warn("The format is not supported.");
    }
    return ary;
}

static VALUE
gdkeventclient_send_client_message(int argc, VALUE *argv, VALUE self)
{
    VALUE xid, display;
    rb_scan_args(argc, argv, "11", &xid, &display);
    if (NIL_P(display)){
        return CBOOL2RVAL(gdk_event_send_client_message(
                              get_gdkevent(self), RVAL2GDKNATIVEWINDOW(xid)));
    } else {
        return CBOOL2RVAL(gdk_event_send_client_message_for_display(
                              GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                              get_gdkevent(self),
                              RVAL2GDKNATIVEWINDOW(xid)));
    }
}

static VALUE
gdkeventclient_send_clientmessage_toall(VALUE self)
{
    gdk_event_send_clientmessage_toall(get_gdkevent(self));
    return self;
}

static GdkFilterReturn
filter_func(GdkXEvent *xevent, GdkEvent *event, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2, LONG2NUM((glong)xevent), make_gdkevent(event));
    return RVAL2GENUM(ret, GDK_TYPE_FILTER_RETURN);
}

static VALUE
gdkevent_s_add_client_message_filter(VALUE self, VALUE message_type)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gdk_add_client_message_filter(RVAL2ATOM(message_type),
                                  (GdkFilterFunc)filter_func, (gpointer)func);
    return self;
}

/* GdkEventNoExpose */
GDKEVENT_INIT(noexpose, GDK_NO_EXPOSE);

/* GdkEventWindowState */
ATTR_FLAGS(window_state, changed_mask, GDK_TYPE_WINDOW_STATE);
ATTR_FLAGS(window_state, new_window_state, GDK_TYPE_WINDOW_STATE);
GDKEVENT_INIT(window_state, GDK_WINDOW_STATE);

/* GdkEventSetting */
ATTR_ENUM(setting, action, GDK_TYPE_SETTING_ACTION);
ATTR_STR(setting, name);
GDKEVENT_INIT(setting, GDK_SETTING);

/* GdkEventOwnerChange */
ATTR_GDKNATIVEWINDOW(owner_change, owner);
ATTR_ENUM(owner_change, reason, GDK_TYPE_OWNER_CHANGE);
ATTR_ATOM(owner_change, selection);
ATTR_UINT(owner_change, time);
ATTR_UINT(owner_change, selection_time);
GDKEVENT_INIT(owner_change, GDK_OWNER_CHANGE);

/* GdkEventGrabBroken */
ATTR_BOOL(grab_broken, keyboard);
ATTR_BOOL(grab_broken, implicit);
ATTR_GOBJ(grab_broken, grab_window);
GDKEVENT_INIT(grab_broken, GDK_GRAB_BROKEN);

/* MISC */
static VALUE
gdkevent_g2r(const GValue *values)
{
    return make_gdkevent(g_value_get_boxed(&values[0]));
}

void
Init_gtk_gdk_event(VALUE mGdk)
{
    /* GdkEvent */
    rb_cGdkEvent = G_DEF_CLASS(GDK_TYPE_EVENT, "Event", mGdk);
    rb_define_method(rb_cGdkEvent, "initialize", gdkevent_initialize, 1);
    rb_define_method(rb_cGdkEvent, "event_type", gdkevent_type, 0);

    rb_define_singleton_method(rb_cGdkEvent, "events_pending?", gdkevent_s_events_pending, 0);
    rb_define_singleton_method(rb_cGdkEvent, "peek", gdkevent_s_peek, 0);
    rb_define_singleton_method(rb_cGdkEvent, "get", gdkevent_s_get, 0);
    rb_define_method(rb_cGdkEvent, "put", gdkevent_put, 0);
    rb_define_method(rb_cGdkEvent, "get_axis", gdkevent_get_axis, 1);
    rb_define_method(rb_cGdkEvent, "coords", gdkevent_get_coords, 0);
    rb_define_method(rb_cGdkEvent, "root_coords", gdkevent_get_root_coords, 0);

    rb_define_singleton_method(rb_cGdkEvent, "handler_set", gdkevent_s_handler_set, 0);
    rb_define_singleton_method(rb_cGdkEvent, "show_events?", gdkevent_s_get_show_events, 0);
    rb_define_singleton_method(rb_cGdkEvent, "set_show_events", gdkevent_s_set_show_events, 1);
    rb_define_singleton_method(rb_cGdkEvent, "setting_get", gdkevent_s_setting_get, -1);
    rb_define_singleton_method(rb_cGdkEvent, "add_client_message_filter", gdkevent_s_add_client_message_filter, 1);
    rb_define_method(rb_cGdkEvent, "screen", gdkevent_screen, 0);
    rb_define_method(rb_cGdkEvent, "set_screen", gdkevent_set_screen, 1);

    /* GdkEventAny fields */
    DEFINE_ACCESSOR(rb_cGdkEvent, any, window);
    rb_define_method(rb_cGdkEvent, "send_event?", gdkeventany_send_event, 0);
    rb_define_method(rb_cGdkEvent, "set_send_event", gdkeventany_set_send_event, 1);

    G_DEF_SETTERS(rb_cGdkEvent);

    /*
     * GdkEvent's Constants
     */
    rb_define_const(rb_cGdkEvent, "CURRENT_TIME", INT2FIX(GDK_CURRENT_TIME));
    rb_define_const(rb_cGdkEvent, "PRIORITY_EVENTS", INT2FIX(GDK_PRIORITY_EVENTS));
    rb_define_const(rb_cGdkEvent, "PRIORITY_REDRAW", INT2FIX(GDK_PRIORITY_REDRAW));

    /* GdkEventType */
    /* XXX */
    G_RENAME_CONSTANT("2BUTTON_PRESS","BUTTON2_PRESS");
    G_RENAME_CONSTANT("3BUTTON_PRESS","BUTTON3_PRESS");
    G_DEF_CLASS(GDK_TYPE_EVENT_TYPE, "Type", rb_cGdkEvent);
    G_RENAME_CONSTANT("2BUTTON_PRESS","BUTTON2_PRESS");
    G_RENAME_CONSTANT("3BUTTON_PRESS","BUTTON3_PRESS");
    G_DEF_CONSTANTS(rb_cGdkEvent, GDK_TYPE_EVENT_TYPE, "GDK_");

    /* GdkEventMask */
    G_DEF_CLASS(GDK_TYPE_EVENT_MASK, "Mask", rb_cGdkEvent);
    G_DEF_CONSTANTS(rb_cGdkEvent, GDK_TYPE_EVENT_MASK, "GDK_");

    /* GdkEventAny */
    rb_cGdkEventAny =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_ANY, "EventAny",
                                mGdk, rb_cGdkEvent);
    G_DEF_SETTERS(rb_cGdkEventAny);

    /* GdkEventExpose */
    rb_cGdkEventExpose =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_EXPOSE, "EventExpose",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventExpose, expose, area);
    DEFINE_ACCESSOR(rb_cGdkEventExpose, expose, region);
    DEFINE_ACCESSOR(rb_cGdkEventExpose, expose, count);
    DEFINE_INIT(rb_cGdkEventExpose, expose);
    rb_define_singleton_method(rb_cGdkEventExpose, "get_graphics_expose",
                               gdkevent_s_get_graphics_expose, 1);
    G_DEF_SETTERS(rb_cGdkEventExpose);

    /* GdkEventNoExpose */
    rb_cGdkEventNoExpose =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_NO_EXPOSE, "EventNoExpose",
                                mGdk, rb_cGdkEvent);
    DEFINE_INIT(rb_cGdkEventNoExpose, noexpose);

    /* GdkEventVisibility */
    rb_cGdkEventVisibility =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_VISIBILITY, "EventVisibility",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventVisibility, visibility, state);
    DEFINE_INIT(rb_cGdkEventVisibility, visibility);
    G_DEF_SETTERS(rb_cGdkEventVisibility);

    /* GdkVisibilityState */
    G_DEF_CLASS(GDK_TYPE_VISIBILITY_STATE, "State", rb_cGdkEventVisibility);
    G_DEF_CONSTANTS(rb_cGdkEventVisibility,
                    GDK_TYPE_VISIBILITY_STATE, "GDK_VISIBILITY_");


    /* GdkEventMotion */
    rb_cGdkEventMotion =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_MOTION, "EventMotion",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, time);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, x);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, y);
    rb_define_method(rb_cGdkEventMotion, "axes", gdkeventmotion_axes, 0);
    rb_define_method(rb_cGdkEventMotion, "set_axes", gdkeventmotion_set_axes, 1);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, state);
    rb_define_method(rb_cGdkEventMotion, "hint?", gdkeventmotion_is_hint, 0);
    rb_define_method(rb_cGdkEventMotion, "set_hint", gdkeventmotion_set_is_hint, 1);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, device);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, x_root);
    DEFINE_ACCESSOR(rb_cGdkEventMotion, motion, y_root);
    DEFINE_INIT(rb_cGdkEventMotion, motion);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(rb_cGdkEventMotion, "request", gdkeventmotion_request_motions, 0);
#endif
    G_DEF_SETTERS(rb_cGdkEventMotion);

    /* GdkEventButton */
    rb_cGdkEventButton =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_BUTTON, "EventButton",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, time);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, x);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, y);
    rb_define_method(rb_cGdkEventButton, "axes", gdkeventbutton_axes, 0);
    rb_define_method(rb_cGdkEventButton, "set_axes", gdkeventbutton_set_axes, 2);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, state);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, button);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, device);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, x_root);
    DEFINE_ACCESSOR(rb_cGdkEventButton, button, y_root);
    G_DEF_SETTERS(rb_cGdkEventButton);

    /* GdkEventScroll */
    rb_cGdkEventScroll =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_SCROLL, "EventScroll",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, time);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, x);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, y);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, state);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, direction);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, device);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, x_root);
    DEFINE_ACCESSOR(rb_cGdkEventScroll, scroll, y_root);
    DEFINE_INIT(rb_cGdkEventScroll, scroll);
    G_DEF_SETTERS(rb_cGdkEventScroll);

    /* GdkScrollDirection */
    G_DEF_CLASS(GDK_TYPE_SCROLL_DIRECTION, "Direction", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventScroll,
                    GDK_TYPE_SCROLL_DIRECTION, "GDK_SCROLL_");

    /* GdkEventKey */
    rb_cGdkEventKey =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_KEY, "EventKey",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventKey, key, time);
    DEFINE_ACCESSOR(rb_cGdkEventKey, key, state);
    DEFINE_ACCESSOR(rb_cGdkEventKey, key, keyval);
    DEFINE_ACCESSOR(rb_cGdkEventKey, key, hardware_keycode);
    DEFINE_ACCESSOR(rb_cGdkEventKey, key, group);
    G_DEF_SETTERS(rb_cGdkEventKey);

    /* GdkEventCrossing */
    rb_cGdkEventCrossing =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_CROSSING, "EventCrossing",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, subwindow);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, time);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, x);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, y);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, x_root);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, y_root);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, mode);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, detail);
    rb_define_method(rb_cGdkEventCrossing, "focus?", gdkeventcrossing_focus, 0);
    rb_define_method(rb_cGdkEventCrossing, "set_focus", gdkeventcrossing_set_focus, 1);
    DEFINE_ACCESSOR(rb_cGdkEventCrossing, crossing, state);
    G_DEF_SETTERS(rb_cGdkEventCrossing);

    /* GdkCrossingMode */
    G_DEF_CLASS(GDK_TYPE_CROSSING_MODE, "Mode", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventCrossing,
                    GDK_TYPE_CROSSING_MODE, "GDK_CROSSING_");

    /* GdkNotifyType */
    G_DEF_CLASS(GDK_TYPE_NOTIFY_TYPE, "NotifyType", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventScroll, GDK_TYPE_NOTIFY_TYPE, "GDK_");

    /* GdkEventFocus */
    rb_cGdkEventFocus =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_FOCUS, "EventFocus",
                                mGdk, rb_cGdkEvent);
    rb_define_method(rb_cGdkEventFocus, "in?", gdkeventfocus_change_in, 0);
    rb_define_method(rb_cGdkEventFocus, "set_in",
                     gdkeventfocus_change_set_in, 1);
    DEFINE_INIT(rb_cGdkEventFocus, focus_change);
    G_DEF_SETTERS(rb_cGdkEventFocus);

    /* GdkEventConfigure */
    rb_cGdkEventConfigure =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_CONFIGURE, "EventConfigure",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventConfigure, configure, x);
    DEFINE_ACCESSOR(rb_cGdkEventConfigure, configure, y);
    DEFINE_ACCESSOR(rb_cGdkEventConfigure, configure, width);
    DEFINE_ACCESSOR(rb_cGdkEventConfigure, configure, height);
    DEFINE_INIT(rb_cGdkEventConfigure, configure);
    G_DEF_SETTERS(rb_cGdkEventConfigure);

    /* GdkEventProperty */
    rb_cGdkEventProperty =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_PROPERTY, "EventProperty",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventProperty, property, atom);
    DEFINE_ACCESSOR(rb_cGdkEventProperty, property, time);
    DEFINE_ACCESSOR(rb_cGdkEventProperty, property, state);
    DEFINE_INIT(rb_cGdkEventProperty, property);
    G_DEF_SETTERS(rb_cGdkEventProperty);

    /* GdkPropertyState */
    G_DEF_CLASS(GDK_TYPE_PROPERTY_STATE, "State", rb_cGdkEventProperty);
    G_DEF_CONSTANTS(rb_cGdkEventProperty, GDK_TYPE_PROPERTY_STATE, "GDK_");

    /* GdkEventSelection */
    rb_cGdkEventSelection =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_SELECTION, "EventSelection",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventSelection, selection, selection);
    DEFINE_ACCESSOR(rb_cGdkEventSelection, selection, target);
    DEFINE_ACCESSOR(rb_cGdkEventSelection, selection, property);
    DEFINE_ACCESSOR(rb_cGdkEventSelection, selection, requestor);
    DEFINE_ACCESSOR(rb_cGdkEventSelection, selection, time);
    G_DEF_SETTERS(rb_cGdkEventSelection);

    /* GdkEventOwnerChange */
    rb_cGdkEventOwnerChange =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_OWNER_CHANGE, "EventOwnerChange",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventOwnerChange, owner_change, owner);
    DEFINE_ACCESSOR(rb_cGdkEventOwnerChange, owner_change, reason);
    DEFINE_ACCESSOR(rb_cGdkEventOwnerChange, owner_change, selection);
    DEFINE_ACCESSOR(rb_cGdkEventOwnerChange, owner_change, time);
    DEFINE_ACCESSOR(rb_cGdkEventOwnerChange, owner_change, selection_time);
    G_DEF_SETTERS(rb_cGdkEventOwnerChange);
    DEFINE_INIT(rb_cGdkEventOwnerChange, owner_change);

    /* GdkOwnerChange */
    G_DEF_CLASS(GDK_TYPE_OWNER_CHANGE, "OwnerChange", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventScroll,
                    GDK_TYPE_OWNER_CHANGE, "GDK_OWNER_CHANGE_");

    /* GdkEventProximity */
    rb_cGdkEventProximity =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_PROXIMITY, "EventProximity",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventProximity, proximity, time);
    DEFINE_ACCESSOR(rb_cGdkEventProximity, proximity, device);
    G_DEF_SETTERS(rb_cGdkEventProximity);

    /* GdkEventClient */
    rb_cGdkEventClient =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_CLIENT, "EventClient",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventClient, client, message_type);
    rb_define_method(rb_cGdkEventClient,
                     "data_format", gdkeventclient_data_format, 0);
    rb_define_method(rb_cGdkEventClient, "data", gdkeventclient_data, 0);
    rb_define_method(rb_cGdkEventClient, "send_client_message",
                     gdkeventclient_send_client_message, -1);
    rb_define_method(rb_cGdkEventClient, "send_clientmessage_toall",
                     gdkeventclient_send_clientmessage_toall, 0);
    DEFINE_INIT(rb_cGdkEventClient, client);
    G_DEF_SETTERS(rb_cGdkEventClient);

    /* GdkEventDND */
    rb_cGdkEventDND =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_DND, "EventDND",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventDND, dnd, context);
    DEFINE_ACCESSOR(rb_cGdkEventDND, dnd, time);
    DEFINE_ACCESSOR(rb_cGdkEventDND, dnd, x_root);
    DEFINE_ACCESSOR(rb_cGdkEventDND, dnd, y_root);
    G_DEF_SETTERS(rb_cGdkEventDND);

    /* GdkEventWindowState */
    rb_cGdkEventWindowState =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_WINDOW_STATE, "EventWindowState",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventWindowState, window_state, changed_mask);
    DEFINE_ACCESSOR(rb_cGdkEventWindowState, window_state, new_window_state);
    DEFINE_INIT(rb_cGdkEventWindowState, window_state);
    G_DEF_SETTERS(rb_cGdkEventWindowState);

    /* GdkWindowState */
    G_DEF_CLASS(GDK_TYPE_WINDOW_STATE, "WindowState", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventWindowState,
                    GDK_TYPE_WINDOW_STATE, "GDK_WINDOW_STATE_");

    /* GdkEventSetting */
    rb_cGdkEventSetting =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_SETTING, "EventSetting",
                                mGdk, rb_cGdkEvent);
    DEFINE_ACCESSOR(rb_cGdkEventSetting, setting, action);
    DEFINE_ACCESSOR(rb_cGdkEventSetting, setting, name);
    DEFINE_INIT(rb_cGdkEventSetting, setting);
    G_DEF_SETTERS(rb_cGdkEventSetting);

    /* GdkSettingAction */
    G_DEF_CLASS(GDK_TYPE_SETTING_ACTION, "Action", rb_cGdkEventScroll);
    G_DEF_CONSTANTS(rb_cGdkEventSetting,
                    GDK_TYPE_SETTING_ACTION, "GDK_SETTING_");

    /* GdkEventGrabBroken */
    rb_cGdkEventGrabBroken =
        G_DEF_CLASS_WITH_PARENT(GDK_TYPE_EVENT_GRAB_BROKEN, "EventGrabBroken",
                                mGdk, rb_cGdkEvent);
    rb_define_method(rb_cGdkEventGrabBroken,
                     "keyboard?", gdkeventgrab_broken_keyboard, 0);
    rb_define_method(rb_cGdkEventGrabBroken,
                     "set_keyboard", gdkeventgrab_broken_set_keyboard, 1);
    rb_define_method(rb_cGdkEventGrabBroken,
                     "implicit?", gdkeventgrab_broken_implicit, 0);
    rb_define_method(rb_cGdkEventGrabBroken,
                     "set_implicit", gdkeventgrab_broken_set_implicit, 1);
    DEFINE_ACCESSOR(rb_cGdkEventGrabBroken, grab_broken, grab_window);
    DEFINE_INIT(rb_cGdkEventGrabBroken, grab_broken);
    G_DEF_SETTERS(rb_cGdkEventGrabBroken);

    rbgobj_register_g2r_func(GDK_TYPE_EVENT, &gdkevent_g2r);
}
