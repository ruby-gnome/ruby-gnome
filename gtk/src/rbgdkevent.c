/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkevent.c -

  $Author: sakai $
  $Date: 2002/07/27 14:14:12 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE gdkEventAny;
VALUE gdkEventExpose;
VALUE gdkEventNoExpose;
VALUE gdkEventVisibility;
VALUE gdkEventMotion;
VALUE gdkEventButton;
VALUE gdkEventKey;
VALUE gdkEventCrossing;
VALUE gdkEventFocus;
VALUE gdkEventConfigure;
VALUE gdkEventProperty;
VALUE gdkEventSelection;
VALUE gdkEventDND;
VALUE gdkEventProximity;
VALUE gdkEventClient;
VALUE gdkEventOther;
VALUE gdkDragContext;

/* GdkEvent Singleton Methods */
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
static VALUE
gdkevent_type(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->type);
}

static VALUE
gdkevent_put(self)
    VALUE self;
{
    gdk_event_put(get_gdkevent(self));
    return Qnil;
}

static VALUE
gdkevent_copy(self)
    VALUE self;
{
    return make_gdkevent(gdk_event_copy(get_gdkevent(self)));
}

static VALUE
gdkevent_get_time(self)
    VALUE self;
{
    return INT2NUM(gdk_event_get_time(get_gdkevent(self)));
}

/* GdkEventAny */
static VALUE
gdkeventany_window(self)
    VALUE self;
{
    return GOBJ2RVAL(get_gdkevent(self)->any.window);
}

static VALUE
gdkeventany_send_event(self)
    VALUE self;
{
    return get_gdkevent(self)->any.send_event ? Qtrue : Qfalse;
}

/* GdkEventExpose */
static VALUE
gdkeventexpose_area(self)
    VALUE self;
{
    return make_gdkrectangle(&get_gdkevent(self)->expose.area);
}

static VALUE
gdkeventexpose_count(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->expose.count);
}

/* GdkEventVisibility */
static VALUE
gdkeventvisibility_state(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->visibility.state);
}

/* GdkEventMotion */
static VALUE
gdkeventmotion_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->motion.time);
}

static VALUE
gdkeventmotion_x(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->motion.x);
}

static VALUE
gdkeventmotion_y(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->motion.y);
}

static VALUE
gdkeventmotion_x_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->motion.x_root);
}

static VALUE
gdkeventmotion_y_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->motion.y_root);
}

static VALUE
gdkeventmotion_state(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->motion.state);
}

static VALUE
gdkeventmotion_is_hint(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->motion.is_hint);
}

/* GdkEventButton */
static VALUE
gdkeventbutton_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->button.time);
}

static VALUE
gdkeventbutton_x(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->button.x);
}

static VALUE
gdkeventbutton_y(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->button.y);
}

static VALUE
gdkeventbutton_x_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->button.x_root);
}

static VALUE
gdkeventbutton_y_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->button.y_root);
}

/* GdkEventKey */
static VALUE
gdkeventkey_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->key.time);
}

static VALUE
gdkeventkey_state(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->key.state);
}

static VALUE
gdkeventkey_keyval(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->key.keyval);
}

static VALUE
gdkeventkey_length(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->key.length);
}

static VALUE
gdkeventkey_string(self)
    VALUE self;
{
    return rb_str_new2(get_gdkevent(self)->key.string);
}

/* GdkEventCrossing */
static VALUE
gdkeventcrossing_subwindow(self)
    VALUE self;
{
    return GOBJ2RVAL(get_gdkevent(self)->crossing.subwindow);
}

static VALUE
gdkeventcrossing_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->crossing.time);
}

static VALUE
gdkeventcrossing_x(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->crossing.x);
}

static VALUE
gdkeventcrossing_y(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->crossing.y);
}

static VALUE
gdkeventcrossing_x_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->crossing.x_root);
}

static VALUE
gdkeventcrossing_y_root(self)
    VALUE self;
{
    return rb_float_new(get_gdkevent(self)->crossing.y_root);
}

static VALUE
gdkeventcrossing_mode(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->crossing.mode);
}

static VALUE
gdkeventcrossing_detail(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->crossing.detail);
}

static VALUE
gdkeventcrossing_focus(self)
    VALUE self;
{
    return get_gdkevent(self)->crossing.focus ? Qtrue : Qfalse;
}

