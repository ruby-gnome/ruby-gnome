/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;
VALUE gdkError;

VALUE gdkEvent;

VALUE gdk_object_list;

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

VALUE
make_gdkcmap(cmap)
    GdkColormap *cmap;
{
    if (cmap == NULL) return Qnil;

    gdk_colormap_ref(cmap);
    return Data_Wrap_Struct(gdkColormap, 0, gdk_colormap_unref, cmap);
}

GdkColormap*
get_gdkcmap(cmap)
    VALUE cmap;
{
    GdkColormap *gcmap;

    if (NIL_P(cmap)) return NULL;

    if (!rb_obj_is_kind_of(cmap, gdkColormap)) {
        rb_raise(rb_eTypeError, "not a GdkColormap");
    }
    Data_Get_Struct(cmap, GdkColormap, gcmap);

    return gcmap;
}

VALUE
make_gdkvisual(visual)
    GdkVisual *visual;
{
    if (visual == NULL) return Qnil;

    gdk_visual_ref(visual);
#if GTK_MAJOR_VERSION >= 2
    return Data_Wrap_Struct(gdkVisual, 0, g_object_unref, visual);
#else
    return Data_Wrap_Struct(gdkVisual, 0, gdk_visual_unref, visual);
#endif
}

GdkVisual*
get_gdkvisual(visual)
    VALUE visual;
{
    GdkVisual *gvisual;

    if (NIL_P(visual)) return NULL;

    if (!rb_obj_is_kind_of(visual, gdkVisual)) {
        rb_raise(rb_eTypeError, "not a GdkVisual");
    }
    Data_Get_Struct(visual, GdkVisual, gvisual);

    return gvisual;
}

static void
delete_gdkdraw(obj)
    VALUE obj;
{
    GdkDrawable *draw = NULL;
    VALUE klass;

    klass = rb_class_of(obj);
    Data_Get_Struct(draw, GdkDrawable, draw);
    if (0);
    else if (klass == gdkWindow) gdk_window_unref(draw);
    else if (klass == gdkBitmap) gdk_bitmap_unref(draw);
    else if (klass == gdkPixmap) gdk_pixmap_unref(draw);
    else {
        rb_p(klass);
        rb_raise(rb_eTypeError, "not a Gdk::Drawable object.");
    }
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)draw), Qnil);
}

VALUE
make_gdkdraw(klass, draw, ref, unref)
    VALUE klass;
    GdkDrawable *draw;
    void (*ref)();
    void (*unref)();
{
    VALUE obj;

    if (draw == NULL) return Qnil;

    obj = rb_hash_aref(gdk_object_list, INT2NUM((VALUE)draw));
    if (obj == Qnil) {
        (*ref)(draw);
        obj = Data_Wrap_Struct(klass, 0, delete_gdkdraw, draw);
        rb_hash_aset(gdk_object_list, INT2NUM((VALUE)draw), INT2NUM(obj));
    } else {
        obj = NUM2INT(obj);
    }
    return obj;
}

GdkWindow*
get_gdkdraw(draw, klass, kname)
    VALUE draw, klass;
    const char *kname;
{
    GdkDrawable *d;

    if (NIL_P(draw)) return NULL;

    if (!rb_obj_is_kind_of(draw, klass)) {
        rb_raise(rb_eTypeError, "not a %s", kname);
    }
    Data_Get_Struct(draw, GdkDrawable, d);

    return d;
}

static void
delete_gdkwindow(window)
    GdkWindow *window;
{
    gdk_window_unref(window);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)window), Qnil);
}

VALUE
new_gdkwindow(window)
    GdkWindow *window;
{
    VALUE obj;

    if (window == NULL) return Qnil;

    obj = Data_Wrap_Struct(gdkWindow, 0, delete_gdkwindow, window);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)window), INT2NUM(obj));

    return obj;
}

VALUE
make_gdkwindow(window)
    GdkWindow *window;
{
    VALUE obj;

    if (window == NULL) return Qnil;

    obj = rb_hash_aref(gdk_object_list, INT2NUM((VALUE)window));
    if (obj == Qnil) {
        gdk_window_ref(window);
        obj = new_gdkwindow(window);
    } else {
        obj = NUM2INT(obj);
    }
    return obj;
}

static void
delete_gdkpixmap(pixmap)
    GdkPixmap *pixmap;
{
    gdk_pixmap_unref(pixmap);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)pixmap), Qnil);
}

VALUE
new_gdkpixmap(pixmap)
    GdkPixmap *pixmap;
{
    VALUE obj;

    if (pixmap == NULL) return Qnil;

    obj = Data_Wrap_Struct(gdkPixmap, 0, delete_gdkpixmap, pixmap);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)pixmap), INT2NUM(obj));

    return obj;
}

VALUE
make_gdkpixmap(pixmap)
    GdkPixmap *pixmap;
{
    VALUE obj;

    if (pixmap == NULL) return Qnil;

    obj = rb_hash_aref(gdk_object_list, INT2NUM((VALUE)pixmap));
    if (obj == Qnil) {
        gdk_pixmap_ref(pixmap);
        obj = new_gdkpixmap(pixmap);
    } else {
        obj = NUM2INT(obj);
    }
    return obj;
}

static void
delete_gdkbitmap(bitmap)
    GdkBitmap *bitmap;
{
    gdk_bitmap_unref(bitmap);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)bitmap), Qnil);
}

