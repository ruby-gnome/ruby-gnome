/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2002/08/18 06:28:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;

VALUE gdkEvent;

VALUE
make_tobj(obj, klass, size)
    gpointer obj;
    VALUE klass;
    int size;
{
    gpointer copy;
    VALUE data;

    if (obj == NULL) return Qnil;

    copy = xmalloc(size);
    memcpy(copy, obj, size);
    data = Data_Wrap_Struct(klass, 0, g_free, copy);

    return data;
}

gpointer
get_tobj(obj, klass)
    VALUE obj, klass;
{
    void *ptr;

    if (NIL_P(obj)) return NULL;

    if (!rb_obj_is_instance_of(obj, klass)) {
        rb_raise(rb_eTypeError, "not a %s", rb_class2name(klass));
    }
    Data_Get_Struct(obj, void, ptr);

    return ptr;
}
#ifndef GTK_DISABLE_DEPRECATED
VALUE
make_gdkfont(font)
    GdkFont *font;
{
    VALUE obj;

    if (font == NULL) return Qnil;

    gdk_font_ref(font);
    obj = Data_Wrap_Struct(gdkFont, 0, gdk_font_unref, font);

    return obj;
}

GdkFont*
get_gdkfont(font)
    VALUE font;
{
    GdkFont *gfont;

    if (NIL_P(font)) return NULL;

    if (!rb_obj_is_instance_of(font, gdkFont)) {
        rb_raise(rb_eTypeError, "not a GdkFont");
    }
    Data_Get_Struct(font, GdkFont, gfont);

    return gfont;
}
#endif

VALUE
make_gdkevent(ev)
    GdkEvent *ev;
{
    GdkEvent *event;
    VALUE ret;

    if (ev == NULL) return Qnil;

    event = gdk_event_copy(ev);

    switch (event->type) {

    case GDK_EXPOSE:
        ret = Data_Wrap_Struct(gdkEventExpose, 0, gdk_event_free, event);
        break;

    case GDK_NO_EXPOSE:
        ret = Data_Wrap_Struct(gdkEventNoExpose, 0, gdk_event_free, event);
        break;

    case GDK_VISIBILITY_NOTIFY:
        ret = Data_Wrap_Struct(gdkEventVisibility, 0, gdk_event_free, event);
        break;

    case GDK_MOTION_NOTIFY:
        ret = Data_Wrap_Struct(gdkEventMotion, 0, gdk_event_free, event);
        break;

    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
        ret = Data_Wrap_Struct(gdkEventButton, 0, gdk_event_free, event);
        break;

    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
        ret = Data_Wrap_Struct(gdkEventKey, 0, gdk_event_free, event);
        break;

    case GDK_FOCUS_CHANGE:
        ret = Data_Wrap_Struct(gdkEventFocus, 0, gdk_event_free, event);
        break;

    case GDK_CONFIGURE:
        ret = Data_Wrap_Struct(gdkEventConfigure, 0, gdk_event_free, event);
        break;

    case GDK_PROPERTY_NOTIFY:
        ret = Data_Wrap_Struct(gdkEventProperty, 0, gdk_event_free, event);
        break;

    case GDK_SELECTION_CLEAR:
    case GDK_SELECTION_REQUEST:
    case GDK_SELECTION_NOTIFY:
        ret = Data_Wrap_Struct(gdkEventSelection, 0, gdk_event_free, event);
        break;

    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DRAG_STATUS:
    case GDK_DROP_START:
    case GDK_DROP_FINISHED:
        ret = Data_Wrap_Struct(gdkEventDND, 0, gdk_event_free, event);
        break;

    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
        ret = Data_Wrap_Struct(gdkEventProximity, 0, gdk_event_free, event);
        break;

    case GDK_CLIENT_EVENT:
        ret = Data_Wrap_Struct(gdkEventClient, 0, gdk_event_free, event);
        break;

    case GDK_ENTER_NOTIFY:
    case GDK_LEAVE_NOTIFY:
        ret = Data_Wrap_Struct(gdkEventCrossing, 0, gdk_event_free, event);
        break;

    /*
     * I don't know which these types below should be related to.
     * Please teach me if you know it.
     */
    case GDK_DELETE:
    case GDK_DESTROY:
    case GDK_MAP:
    case GDK_UNMAP:

    default:
        ret = Data_Wrap_Struct(gdkEvent, 0, gdk_event_free, event);
    }

    return ret;
}