static VALUE
gdkeventcrossing_state(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->crossing.state);
}

/* GdkEventFocus */
static VALUE
gdkeventfocus_in(self)
    VALUE self;
{
    return get_gdkevent(self)->focus_change.in ? Qtrue : Qfalse;
}

/* GdkEventConfigure */
static VALUE
gdkeventconfigure_x(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->configure.x);
}

static VALUE
gdkeventconfigure_y(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->configure.y);
}

static VALUE
gdkeventconfigure_width(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->configure.width);
}

static VALUE
gdkeventconfigure_height(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->configure.height);
}

/* GdkEventProperty */
static VALUE
gdkeventproperty_atom(self)
    VALUE self;
{
    return INT2NUM((int)(get_gdkevent(self)->property.atom));
}

static VALUE
gdkeventproperty_time(self)
    VALUE self;
{
    return INT2NUM((int)(get_gdkevent(self)->property.time));
}

static VALUE
gdkeventproperty_state(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->property.state);
}

/* GdkEventSelection */
static VALUE
gdkeventselection_selection(self)
    VALUE self;
{
    return make_gdkatom(get_gdkevent(self)->selection.selection);
}

static VALUE
gdkeventselection_target(self)
    VALUE self;
{
    return make_gdkatom(get_gdkevent(self)->selection.target);
}

static VALUE
gdkeventselection_property(self)
    VALUE self;
{
    return make_gdkatom(get_gdkevent(self)->selection.property);
}

static VALUE
gdkeventselection_requestor(self)
    VALUE self;
{
#ifdef GDK_NATIVE_WINDOW_POINTER
    return UINT2NUM(GPOINTER_TO_UINT(get_gdkevent(self)->selection.requestor));
#else
    return UINT2NUM(get_gdkevent(self)->selection.requestor);
#endif
}

static VALUE
gdkeventselection_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->selection.time);
}

/* GdkEventDND */
static VALUE
gdkeventDND_context(self)
    VALUE self;
{
    return GOBJ2RVAL(get_gdkevent(self)->dnd.context);
}

static VALUE
gdkeventDND_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->dnd.time);
}

static VALUE
gdkeventDND_x_root(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->dnd.x_root);
}

static VALUE
gdkeventDND_y_root(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->dnd.y_root);
}


/* GdkEventProximity */
static VALUE
gdkeventproximity_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->proximity.time);
}

/* GdkEventClient */
static VALUE
gdkeventclient_message_type(self)
    VALUE self;
{
    return INT2NUM((int)(get_gdkevent(self)->client.message_type));
}

static VALUE
gdkeventclient_data_format(self)
    VALUE self;
{
    return INT2NUM(get_gdkevent(self)->client.data_format);
}

static VALUE
gdkeventclient_data_byte(self)
    VALUE self;
{
    VALUE ary = rb_ary_new2(20);
    int i;
    for (i=0; i<20; i++)
		rb_ary_push(ary, INT2FIX(get_gdkevent(self)->client.data.b[i]));

    return ary;
}

static VALUE
gdkeventclient_data_short(self)
    VALUE self;
{
    VALUE ary = rb_ary_new2(10);
    int i;
    for (i=0; i<10; i++)
		rb_ary_push(ary, INT2FIX(get_gdkevent(self)->client.data.s[i]));

    return ary;
}

static VALUE
gdkeventclient_data_long(self)
    VALUE self;
{
    VALUE ary = rb_ary_new2(5);
    int i;
    for (i=0; i<5; i++)
		rb_ary_push(ary, INT2FIX(get_gdkevent(self)->client.data.l[i]));

    return ary;
}

static VALUE
gdkeventclient_send_client_message(self, xid)
    VALUE self, xid;
{
    return gdk_event_send_client_message(
	    get_gdkevent(self), NUM2INT(xid)) ? Qtrue : Qfalse;
}

static VALUE
gdkeventclient_send_clientmessage_toall(self)
    VALUE self;
{
    gdk_event_send_clientmessage_toall(get_gdkevent(self));
    return Qnil;
}

static
void gdkevent_r2g(VALUE from, GValue *to)
{
    GdkEvent* event = get_gdkevent(from);
    g_value_set_boxed(to, event);
}

