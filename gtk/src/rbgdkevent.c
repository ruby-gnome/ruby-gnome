/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkevent.c -

  $Author: mutoh $
  $Date: 2005/01/31 16:48:17 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)
static VALUE gdkevents[35];
#else
static VALUE gdkevents[34];
#endif

/***********************************************/

VALUE
make_gdkevent(ev)
    GdkEvent *ev;
{
    VALUE obj;
    if (ev == NULL) return Qnil;

    obj = BOXED2RVAL(ev, GDK_TYPE_EVENT);
    RBASIC(obj)->klass = gdkevents[ev->type]; /* hack */
    return obj;
}

GdkEvent*
get_gdkevent(event)
    VALUE event;
{
    return NIL_P(event) ? NULL : RVAL2BOXED(event, GDK_TYPE_EVENT);
}

/***********************************************/
#define ATTR_STR(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return CSTR2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RVAL2CSTR(val);\
    return self;\
}

#define ATTR_INT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return INT2NUM(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = NUM2INT(val);\
    return self;\
}

#define ATTR_UINT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return UINT2NUM(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = NUM2UINT(val);\
    return self;\
}

#define ATTR_FLOAT(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return rb_float_new(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = NUM2DBL(val);\
    return self;\
}

#define ATTR_GOBJ(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return GOBJ2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RVAL2GOBJ(val);\
    return self;\
}

#define ATTR_BOOL(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return CBOOL2RVAL(get_gdkevent(self)->type.name);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RTEST(val);\
    return self;\
}

#define ATTR_ATOM(type, name)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    GdkAtom atom = get_gdkevent(self)->type.name;\
    return atom ? BOXED2RVAL(atom, GDK_TYPE_ATOM) : Qnil;\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RVAL2ATOM(val);\
    return self;\
}

#define ATTR_FLAGS(type, name, gtype)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return GFLAGS2RVAL(get_gdkevent(self)->type.name, gtype);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RVAL2GFLAGS(val, gtype);\
    return self;\
}

#define ATTR_ENUM(type, name, gtype)\
static VALUE \
gdkevent ## type ## _ ## name (self)\
    VALUE self;\
{\
    return GENUM2RVAL(get_gdkevent(self)->type.name, gtype);\
}\
static VALUE \
gdkevent ## type ## _set_ ## name (self, val)\
    VALUE self, val;\
{\
    get_gdkevent(self)->type.name = RVAL2GENUM(val, gtype);\
    return self;\
}

#define ATTR_AXES(type, gdkklass) \
static VALUE \
gdkevent ##type ## _axes(self)\
    VALUE self;\
{\
    gdkklass type = get_gdkevent(self)->type;\
    return type.axes ? rb_ary_new3(2, \
                       rb_float_new(type.axes[0]),\
                       rb_float_new(type.axes[1])) : Qnil;\
} \
static VALUE \
gdkevent ## type ## _set_axes(self, x, y)\
    VALUE self, x, y;\
{\
    gdkklass val = get_gdkevent(self)->type;\
    val.axes[0] = NUM2DBL(x);\
    val.axes[1] = NUM2DBL(y);\
    return self;\
}

