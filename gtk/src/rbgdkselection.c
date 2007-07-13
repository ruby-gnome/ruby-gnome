/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkselection.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:08 $

  Copyright (C) 2002-2004 Masao Mutoh
************************************************/


#include "global.h"

#define GATOM2RVAL(g) (BOXED2RVAL(g, GDK_TYPE_ATOM))

static VALUE
gdkselection_owner_set(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE owner, selection, time, send_event;
    int ret;

    if (argc == 4){
        rb_scan_args(argc, argv, "40", &owner, &selection, &time, &send_event);
        ret = gdk_selection_owner_set(GDK_WINDOW(RVAL2GOBJ(owner)), 
                                      RVAL2ATOM(selection), 
                                      NUM2UINT(time), RTEST(send_event));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
      VALUE display = Qnil;
      rb_scan_args(argc, argv, "50", &display, &owner, &selection, &time, &send_event);
      ret = gdk_selection_owner_set_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                GDK_WINDOW(RVAL2GOBJ(owner)), 
                                                RVAL2ATOM(selection), 
                                                NUM2UINT(time), RTEST(send_event));
#else
      rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif

    }
    return CBOOL2RVAL(ret);
}

static VALUE
gdkselection_owner_get(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE selection;

    if (argc == 1) {
        rb_scan_args(argc, argv, "10", &selection);
        return GOBJ2RVAL(gdk_selection_owner_get(RVAL2ATOM(selection)));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
      VALUE display = Qnil;
      rb_scan_args(argc, argv, "20", &display, &selection);
      return GOBJ2RVAL(gdk_selection_owner_get_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                           RVAL2ATOM(selection)));
#else
      rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    }
}

static VALUE
gdkselection_convert(self, requestor, selection, target, time)
    VALUE self, requestor, selection, target, time;
{
    gdk_selection_convert(GDK_WINDOW(RVAL2GOBJ(requestor)), 
                          RVAL2ATOM(selection), 
                          RVAL2ATOM(target), NUM2INT(time));
    return self;
}

static VALUE
gdkselection_property_get(self, requestor)
    VALUE self, requestor;
{
    guchar *data;
    GdkAtom prop_type;
    gint prop_format;
    VALUE ary;

    gdk_selection_property_get(GDK_WINDOW(RVAL2GOBJ(requestor)), &data, 
                               &prop_type, &prop_format);

    ary = rb_ary_new3(3, CSTR2RVAL((const char*)data), GATOM2RVAL(prop_type), 
                      INT2NUM(prop_format));
    g_free(data);
    return ary;
}

static VALUE
gdkselection_send_notify(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE requestor, selection, target, property, time;

    if (argc == 5) {
        rb_scan_args(argc, argv, "50", &requestor, &selection, &target, &property, &time);
        gdk_selection_send_notify(NUM2UINT(requestor), RVAL2ATOM(selection),
                                  RVAL2ATOM(target), 
                                  NIL_P(property) ? GDK_NONE : RVAL2ATOM(property), 
                                  NUM2INT(time));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
      VALUE display = Qnil;
      rb_scan_args(argc, argv, "60", &display, &requestor, &selection, &target, &property, &time);
      gdk_selection_send_notify_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                            NUM2UINT(requestor), RVAL2ATOM(selection),
                                            RVAL2ATOM(target), 
                                            NIL_P(property) ? GDK_NONE : RVAL2ATOM(property), 
                                            NUM2INT(time));
#else
      rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    }
    return self;
}

void
Init_gtk_gdk_selection()
{
    VALUE mGdkSelection = rb_define_module_under(mGdk, "Selection");

    rb_define_module_function(mGdkSelection, "owner_set", gdkselection_owner_set, -1);
    rb_define_module_function(mGdkSelection, "owner_get", gdkselection_owner_get, -1);
    rb_define_module_function(mGdkSelection, "convert", gdkselection_convert, 4);
    rb_define_module_function(mGdkSelection, "property_get", gdkselection_property_get, 1);
    rb_define_module_function(mGdkSelection, "send_notify", gdkselection_send_notify, -1);

    /* Constants */
    rb_define_const(mGdkSelection, "PRIMARY", GATOM2RVAL(GDK_SELECTION_PRIMARY));
    rb_define_const(mGdkSelection, "SECONDARY", GATOM2RVAL(GDK_SELECTION_SECONDARY));
    rb_define_const(mGdkSelection, "CLIPBOARD", GATOM2RVAL(GDK_SELECTION_CLIPBOARD));
    
    /* GdkSelectionType */
    rb_define_const(mGdkSelection, "TYPE_ATOM", GATOM2RVAL(GDK_SELECTION_TYPE_ATOM));
    rb_define_const(mGdkSelection, "TYPE_BITMAP", GATOM2RVAL(GDK_SELECTION_TYPE_BITMAP));
    rb_define_const(mGdkSelection, "TYPE_COLORMAP", GATOM2RVAL(GDK_SELECTION_TYPE_COLORMAP));
    rb_define_const(mGdkSelection, "TYPE_DRAWABLE", GATOM2RVAL(GDK_SELECTION_TYPE_DRAWABLE));
    rb_define_const(mGdkSelection, "TYPE_INTEGER", GATOM2RVAL(GDK_SELECTION_TYPE_INTEGER));
    rb_define_const(mGdkSelection, "TYPE_PIXMAP", GATOM2RVAL(GDK_SELECTION_TYPE_PIXMAP));
    rb_define_const(mGdkSelection, "TYPE_WINDOW", GATOM2RVAL(GDK_SELECTION_TYPE_WINDOW));
    rb_define_const(mGdkSelection, "TYPE_STRING", GATOM2RVAL(GDK_SELECTION_TYPE_STRING));

    /* GdkTarget */
    rb_define_const(mGdkSelection, "TARGET_BITMAP", GATOM2RVAL(GDK_TARGET_BITMAP));
    rb_define_const(mGdkSelection, "TARGET_COLORMAP", GATOM2RVAL(GDK_TARGET_COLORMAP));
    rb_define_const(mGdkSelection, "TARGET_DRAWABLE", GATOM2RVAL(GDK_TARGET_DRAWABLE));
    rb_define_const(mGdkSelection, "TARGET_PIXMAP", GATOM2RVAL(GDK_TARGET_PIXMAP));
    rb_define_const(mGdkSelection, "TARGET_STRING", GATOM2RVAL(GDK_TARGET_STRING));

}