static
VALUE gdkevent_g2r(GValue *from)
{
    return make_gdkevent(g_value_get_boxed(from));
}

void
Init_gtk_gdk_event()
{
    gdkEvent = rb_define_class_under(mGdk, "Event", rb_cData);
    gdkEventAny = rb_define_class_under(mGdk, "EventAny", gdkEvent);
    gdkEventExpose = rb_define_class_under(mGdk, "EventExpose", gdkEventAny);
    gdkEventNoExpose = rb_define_class_under(mGdk, "EventNoExpose", gdkEventAny);
    gdkEventVisibility = rb_define_class_under(mGdk, "EventVisibility", gdkEventAny);
    gdkEventMotion = rb_define_class_under(mGdk, "EventMotion", gdkEventAny);
    gdkEventButton = rb_define_class_under(mGdk, "EventButton", gdkEventAny);
    gdkEventKey = rb_define_class_under(mGdk, "EventKey", gdkEventAny);
    gdkEventCrossing = rb_define_class_under(mGdk, "EventCrossing", gdkEventAny);
    gdkEventFocus = rb_define_class_under(mGdk, "EventFocus", gdkEventAny);
    gdkEventConfigure = rb_define_class_under(mGdk, "EventConfigure", gdkEventAny);
    gdkEventProperty = rb_define_class_under(mGdk, "EventProperty", gdkEventAny);
    gdkEventSelection = rb_define_class_under(mGdk, "EventSelection", gdkEventAny);
    gdkEventDND = rb_define_class_under(mGdk, "EventDND", gdkEventAny);
    gdkEventProximity = rb_define_class_under(mGdk, "EventProximity", gdkEventAny);
    gdkEventClient = rb_define_class_under(mGdk, "EventClient", gdkEventAny);
    gdkEventOther = rb_define_class_under(mGdk, "EventOther", gdkEventAny);

    /* GdkEvent */
    rb_define_method(gdkEvent, "event_type", gdkevent_type, 0);

    rb_define_singleton_method(gdkEvent, "peek", gdkevent_s_peek, 0);
    rb_define_singleton_method(gdkEvent, "get", gdkevent_s_get, 0);
    rb_define_singleton_method(gdkEvent, "get_graphics_expose", 
							   gdkevent_s_get_graphics_expose, 1);
    rb_define_method(gdkEvent, "put", gdkevent_put, 0);
    rb_define_method(gdkEvent, "copy", gdkevent_copy, 0);
    rb_define_method(gdkEvent, "get_time", gdkevent_get_time, 0);

    /* GdkEventAny */
    rb_define_method(gdkEventAny, "window", gdkeventany_window, 0);
    rb_define_method(gdkEventAny, "send_event", gdkeventany_send_event, 0);

    /* GdkEventExpose */
    rb_define_method(gdkEventExpose, "area", gdkeventexpose_area, 0);
    rb_define_method(gdkEventExpose, "count", gdkeventexpose_count, 0);

    /* GdkEventVisibility */
    rb_define_method(gdkEventVisibility, "state", gdkeventvisibility_state, 0);

    /* GdkEventMotion */
    rb_define_method(gdkEventMotion, "time", gdkeventmotion_time, 0);
    rb_define_method(gdkEventMotion, "x", gdkeventmotion_x, 0);
    rb_define_method(gdkEventMotion, "y", gdkeventmotion_y, 0);
    rb_define_method(gdkEventMotion, "state", gdkeventmotion_state, 0);
    rb_define_method(gdkEventMotion, "is_hint", gdkeventmotion_is_hint, 0);
    rb_define_method(gdkEventMotion, "x_root", gdkeventmotion_x_root, 0);
    rb_define_method(gdkEventMotion, "y_root", gdkeventmotion_y_root, 0);

    /* GdkEventButton */
    rb_define_method(gdkEventButton, "time", gdkeventbutton_time, 0);
    rb_define_method(gdkEventButton, "x", gdkeventbutton_x, 0);
    rb_define_method(gdkEventButton, "y", gdkeventbutton_y, 0);
    rb_define_method(gdkEventButton, "x_root", gdkeventbutton_x_root, 0);
    rb_define_method(gdkEventButton, "y_root", gdkeventbutton_y_root, 0);

    /* GdkEventKey */
    rb_define_method(gdkEventKey, "time",   gdkeventkey_time, 0);
    rb_define_method(gdkEventKey, "state",  gdkeventkey_state, 0);
    rb_define_method(gdkEventKey, "keyval", gdkeventkey_keyval, 0);
    rb_define_method(gdkEventKey, "length", gdkeventkey_length, 0);
    rb_define_method(gdkEventKey, "string", gdkeventkey_string, 0);

    /* GdkEventCrossing */
    rb_define_method(gdkEventCrossing, "subwindow", gdkeventcrossing_subwindow, 0);
    rb_define_method(gdkEventCrossing, "time", gdkeventcrossing_time, 0);
    rb_define_method(gdkEventCrossing, "x", gdkeventcrossing_x, 0);
    rb_define_method(gdkEventCrossing, "y", gdkeventcrossing_y, 0);
    rb_define_method(gdkEventCrossing, "x_root", gdkeventcrossing_x_root, 0);
    rb_define_method(gdkEventCrossing, "y_root", gdkeventcrossing_y_root, 0);
    rb_define_method(gdkEventCrossing, "mode", gdkeventcrossing_mode, 0);
    rb_define_method(gdkEventCrossing, "detail", gdkeventcrossing_detail, 0);
    rb_define_method(gdkEventCrossing, "focus", gdkeventcrossing_focus, 0);
    rb_define_method(gdkEventCrossing, "state", gdkeventcrossing_state, 0);

    /* GdkEventFocus */
    rb_define_method(gdkEventFocus, "in", gdkeventfocus_in, 0);

    /* GdkEventConfigure */
    rb_define_method(gdkEventConfigure, "x", gdkeventconfigure_x, 0);
    rb_define_method(gdkEventConfigure, "y", gdkeventconfigure_y, 0);
    rb_define_method(gdkEventConfigure, "width", gdkeventconfigure_width, 0);
    rb_define_method(gdkEventConfigure, "height", gdkeventconfigure_height, 0);

    /* GdkEventProperty */
    rb_define_method(gdkEventProperty, "atom", gdkeventproperty_atom, 0);
    rb_define_method(gdkEventProperty, "time", gdkeventproperty_time, 0);
    rb_define_method(gdkEventProperty, "state", gdkeventproperty_state, 0);

    /* GdkEventSelection */
    rb_define_method(gdkEventSelection, "selection", gdkeventselection_selection, 0);
    rb_define_method(gdkEventSelection, "target", gdkeventselection_target, 0);
    rb_define_method(gdkEventSelection, "property", gdkeventselection_property, 0);
    rb_define_method(gdkEventSelection, "requestor", gdkeventselection_requestor, 0);
    rb_define_method(gdkEventSelection, "time", gdkeventselection_time, 0);

    /* GdkEventDND */
    rb_define_method(gdkEventDND, "context", gdkeventDND_context, 0);
    rb_define_method(gdkEventDND, "time", gdkeventDND_time, 0);
    rb_define_method(gdkEventDND, "x_root", gdkeventDND_x_root, 0);
    rb_define_method(gdkEventDND, "y_root", gdkeventDND_y_root, 0);

    /* GdkEventProximity */
    rb_define_method(gdkEventProximity, "time", gdkeventproximity_time, 0);

    /* GdkEventClient */
    rb_define_method(gdkEventClient, "message_type", gdkeventclient_message_type, 0);
    rb_define_method(gdkEventClient, "data_format", gdkeventclient_data_format, 0);
    rb_define_method(gdkEventClient, "data_byte", gdkeventclient_data_byte, 0);
    rb_define_method(gdkEventClient, "data_short", gdkeventclient_data_short, 0);
    rb_define_method(gdkEventClient, "data_long", gdkeventclient_data_long, 0);
    rb_define_method(gdkEventClient, "send_client_message", 
					 gdkeventclient_send_client_message, 1);
    rb_define_method(gdkEventClient, "send_clientmessage_toall", 
					 gdkeventclient_send_clientmessage_toall, 0);
    rb_define_alias(gdkEventClient, "send_client_message_toall",
					"send_clientmessage_toall");

    rbgobj_register_r2g_func(gdkEvent, &gdkevent_r2g);
    rbgobj_register_g2r_func(GDK_TYPE_EVENT, &gdkevent_g2r);
}