#define DEFINE_ACCESSOR(event, type, name) \
    rb_define_method(event, G_STRINGIFY(name), gdkevent ## type ## _## name, 0);\
    rb_define_method(event, G_STRINGIFY(set_ ## name), gdkevent ## type ## _set_## name, 1);

/***********************************************/

/* GdkEvent Singleton Methods */
static VALUE
gdkevent_s_events_pending(self)
    VALUE self;
{
    return gdk_events_pending() ? Qtrue : Qfalse;
}

static VALUE
gdkevent_s_peek(self)
    VALUE self;
{
    return make_gdkevent(gdk_event_peek());
}

static VALUE
gdkevent_s_get(self)
    VALUE self;
{
    return make_gdkevent(gdk_event_get());
}

static VALUE
gdkevent_s_get_graphics_expose(self, window)
    VALUE self, window;
{
    return make_gdkevent(gdk_event_get_graphics_expose(GDK_WINDOW(RVAL2GOBJ(window))));
}

/* GdkEvent */
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkevent_initialize(self, type)
    VALUE self, type;
{
    GdkEventType gtype = RVAL2GENUM(type, GDK_TYPE_EVENT_TYPE);
    if (RBASIC(self)->klass != gdkevents[gtype])
        rb_raise(rb_eArgError, "Wrong event type for this class.");

    G_INITIALIZE(self, gdk_event_new(gtype));
    return Qnil;
}
#endif

static VALUE
gdkevent_type(self)
    VALUE self;
{
    return GENUM2RVAL(get_gdkevent(self)->type, GDK_TYPE_EVENT_TYPE);
}

static VALUE
gdkevent_put(self)
    VALUE self;
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
gdkevent_get_axis(self, axis_use)
    VALUE self, axis_use;
{
    gdouble value;
    gboolean ret = gdk_event_get_axis(get_gdkevent(self), 
                                      RVAL2GENUM(axis_use, GDK_TYPE_AXIS_USE), &value);
    return ret ? rb_float_new(value) : Qnil;
}

static VALUE
gdkevent_get_coords(self)
    VALUE self;
{
    gdouble x_win, y_win;
    gboolean ret = gdk_event_get_coords(get_gdkevent(self), &x_win, &y_win);

    return ret ? rb_ary_new3(2, rb_float_new(x_win), rb_float_new(y_win)) : Qnil;
}

static VALUE
gdkevent_get_root_coords(self)
    VALUE self;
{
    gdouble x_root, y_root;
    gboolean ret = gdk_event_get_root_coords(get_gdkevent(self), &x_root, &y_root);

    return ret ? rb_ary_new3(2, rb_float_new(x_root), rb_float_new(y_root)) : Qnil;
}

static void
handler_func(event, func)
    GdkEvent* event;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 1, make_gdkevent(event));
}

static VALUE
gdkevent_s_handler_set(self)
    VALUE self;
{
    volatile VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);

    gdk_event_handler_set((GdkEventFunc)handler_func, (gpointer)func, NULL);
    return self;
}

static VALUE
gdkevent_s_get_show_events(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_get_show_events());
}

static VALUE
gdkevent_s_set_show_events(self, show_events)
    VALUE self, show_events;
{
    gdk_set_show_events(RTEST(show_events));
    return self;
}
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkevent_set_screen(self, screen)
    VALUE self, screen;
{
    gdk_event_set_screen(get_gdkevent(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
gdkevent_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_event_get_screen(get_gdkevent(self)));
}
#endif

/*
  type: String, Integer, Gdk::Color.
 */
static VALUE
gdkevent_s_setting_get(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name, type;
    GType gtype;
    GValue val = {0,};
    gboolean ret;
    VALUE value;

    rb_scan_args(argc, argv, "11", &name, &type);
    if NIL_P(type) 
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

/* GdkEventExpose */
static VALUE
gdkeventexpose_area(self)
    VALUE self;
{
    return BOXED2RVAL(&get_gdkevent(self)->expose.area, GDK_TYPE_RECTANGLE);
}
 
static VALUE
gdkeventexpose_set_area(self, rect)
    VALUE self, rect;
{
    GdkRectangle* grect = (GdkRectangle*)RVAL2BOXED(rect, GDK_TYPE_RECTANGLE);
    GdkEventExpose event = get_gdkevent(self)->expose;
    event.area.x = grect->x;
    event.area.y = grect->y;
    event.area.width = grect->width;
    event.area.height = grect->height;
    return self;
}

static VALUE
gdkeventexpose_region(self)
    VALUE self;
{
    return BOXED2RVAL(get_gdkevent(self)->expose.region, GDK_TYPE_REGION);
}
static VALUE
gdkeventexpose_set_region(self, region)
    VALUE self, region;
{
    get_gdkevent(self)->expose.region = RVAL2BOXED(region, GDK_TYPE_REGION);
    return self;
}

ATTR_INT(expose, count);

/* GdkEventVisibility */
ATTR_ENUM(visibility, state, GDK_TYPE_VISIBILITY_STATE);

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

/* GdkEventConfigure */
ATTR_INT(configure, x);
ATTR_INT(configure, y);
ATTR_INT(configure, width);
ATTR_INT(configure, height);

/* GdkEventProperty */
ATTR_ATOM(property, atom);
ATTR_UINT(property, time);
ATTR_ENUM(property, state, GDK_TYPE_PROPERTY_STATE);

/* GdkEventSelection */
ATTR_ATOM(selection, selection);
ATTR_ATOM(selection, target);
ATTR_ATOM(selection, property);
ATTR_UINT(selection, requestor);
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

static VALUE
gdkeventclient_data_format(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->client.data_format);
}

static VALUE
gdkeventclient_data(self)
    VALUE self;
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
gdkeventclient_send_client_message(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE xid, display;
    rb_scan_args(argc, argv, "11", &xid, &display);
    if (NIL_P(display)){
        return CBOOL2RVAL(gdk_event_send_client_message(
                              get_gdkevent(self), NUM2INT(xid)));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        return CBOOL2RVAL(gdk_event_send_client_message_for_display(
                              GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                              get_gdkevent(self),
                              NUM2UINT(xid)));
#else
        rb_warn("this arguments number has been supported since 2.2");
        return Qfalse;
#endif
    }
}

static VALUE
gdkeventclient_send_clientmessage_toall(self)
    VALUE self;
{
    gdk_event_send_clientmessage_toall(get_gdkevent(self));
    return self;
}

static GdkFilterReturn
filter_func(xevent, event, func)
    GdkXEvent* xevent;
    GdkEvent* event;
    gpointer func;
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2, UINT2NUM((guint)xevent), make_gdkevent(event));
    return RVAL2GENUM(ret, GDK_TYPE_FILTER_RETURN);
}