GdkEvent*
get_gdkevent(event)
    VALUE event;
{
    GdkEvent *gevent;

    if (NIL_P(event)) return NULL;

    if (!rb_obj_is_kind_of(event, gdkEvent)) {
        rb_raise(rb_eTypeError, "not a GdkEvent...");
    }
    Data_Get_Struct(event, GdkEvent, gevent);

    return gevent;
}

struct _rbgdkatom {
  GdkAtom atom;
};
typedef struct _rbgdkatom GdkAtomData;

VALUE
make_gdkatom(atom)
    GdkAtom atom;
{
    GdkAtomData data;
    data.atom = atom;

    return make_tobj(&data, gdkAtom, sizeof(GdkAtomData));
}

GdkAtom
get_gdkatom(atom)
    VALUE atom;
{
    GdkAtomData* gatom;

    if (NIL_P(atom)) return 0;

    if (!rb_obj_is_kind_of(atom, gdkAtom)) {
        rb_raise(rb_eTypeError, "not a GdkAtom");
    }
    Data_Get_Struct(atom, GdkAtomData, gatom);

    return gatom->atom;
}

/*
 * Gdk
 */
static VALUE
gdk_s_screen_width(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_width());
}

static VALUE
gdk_s_screen_width_mm(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_width_mm());
}

static VALUE
gdk_s_screen_height(self)
   VALUE self;
{
    return INT2NUM(gdk_screen_height());
}

static VALUE
gdk_s_screen_height_mm(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_height_mm());
}

static VALUE
gdk_s_beep(self)
    VALUE self;
{
    gdk_beep();
    return Qnil;
}

static VALUE
gdk_s_flush(self)
    VALUE self;
{
    gdk_flush();
    return Qnil;
}

static VALUE
gdk_s_events_pending(self)
    VALUE self;
{
    return gdk_events_pending() ? Qtrue : Qfalse;
}

extern void Init_gtk_gdk_const();
extern void Init_gtk_gdk_colormap();
extern void Init_gtk_gdk_color();
extern void Init_gtk_gdk_cursor();
extern void Init_gtk_gdk_visual();
extern void Init_gtk_gdk_draw();
extern void Init_gtk_gdk_event();
#ifndef GTK_DISABLE_DEPRECATED
extern void Init_gtk_gdk_font();
#endif
extern void Init_gtk_gdk_gc();
extern void Init_gtk_gdk_image();
extern void Init_gtk_gdk_keyval();
extern void Init_gtk_gdk_pixmap();
extern void Init_gtk_gdk_region();
extern void Init_gtk_gdk_rgb();
extern void Init_gtk_gdk_window();
extern void Init_gtk_gdk_geometry();
extern void Init_gtk_gdkkeysyms();
extern void Init_gtk_gdk_dnd();
extern void Init_gtk_gdk_atom();
extern void Init_gtk_gdk_selection();

void
Init_gtk_gdk()
{
    /* Gdk */
    mGdk = rb_define_module("Gdk");

    rb_define_singleton_method(mGdk, "screen_width", gdk_s_screen_width, 0);
    rb_define_singleton_method(mGdk, "screen_width_mm", gdk_s_screen_width_mm, 0);
    rb_define_singleton_method(mGdk, "screen_height", gdk_s_screen_height, 0);
    rb_define_singleton_method(mGdk, "screen_height_mm", gdk_s_screen_height_mm, 0);
    rb_define_singleton_method(mGdk, "beep", gdk_s_beep, 0);
    rb_define_singleton_method(mGdk, "flush", gdk_s_flush, 0);
    rb_define_singleton_method(mGdk, "events_pending", gdk_s_events_pending, 0);

    Init_gtk_gdk_const();
    Init_gtk_gdk_colormap();
    Init_gtk_gdk_color();
    Init_gtk_gdk_cursor();
    Init_gtk_gdk_draw();
    Init_gtk_gdk_event();
#ifndef GTK_DISABLE_DEPRECATED
    Init_gtk_gdk_font();
#endif
    Init_gtk_gdk_gc();
    Init_gtk_gdk_dnd();
    Init_gtk_gdk_image();
    Init_gtk_gdk_keyval();
    Init_gtk_gdk_visual();
    Init_gtk_gdk_pixmap();
    Init_gtk_gdk_window();
    Init_gtk_gdk_geometry();
    Init_gtk_gdk_region();
    Init_gtk_gdk_rgb();
    Init_gtk_gdkkeysyms();
    Init_gtk_gdk_atom();
    Init_gtk_gdk_selection();
}
