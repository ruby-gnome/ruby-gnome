/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkwindowgroup.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:19 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

/*
 * Gtk::WindowGroup
 */
static VALUE
rbgwingrp_initialize(self)
	VALUE self;
{
	RBGOBJ_INITIALIZE(self, gtk_window_group_new());
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
    VALUE gWindowGroup = G_DEF_CLASS(GTK_TYPE_WINDOW_GROUP, "WindowGroup", mGtk);
	rb_define_method(gWindowGroup, "initialize", rbgwingrp_initialize, 0);
	rb_define_method(gWindowGroup, "add", rbgwingrp_add_window, 1);
	rb_define_method(gWindowGroup, "remove", rbgwingrp_remove_window, 1);
}