static VALUE
gdkevent_s_add_client_message_filter(self, message_type)
    VALUE self, message_type;
{
    volatile VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gdk_add_client_message_filter(RVAL2ATOM(message_type),
                                  (GdkFilterFunc)filter_func, (gpointer)func);
    return self;
}

/* GdkEventWindowState */
ATTR_FLAGS(window_state, changed_mask, GDK_TYPE_WINDOW_STATE);
ATTR_FLAGS(window_state, new_window_state, GDK_TYPE_WINDOW_STATE);

/* GdkEventSetting */
ATTR_ENUM(setting, action, GDK_TYPE_SETTING_ACTION);
ATTR_STR(setting, name);

/* GdkEventOwnerChange */
#if GTK_CHECK_VERSION(2,6,0)
ATTR_UINT(owner_change, owner);
ATTR_ENUM(owner_change, reason, GDK_TYPE_OWNER_CHANGE);
ATTR_ATOM(owner_change, selection);
ATTR_UINT(owner_change, time);
ATTR_UINT(owner_change, selection_time);
#endif

/* MISC */
static VALUE 
gdkevent_g2r(const GValue *values)
{
    return make_gdkevent(g_value_get_boxed(&values[0]));
}

void
Init_gtk_gdk_event()
{
    VALUE ev;
    VALUE gdkEvent;
    VALUE gdkEventAny;

    gdkEvent = G_DEF_CLASS(GDK_TYPE_EVENT, "Event", mGdk);
    gdkEventAny = rb_define_class_under(mGdk, "EventAny", gdkEvent);

    gdkevents[GDK_DELETE]        = gdkEventAny;
    gdkevents[GDK_DESTROY]       = gdkEventAny;
    gdkevents[GDK_EXPOSE]        = rb_define_class_under(mGdk, "EventExpose", gdkEventAny);
    gdkevents[GDK_MOTION_NOTIFY] = rb_define_class_under(mGdk, "EventMotion", gdkEventAny);
    gdkevents[GDK_BUTTON_PRESS]  = rb_define_class_under(mGdk, "EventButton", gdkEventAny);
    gdkevents[GDK_2BUTTON_PRESS] = gdkevents[GDK_BUTTON_PRESS];
    gdkevents[GDK_3BUTTON_PRESS] = gdkevents[GDK_BUTTON_PRESS];
    gdkevents[GDK_BUTTON_RELEASE]= gdkevents[GDK_BUTTON_PRESS];
    gdkevents[GDK_KEY_PRESS]     = rb_define_class_under(mGdk, "EventKey", gdkEventAny);
    gdkevents[GDK_KEY_RELEASE]   = gdkevents[GDK_KEY_PRESS];
    gdkevents[GDK_ENTER_NOTIFY]  = rb_define_class_under(mGdk, "EventCrossing", gdkEventAny);
    gdkevents[GDK_LEAVE_NOTIFY]  = gdkevents[GDK_ENTER_NOTIFY];
    gdkevents[GDK_FOCUS_CHANGE]  = rb_define_class_under(mGdk, "EventFocus", gdkEventAny);
    gdkevents[GDK_CONFIGURE]     = rb_define_class_under(mGdk, "EventConfigure", gdkEventAny);
    gdkevents[GDK_MAP]           = gdkEventAny;
    gdkevents[GDK_UNMAP]         = gdkEventAny;
    gdkevents[GDK_PROPERTY_NOTIFY]= rb_define_class_under(mGdk, "EventProperty", gdkEventAny);
    gdkevents[GDK_SELECTION_CLEAR]= rb_define_class_under(mGdk, "EventSelection", gdkEventAny);
    gdkevents[GDK_SELECTION_REQUEST]= gdkevents[GDK_SELECTION_CLEAR];
    gdkevents[GDK_SELECTION_NOTIFY] = gdkevents[GDK_SELECTION_CLEAR];
    gdkevents[GDK_PROXIMITY_IN]  = rb_define_class_under(mGdk, "EventProximity", gdkEventAny);
    gdkevents[GDK_PROXIMITY_OUT] = gdkevents[GDK_PROXIMITY_IN];
    gdkevents[GDK_DRAG_ENTER]    = rb_define_class_under(mGdk, "EventDND", gdkEventAny);
    gdkevents[GDK_DRAG_LEAVE]    = gdkevents[GDK_DRAG_ENTER];
    gdkevents[GDK_DRAG_MOTION]   = gdkevents[GDK_DRAG_ENTER];
    gdkevents[GDK_DRAG_STATUS]   = gdkevents[GDK_DRAG_ENTER];
    gdkevents[GDK_DROP_START]    = gdkevents[GDK_DRAG_ENTER];
    gdkevents[GDK_DROP_FINISHED] = gdkevents[GDK_DRAG_ENTER];
    gdkevents[GDK_CLIENT_EVENT]  = rb_define_class_under(mGdk, "EventClient", gdkEventAny);
    gdkevents[GDK_VISIBILITY_NOTIFY] = rb_define_class_under(mGdk, "EventVisibility", gdkEventAny);
    gdkevents[GDK_NO_EXPOSE]     = rb_define_class_under(mGdk, "EventNoExpose", gdkEventAny);
    gdkevents[GDK_SCROLL]        = rb_define_class_under(mGdk, "EventScroll", gdkEventAny);
    gdkevents[GDK_WINDOW_STATE]  = rb_define_class_under(mGdk, "EventWindowState", gdkEventAny);
    gdkevents[GDK_SETTING]       = rb_define_class_under(mGdk, "EventSetting", gdkEventAny);
#if GTK_CHECK_VERSION(2,6,0)
    gdkevents[GDK_OWNER_CHANGE]  = rb_define_class_under(mGdk, "EventOwnerChange", gdkEventAny);
#endif

    /* GdkEvent */
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkEvent, "initialize", gdkevent_initialize, 1);
#endif
    rb_define_method(gdkEvent, "event_type", gdkevent_type, 0);

    rb_define_singleton_method(gdkEvent, "events_pending?", gdkevent_s_events_pending, 0);
    rb_define_singleton_method(gdkEvent, "peek", gdkevent_s_peek, 0);
    rb_define_singleton_method(gdkEvent, "get", gdkevent_s_get, 0);
    rb_define_method(gdkEvent, "put", gdkevent_put, 0);
    rb_define_method(gdkEvent, "get_axis", gdkevent_get_axis, 1);
    rb_define_method(gdkEvent, "coords", gdkevent_get_coords, 0);
    rb_define_method(gdkEvent, "root_coords", gdkevent_get_root_coords, 0);

    rb_define_singleton_method(gdkEvent, "handler_set", gdkevent_s_handler_set, 0);
    rb_define_singleton_method(gdkEvent, "show_events?", gdkevent_s_get_show_events, 0);
    rb_define_singleton_method(gdkEvent, "set_show_events", gdkevent_s_set_show_events, 1);
    rb_define_singleton_method(gdkEvent, "setting_get", gdkevent_s_setting_get, -1);
    rb_define_singleton_method(gdkEvent, "add_client_message_filter", gdkevent_s_add_client_message_filter, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkEvent, "screen", gdkevent_screen, 0);
    rb_define_method(gdkEvent, "set_screen", gdkevent_set_screen, 1);
