/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkselection.c -

  $Author: mutoh $
  $Date: 2002/06/12 16:28:54 $

  Copyright (C) 2002 MUTOH Masao
************************************************/


#include "global.h"

VALUE mGdkSelection;

static VALUE
gdkselection_owner_set(self, owner, selection, time, send_event)
    VALUE self, owner, selection, time, send_event;
{
    int ret = gdk_selection_owner_set(get_gdkwindow(owner), 
                                      get_gdkatom(selection), 
                                      NUM2INT(time), RTEST(send_event));
    return ret ? Qtrue : Qfalse;
}

static VALUE
gdkselection_owner_get(self, selection)
    VALUE self, selection;
{
    return make_gdkwindow(gdk_selection_owner_get(get_gdkatom(selection)));
}

static VALUE
gdkselection_convert(self, requestor, selection, target, time)
    VALUE self, requestor, selection, target, time;
{
    gdk_selection_convert(get_gdkwindow(requestor), get_gdkatom(selection), 
                        get_gdkatom(target), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkselection_property_get(self, requestor)
    VALUE self, requestor;
{
    guchar *data;
    GdkAtom prop_type;
    gint prop_format;

    gdk_selection_property_get(get_gdkwindow(requestor), &data, 
                               &prop_type, &prop_format);
    return rb_ary_new3(3, rb_str_new2(data), make_gdkatom(prop_type), 
                       INT2NUM(prop_format));
}

static VALUE
gdkselection_send_notify(self, requestor, selection, target, property, time)
    VALUE self, requestor, selection, target, property, time;
{
    if( property == Qnil){
        gdk_selection_send_notify(NUM2INT(requestor), get_gdkatom(selection),
				  get_gdkatom(target), GDK_NONE, NUM2INT(time));
    } else {
        gdk_selection_send_notify(NUM2INT(requestor), get_gdkatom(selection),
				  get_gdkatom(target), get_gdkatom(property), NUM2INT(time));
    }
    return Qnil;
}

void
Init_gtk_gdk_selection()
{
    mGdkSelection = rb_define_module_under(mGdk, "Selection");

    rb_define_module_function(mGdkSelection, "owner_set", gdkselection_owner_set, 4);
    rb_define_module_function(mGdkSelection, "owner_get", gdkselection_owner_get, 1);
    rb_define_module_function(mGdkSelection, "convert", gdkselection_convert, 4);
    rb_define_module_function(mGdkSelection, "property_get", gdkselection_property_get, 1);
    rb_define_module_function(mGdkSelection, "send_notify", gdkselection_send_notify, 5);
}
