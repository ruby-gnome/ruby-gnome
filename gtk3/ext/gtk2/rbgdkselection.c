/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE mSelection
#define GATOM2RVAL(g) (BOXED2RVAL(g, GDK_TYPE_ATOM))

static VALUE
rg_m_owner_set(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE owner, selection, time, send_event;
    int ret;

    if (argc == 4){
        rb_scan_args(argc, argv, "40", &owner, &selection, &time, &send_event);
        ret = gdk_selection_owner_set(GDK_WINDOW(RVAL2GOBJ(owner)), 
                                      RVAL2ATOM(selection), 
                                      NUM2UINT(time), RVAL2CBOOL(send_event));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
      VALUE display = Qnil;
      rb_scan_args(argc, argv, "50", &display, &owner, &selection, &time, &send_event);
      ret = gdk_selection_owner_set_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                GDK_WINDOW(RVAL2GOBJ(owner)), 
                                                RVAL2ATOM(selection), 
                                                NUM2UINT(time), RVAL2CBOOL(send_event));
#else
      rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif

    }
    return CBOOL2RVAL(ret);
}

static VALUE
rg_m_owner_get(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_m_convert(VALUE self, VALUE requestor, VALUE selection, VALUE target, VALUE time)
{
    gdk_selection_convert(GDK_WINDOW(RVAL2GOBJ(requestor)), 
                          RVAL2ATOM(selection), 
                          RVAL2ATOM(target), NUM2INT(time));
    return self;
}

static VALUE
rg_m_property_get(G_GNUC_UNUSED VALUE self, VALUE requestor)
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
rg_m_send_notify(int argc, VALUE *argv, VALUE self)
{
    VALUE requestor, selection, target, property, time;

    if (argc == 5) {
        rb_scan_args(argc, argv, "50", &requestor, &selection, &target, &property, &time);
        gdk_selection_send_notify(RVAL2GDKNATIVEWINDOW(requestor), RVAL2ATOM(selection),
                                  RVAL2ATOM(target), 
                                  NIL_P(property) ? GDK_NONE : RVAL2ATOM(property), 
                                  NUM2INT(time));
    } else {
#if GTK_CHECK_VERSION(2,2,0)
      VALUE display = Qnil;
      rb_scan_args(argc, argv, "60", &display, &requestor, &selection, &target, &property, &time);
      gdk_selection_send_notify_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                            RVAL2GDKNATIVEWINDOW(requestor), RVAL2ATOM(selection),
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
Init_gtk_gdk_selection(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Selection");

    RG_DEF_MODFUNC(owner_set, -1);
    RG_DEF_MODFUNC(owner_get, -1);
    RG_DEF_MODFUNC(convert, 4);
    RG_DEF_MODFUNC(property_get, 1);
    RG_DEF_MODFUNC(send_notify, -1);

    /* Constants */
    rb_define_const(RG_TARGET_NAMESPACE, "PRIMARY", GATOM2RVAL(GDK_SELECTION_PRIMARY));
    rb_define_const(RG_TARGET_NAMESPACE, "SECONDARY", GATOM2RVAL(GDK_SELECTION_SECONDARY));
    rb_define_const(RG_TARGET_NAMESPACE, "CLIPBOARD", GATOM2RVAL(GDK_SELECTION_CLIPBOARD));

    /* GdkSelectionType */
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_ATOM", GATOM2RVAL(GDK_SELECTION_TYPE_ATOM));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_BITMAP", GATOM2RVAL(GDK_SELECTION_TYPE_BITMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_COLORMAP", GATOM2RVAL(GDK_SELECTION_TYPE_COLORMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_DRAWABLE", GATOM2RVAL(GDK_SELECTION_TYPE_DRAWABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_INTEGER", GATOM2RVAL(GDK_SELECTION_TYPE_INTEGER));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_PIXMAP", GATOM2RVAL(GDK_SELECTION_TYPE_PIXMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_WINDOW", GATOM2RVAL(GDK_SELECTION_TYPE_WINDOW));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STRING", GATOM2RVAL(GDK_SELECTION_TYPE_STRING));

    /* GdkTarget */
    rb_define_const(RG_TARGET_NAMESPACE, "TARGET_BITMAP", GATOM2RVAL(GDK_TARGET_BITMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TARGET_COLORMAP", GATOM2RVAL(GDK_TARGET_COLORMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TARGET_DRAWABLE", GATOM2RVAL(GDK_TARGET_DRAWABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "TARGET_PIXMAP", GATOM2RVAL(GDK_TARGET_PIXMAP));
    rb_define_const(RG_TARGET_NAMESPACE, "TARGET_STRING", GATOM2RVAL(GDK_TARGET_STRING));

}