#endif
    G_DEF_SETTERS(gdkEvent);

    /* GdkEventAny */
    DEFINE_ACCESSOR(gdkEventAny, any, window);
    rb_define_method(gdkEventAny, "send_event?", gdkeventany_send_event, 0);
    rb_define_method(gdkEventAny, "set_send_event", gdkeventany_set_send_event, 1);
    G_DEF_SETTERS(gdkEventAny);

    /* GdkEventKey */
    ev = gdkevents[GDK_KEY_PRESS];
    DEFINE_ACCESSOR(ev, key, time);
    DEFINE_ACCESSOR(ev, key, state);
    DEFINE_ACCESSOR(ev, key, keyval);
    DEFINE_ACCESSOR(ev, key, hardware_keycode);
    DEFINE_ACCESSOR(ev, key, group);
    G_DEF_SETTERS(ev);

    /* GdkEventButton */
    ev = gdkevents[GDK_BUTTON_PRESS];
    DEFINE_ACCESSOR(ev, button, time);
    DEFINE_ACCESSOR(ev, button, x);
    DEFINE_ACCESSOR(ev, button, y);
    rb_define_method(ev, "axes", gdkeventbutton_axes, 0);
    rb_define_method(ev, "set_axes", gdkeventbutton_set_axes, 2);
    DEFINE_ACCESSOR(ev, button, state);
    DEFINE_ACCESSOR(ev, button, button);
    DEFINE_ACCESSOR(ev, button, device);
    DEFINE_ACCESSOR(ev, button, x_root);
    DEFINE_ACCESSOR(ev, button, y_root);
    G_DEF_SETTERS(ev);

    /* GdkEventScroll */
    ev = gdkevents[GDK_SCROLL];
    DEFINE_ACCESSOR(ev, scroll, time);
    DEFINE_ACCESSOR(ev, scroll, x);
    DEFINE_ACCESSOR(ev, scroll, y);
    DEFINE_ACCESSOR(ev, scroll, state);
    DEFINE_ACCESSOR(ev, scroll, direction);
    DEFINE_ACCESSOR(ev, scroll, device);
    DEFINE_ACCESSOR(ev, scroll, x_root);
    DEFINE_ACCESSOR(ev, scroll, y_root);
    G_DEF_SETTERS(ev);

    /* GdkScrollDirection */
    G_DEF_CLASS(GDK_TYPE_SCROLL_DIRECTION, "Direction", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_SCROLL_DIRECTION, "GDK_SCROLL_");

    /* GdkEventMotion */
    ev = gdkevents[GDK_MOTION_NOTIFY];
    DEFINE_ACCESSOR(ev, motion, time);
    DEFINE_ACCESSOR(ev, motion, x);
    DEFINE_ACCESSOR(ev, motion, y);
    rb_define_method(ev, "axes", gdkeventmotion_axes, 0);
    rb_define_method(ev, "set_axes", gdkeventmotion_set_axes, 1);
    DEFINE_ACCESSOR(ev, motion, state);
    rb_define_method(ev, "hint?", gdkeventmotion_is_hint, 0);
    rb_define_method(ev, "set_hint", gdkeventmotion_set_is_hint, 1);
    DEFINE_ACCESSOR(ev, motion, device);
    DEFINE_ACCESSOR(ev, motion, x_root);
    DEFINE_ACCESSOR(ev, motion, y_root);
    G_DEF_SETTERS(ev);

    /* GdkEventExpose */
    ev = gdkevents[GDK_EXPOSE];
    DEFINE_ACCESSOR(ev, expose, area);
    DEFINE_ACCESSOR(ev, expose, region);
    DEFINE_ACCESSOR(ev, expose, count);
    rb_define_singleton_method(ev, "get_graphics_expose", 
                               gdkevent_s_get_graphics_expose, 1);
    G_DEF_SETTERS(ev);

    /* GdkEventVisibility */
    ev = gdkevents[GDK_VISIBILITY_NOTIFY];
    DEFINE_ACCESSOR(ev, visibility, state);
    G_DEF_SETTERS(ev);

    /* GdkVisibilityState */
    G_DEF_CLASS(GDK_TYPE_VISIBILITY_STATE, "State", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_VISIBILITY_STATE, "GDK_VISIBILITY_");

    /* GdkEventCrossing */
    ev = gdkevents[GDK_ENTER_NOTIFY];
    DEFINE_ACCESSOR(ev, crossing, subwindow);
    DEFINE_ACCESSOR(ev, crossing, time);
    DEFINE_ACCESSOR(ev, crossing, x);
    DEFINE_ACCESSOR(ev, crossing, y);
    DEFINE_ACCESSOR(ev, crossing, x_root);
    DEFINE_ACCESSOR(ev, crossing, y_root);
    DEFINE_ACCESSOR(ev, crossing, mode);
    DEFINE_ACCESSOR(ev, crossing, detail);
    rb_define_method(ev, "focus?", gdkeventcrossing_focus, 0);
    rb_define_method(ev, "set_focus", gdkeventcrossing_set_focus, 1);
    DEFINE_ACCESSOR(ev, crossing, state);
    G_DEF_SETTERS(ev);

    /* GdkCrossingMode */
    G_DEF_CLASS(GDK_TYPE_CROSSING_MODE, "Mode", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_CROSSING_MODE, "GDK_CROSSING_");
    /* GdkNotifyType */
    G_DEF_CLASS(GDK_TYPE_NOTIFY_TYPE, "NotifyType", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_NOTIFY_TYPE, "GDK_");

    /* GdkEventFocus */
    ev = gdkevents[GDK_FOCUS_CHANGE];
    rb_define_method(ev, "in?", gdkeventfocus_change_in, 0);
    rb_define_method(ev, "set_in", gdkeventfocus_change_set_in, 1);
    G_DEF_SETTERS(ev);

    /* GdkEventConfigure */
    ev = gdkevents[GDK_CONFIGURE];
    DEFINE_ACCESSOR(ev, configure, x);
    DEFINE_ACCESSOR(ev, configure, y);
    DEFINE_ACCESSOR(ev, configure, width);
    DEFINE_ACCESSOR(ev, configure, height);
    G_DEF_SETTERS(ev);

    /* GdkEventProperty */
    ev = gdkevents[GDK_PROPERTY_NOTIFY];
    DEFINE_ACCESSOR(ev, property, atom);
    DEFINE_ACCESSOR(ev, property, time);
    DEFINE_ACCESSOR(ev, property, state);
    G_DEF_SETTERS(ev);

    /* GdkPropertyState */ 
    G_DEF_CLASS(GDK_TYPE_PROPERTY_STATE, "State", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_PROPERTY_STATE, "GDK_");

    /* GdkEventSelection */
    ev = gdkevents[GDK_SELECTION_CLEAR];
    DEFINE_ACCESSOR(ev, selection, selection);
    DEFINE_ACCESSOR(ev, selection, target);
    DEFINE_ACCESSOR(ev, selection, property);
    DEFINE_ACCESSOR(ev, selection, requestor);
    DEFINE_ACCESSOR(ev, selection, time);
    G_DEF_SETTERS(ev);

    /* GdkEventDND */
    ev = gdkevents[GDK_DRAG_ENTER];
    DEFINE_ACCESSOR(ev, dnd, context);
    DEFINE_ACCESSOR(ev, dnd, time);
    DEFINE_ACCESSOR(ev, dnd, x_root);
    DEFINE_ACCESSOR(ev, dnd, y_root);
    G_DEF_SETTERS(ev);

    /* GdkEventProximity */
    ev = gdkevents[GDK_PROXIMITY_IN];
    DEFINE_ACCESSOR(ev, proximity, time);
    DEFINE_ACCESSOR(ev, proximity, device);
    G_DEF_SETTERS(ev);

    /* GdkEventClient */
    ev = gdkevents[GDK_CLIENT_EVENT];
    DEFINE_ACCESSOR(ev, client, message_type);
    rb_define_method(ev, "data_format", gdkeventclient_data_format, 0);
    rb_define_method(ev, "data", gdkeventclient_data, 0);
    rb_define_method(ev, "send_client_message", 
                     gdkeventclient_send_client_message, -1);
    rb_define_method(ev, "send_clientmessage_toall", 
                     gdkeventclient_send_clientmessage_toall, 0);
    G_DEF_SETTERS(ev);

    /* GdkEventNoExpose */
    /* No methods */

    /* GdkEventWindowState */
    ev = gdkevents[GDK_WINDOW_STATE];
    DEFINE_ACCESSOR(ev, window_state, changed_mask);
    DEFINE_ACCESSOR(ev, window_state, new_window_state);
    G_DEF_SETTERS(ev);

    /* GdkWindowState */
    G_DEF_CLASS(GDK_TYPE_WINDOW_STATE, "WindowState", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_WINDOW_STATE, "GDK_WINDOW_STATE_");

    /* GdkEventSetting */
    ev = gdkevents[GDK_SETTING];
    DEFINE_ACCESSOR(ev, setting, action);
    DEFINE_ACCESSOR(ev, setting, name);
    G_DEF_SETTERS(ev);

    /* GdkSettingAction */
    G_DEF_CLASS(GDK_TYPE_SETTING_ACTION, "Action", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_SETTING_ACTION, "GDK_SETTING_");

