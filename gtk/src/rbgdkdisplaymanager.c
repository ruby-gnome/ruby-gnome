/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplaymanager.c -

  $Author: sakai $
  $Date: 2003/11/20 18:27:54 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,2,0)
#define _SELF(obj) GDK_DISPLAY_MANAGER(RVAL2GOBJ(obj))

static VALUE
gdkdisplaymanager_get(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_manager_get());
}

static VALUE
gdkdisplaymanager_list_displays(self)
    VALUE self;
{
    return GSLIST2ARY(gdk_display_manager_list_displays(_SELF(self)));
}

/* Move to Gdk::Display.
static VALUE
gdkdisplaymanager_get_core_pointer(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_core_pointer(_SELF(self)));
}
*/
#endif

void 
Init_gtk_gdk_display_manager()
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE gdkDisplayManager = G_DEF_CLASS(GDK_TYPE_DISPLAY_MANAGER, "DisplayManager", mGdk);

    rb_define_singleton_method(gdkDisplayManager, "get", gdkdisplaymanager_get, 0);
    rb_define_method(gdkDisplayManager, "displays", gdkdisplaymanager_list_displays, 0);
#endif
}
