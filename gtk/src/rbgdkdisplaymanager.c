/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplaymanager.c -

  $Author: kzys $
  $Date: 2003/08/18 20:43:28 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#define _DISPLAY_MANAGER(obj) GDK_DISPLAY_MANAGER(RVAL2GOBJ(obj))

static VALUE
gdkdisplaymanager_get(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_manager_get());
}

static VALUE
gdkdisplaymanager_get_default_display(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_manager_get_default_display(_DISPLAY_MANAGER(self)));
}

static VALUE
gdkdisplaymanager_set_default_display(self, display)
    VALUE self, display;
{
    gdk_display_manager_set_default_display(_DISPLAY_MANAGER(self), RVAL2GOBJ(display));
    return self;
}

static VALUE
gdkdisplaymanager_list_displays(self)
    VALUE self;
{
    return GSLIST2ARY(gdk_display_manager_list_displays(_DISPLAY_MANAGER(self)));
}

void 
Init_gtk_gdk_display_manager()
{
    VALUE gdkDisplayManager = G_DEF_CLASS(GDK_TYPE_DISPLAY_MANAGER, "DisplayManager", mGdk);

    rb_define_singleton_method(gdkDisplayManager, "get", gdkdisplaymanager_get, 0);
    rb_define_method(gdkDisplayManager, "get_default_display", gdkdisplaymanager_get_default_display, 0);
    rb_define_alias(gdkDisplayManager, "default_display", "get_default_display");
    rb_define_method(gdkDisplayManager, "set_default_display", gdkdisplaymanager_set_default_display, 1);
    rb_define_method(gdkDisplayManager, "list_displays", gdkdisplaymanager_list_displays, 0);

    G_DEF_SETTERS(gdkDisplayManager);
}