#if GTK_CHECK_VERSION(2,6,0)
    /* GdkEventOwnerChange */
    ev = gdkevents[GDK_OWNER_CHANGE];
    DEFINE_ACCESSOR(ev, owner_change, owner);
    DEFINE_ACCESSOR(ev, owner_change, reason);
    DEFINE_ACCESSOR(ev, owner_change, selection);
    DEFINE_ACCESSOR(ev, owner_change, time);
    DEFINE_ACCESSOR(ev, owner_change, selection_time);
    G_DEF_SETTERS(ev);

    /* GdkOwnerChange */
    G_DEF_CLASS(GDK_TYPE_OWNER_CHANGE, "OwnerChange", ev);
    G_DEF_CONSTANTS(ev, GDK_TYPE_OWNER_CHANGE, "GDK_OWNER_CHANGE_");
#endif

    rbgobj_register_g2r_func(GDK_TYPE_EVENT, &gdkevent_g2r);

    /*
     * GdkEvent's Constants
     */
    rb_define_const(gdkEvent, "CURRENT_TIME", INT2FIX(GDK_CURRENT_TIME));
    rb_define_const(gdkEvent, "PRIORITY_EVENTS", INT2FIX(GDK_PRIORITY_EVENTS));
    rb_define_const(gdkEvent, "PRIORITY_REDRAW", INT2FIX(GDK_PRIORITY_REDRAW));

    /* GdkEventType */
    /* XXX */
    G_RENAME_CONSTANT("2BUTTON_PRESS","BUTTON2_PRESS");
    G_RENAME_CONSTANT("3BUTTON_PRESS","BUTTON3_PRESS");
    G_DEF_CLASS(GDK_TYPE_EVENT_TYPE, "Type", gdkEvent);
    G_RENAME_CONSTANT("2BUTTON_PRESS","BUTTON2_PRESS");
    G_RENAME_CONSTANT("3BUTTON_PRESS","BUTTON3_PRESS");
    G_DEF_CONSTANTS(gdkEvent, GDK_TYPE_EVENT_TYPE, "GDK_");

    /* GdkEventMask */
    G_DEF_CLASS(GDK_TYPE_EVENT_MASK, "Mask", gdkEvent);
    G_DEF_CONSTANTS(gdkEvent, GDK_TYPE_EVENT_MASK, "GDK_");

}