VALUE
new_gdkbitmap(bitmap)
    GdkBitmap *bitmap;
{
    VALUE obj;

    if (bitmap == NULL) return Qnil;

    obj = Data_Wrap_Struct(gdkBitmap, 0, delete_gdkbitmap, bitmap);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)bitmap), INT2NUM(obj));

    return obj;
}

VALUE
make_gdkbitmap(bitmap)
    GdkBitmap *bitmap;
{
    VALUE obj;

    if (bitmap == NULL) return Qnil;

    obj = rb_hash_aref(gdk_object_list, INT2NUM((VALUE)bitmap));
    if (obj == Qnil) {
        gdk_bitmap_ref(bitmap);
        obj = new_gdkbitmap(bitmap);
    } else {
        obj = NUM2INT(obj);
    }
    return obj;
}

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

VALUE
make_gdkgc(gc)
    GdkGC *gc;
{
    if (gc == NULL) return Qnil;

    gdk_gc_ref(gc);
    return Data_Wrap_Struct(gdkGC, 0, gdk_gc_destroy, gc);
}

GdkGC*
get_gdkgc(gc)
    VALUE gc;
{
    GdkGC *ggc;

    if (NIL_P(gc)) return NULL;

    if (!rb_obj_is_instance_of(gc, gdkGC)) {
        rb_raise(rb_eTypeError, "not a GdkGC");
    }
    Data_Get_Struct(gc, GdkGC, ggc);
    if (ggc == 0) {
        rb_raise(rb_eArgError, "destroyed GdkGC");
    }

    return ggc;
}

VALUE
make_gdkcursor(cursor)
    GdkCursor *cursor;
{
    if (cursor == NULL) return Qnil;

    /* gdk_cursor_ref(cursor); */
    return Data_Wrap_Struct(gdkCursor, 0, gdk_cursor_destroy, cursor);
}

GdkCursor*
get_gdkcursor(cursor)
    VALUE cursor;
{
    GdkCursor *gcursor;

    if (NIL_P(cursor)) return NULL;

    if (!rb_obj_is_kind_of(cursor, gdkCursor)) {
        rb_raise(rb_eTypeError, "not a GdkCursor");
    }
    Data_Get_Struct(cursor, GdkCursor, gcursor);

    return gcursor;
}

static void
delete_gdkdragcontext(context)
    GdkDragContext *context;
{
    gdk_drag_context_unref(context);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)context), Qnil);
}

VALUE
new_gdkdragcontext(context)
    GdkDragContext *context;
{
    VALUE obj;

    if (context == NULL) return Qnil;

    obj = Data_Wrap_Struct(gdkDragContext, 0, delete_gdkdragcontext, context);
    rb_hash_aset(gdk_object_list, INT2NUM((VALUE)context), INT2NUM(obj));

    return obj;
}

VALUE
make_gdkdragcontext(context)
    GdkDragContext *context;
{
    VALUE obj;

    if (context == NULL) return Qnil;

    obj = rb_hash_aref(gdk_object_list, INT2NUM((VALUE)context));
    if (obj == Qnil) {
        gdk_drag_context_ref(context);
        obj = new_gdkdragcontext(context);
    } else {
        obj = NUM2INT(obj);
    }
    return obj;
}

GdkDragContext*
get_gdkdragcontext(context)
    VALUE context;
{
    GdkDragContext *gcontext;

    if (NIL_P(context)) return NULL;

    if (!rb_obj_is_kind_of(context, gdkDragContext)) {
        rb_raise(rb_eTypeError, "not a GdkDragContext");
    }
    Data_Get_Struct(context, GdkDragContext, gcontext);

    return gcontext;
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
extern void Init_gtk_gdk_color();
extern void Init_gtk_gdk_cursor();
extern void Init_gtk_gdk_visual();
extern void Init_gtk_gdk_draw();
extern void Init_gtk_gdk_event();
extern void Init_gtk_gdk_font();
extern void Init_gtk_gdk_gc();
extern void Init_gtk_gdk_image();
#ifdef USE_XIM
extern void Init_gtk_gdk_im();
#endif /* USE_XIM */
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
    gdk_object_list = rb_hash_new();
    rb_global_variable(&gdk_object_list);

    /* Gdk */
    mGdk = rb_define_module("Gdk");
    gdkError = rb_define_class_under(mGdk, "Error", rb_eRuntimeError);
    rb_define_singleton_method(mGdk, "screen_width", gdk_s_screen_width, 0);
    rb_define_singleton_method(mGdk, "screen_width_mm", gdk_s_screen_width_mm, 0);
    rb_define_singleton_method(mGdk, "screen_height", gdk_s_screen_height, 0);
    rb_define_singleton_method(mGdk, "screen_height_mm", gdk_s_screen_height_mm, 0);
    rb_define_singleton_method(mGdk, "beep", gdk_s_beep, 0);
    rb_define_singleton_method(mGdk, "flush", gdk_s_flush, 0);
    rb_define_singleton_method(mGdk, "events_pending", gdk_s_events_pending, 0);

    Init_gtk_gdk_const();
    Init_gtk_gdk_color();
    Init_gtk_gdk_cursor();
    Init_gtk_gdk_draw();
    Init_gtk_gdk_event();
    Init_gtk_gdk_font();
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

#ifdef USE_XIM
    /* Gdk::IM, Gdk::IC, Gdk::ICAttr */
    Init_gtk_gdk_im();
#endif /* USE_XIM */

}
