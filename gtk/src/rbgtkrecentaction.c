/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentaction.c -

  $Author: ggc $
  $Date: 2007/07/12 14:53:09 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,11,0)

static VALUE
recentaction_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE  name, label, tooltip, stock_id, manager;
    rb_scan_args(argc, argv, "23", &name, &label, &tooltip, &stock_id, &manager);
    if (NIL_P(manager)) {
        G_INITIALIZE(self, gtk_recent_action_new(RVAL2CSTR(name),
                                                 RVAL2CSTR(label),
                                                 NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                                 NIL_P(stock_id) ? NULL : RVAL2CSTR(stock_id)));
    } else {
        G_INITIALIZE(self, gtk_recent_action_new_for_manager(RVAL2CSTR(name),
                                                             RVAL2CSTR(label),
                                                             NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                                             NIL_P(stock_id) ? NULL : RVAL2CSTR(stock_id),
                                                             GTK_RECENT_MANAGER(RVAL2GOBJ(manager))));
    }
    return Qnil;
}

#endif

void 
Init_gtk_recentaction()
{
#if GTK_CHECK_VERSION(2,11,0)
    VALUE gRecentaction = G_DEF_CLASS(GTK_TYPE_RECENT_ACTION, "RecentAction", mGtk);
    rb_define_method(gRecentaction, "initialize", recentaction_initialize, -1);
    G_DEF_SETTERS(gRecentaction);
#endif
}
