/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkselection.c -

  $Author: mutoh $
  $Date: 2002/10/17 15:23:10 $

  Copyright (C) 2002 Masao Mutoh
************************************************/


#include "global.h"

#define RVAL2GATOM(v) (((GdkAtomData*)RVAL2BOXED(v, GDK_TYPE_ATOM))->atom)
#define GATOM2RVAL(g) (BOXED2RVAL(g, GDK_TYPE_ATOM))

static VALUE
gdkselection_owner_set(self, owner, selection, time, send_event)
    VALUE self, owner, selection, time, send_event;
{
    int ret = gdk_selection_owner_set(GDK_WINDOW(RVAL2GOBJ(owner)), 
                                      RVAL2GATOM(selection), 
                                      NUM2INT(time), RTEST(send_event));
    return ret ? Qtrue : Qfalse;
}

static VALUE
gdkselection_owner_get(self, selection)
    VALUE self, selection;
{
    return GOBJ2RVAL(gdk_selection_owner_get(RVAL2GATOM(selection)));
}
/*
GdkWindow*  gdk_selection_owner_get_for_display(GdkDisplay *display,
GdkAtom selection);
*/
static VALUE
gdkselection_convert(self, requestor, selection, target, time)
    VALUE self, requestor, selection, target, time;
{
    gdk_selection_convert(GDK_WINDOW(RVAL2GOBJ(requestor)), 
                          RVAL2GATOM(selection), 
                          RVAL2GATOM(target), NUM2INT(time));
    return requestor;
}

static VALUE
gdkselection_property_get(self, requestor)
    VALUE self, requestor;
{
    guchar *data;
    GdkAtom prop_type;
    gint prop_format;

    gdk_selection_property_get(GDK_WINDOW(RVAL2GOBJ(requestor)), &data, 
                               &prop_type, &prop_format);
    return rb_ary_new3(3, rb_str_new2(data), GATOM2RVAL(prop_type), 
                       INT2NUM(prop_format));
}

static VALUE
gdkselection_send_notify(self, requestor, selection, target, property, time)
    VALUE self, requestor, selection, target, property, time;
{
    if( property == Qnil){
        gdk_selection_send_notify(NUM2INT(requestor), RVAL2GATOM(selection),
                                  RVAL2GATOM(target), GDK_NONE, NUM2INT(time));
    } else {
        gdk_selection_send_notify(NUM2INT(requestor), RVAL2GATOM(selection),
                                  RVAL2GATOM(target), RVAL2GATOM(property), 
                                  NUM2INT(time));
    }
    return requestor;
}
/*
void        gdk_selection_send_notify_for_display
                                            (GdkDisplay *display,
                                             guint32 requestor,
                                             GdkAtom selection,
                                             GdkAtom target,
                                             GdkAtom property,
                                             guint32 time);
*/

void
Init_gtk_gdk_selection()
{
    VALUE mGdkSelection = rb_define_module_under(mGdk, "Selection");

    rb_define_module_function(mGdkSelection, "owner_set", gdkselection_owner_set, 4);
    rb_define_module_function(mGdkSelection, "owner_get", gdkselection_owner_get, 1);
    rb_define_module_function(mGdkSelection, "convert", gdkselection_convert, 4);
    rb_define_module_function(mGdkSelection, "property_get", gdkselection_property_get, 1);
    rb_define_module_function(mGdkSelection, "send_notify", gdkselection_send_notify, 5);

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
