/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkwindowgroup.c -

  $Author: mutoh $
  $Date: 2002/06/22 05:26:50 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

VALUE gWindowGroup;

/*
 * Gtk::WindowGroup
 */
static VALUE
rbgwingrp_initialize(self)
	 VALUE self;
{
  rbgobj_set_gobject(self, G_OBJECT(gtk_window_group_new()));
  return Qnil;
}

static VALUE
rbgwingrp_add_window(self, window)
	 VALUE self, window;
{
  gtk_window_group_add_window(GTK_WINDOW_GROUP(RVAL2GOBJ(self)), 
							  GTK_WINDOW(RVAL2GOBJ(window)));
  return self;
}

static VALUE
rbgwingrp_remove_window(self, window)
	 VALUE self, window;
{
  gtk_window_group_remove_window(GTK_WINDOW_GROUP(RVAL2GOBJ(self)), 
								 GTK_WINDOW(RVAL2GOBJ(window)));
  return self;
}
  

void 
Init_gtk_windowgroup()
{
    static RGObjClassInfo cinfo;

    gWindowGroup = rb_define_class_under(mGtk, "WindowGroup", rbgobj_cGObject);
    cinfo.klass = gWindowGroup;
    cinfo.gtype = GTK_TYPE_WINDOW_GROUP;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgobj_register_class(&cinfo);

    rb_define_method(gWindowGroup, "initialize", rbgwingrp_initialize, 0);
	rb_define_method(gWindowGroup, "add", rbgwingrp_add_window, 1);
	rb_define_method(gWindowGroup, "remove", rbgwingrp_remove_window, 1);
}
