/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkstatusbar.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:24 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_STATUSBAR(RVAL2GOBJ(self)))

static VALUE
statusbar_initialize(self)
     VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_statusbar_new());
    return Qnil;
}

static VALUE
statusbar_get_context_id(self, text)
    VALUE self, text;
{

    return INT2FIX(gtk_statusbar_get_context_id(_SELF(self),
                                                RVAL2CSTR(text)));
}

static VALUE
statusbar_push(self, id, text)
    VALUE self, id, text;
{
    return INT2FIX(gtk_statusbar_push(_SELF(self), 
                                      NUM2INT(id), RVAL2CSTR(text)));
}
    
static VALUE
statusbar_pop(self, id)
    VALUE self, id;
{
    gtk_statusbar_pop(_SELF(self), NUM2INT(id));
    return self;

}

static VALUE
statusbar_remove(self, cid, mid)
     VALUE self, cid, mid;
{
    gtk_statusbar_remove(_SELF(self), NUM2INT(cid), NUM2INT(mid)); 
    return self;
}

static VALUE
statusbar_set_has_resize_grip(self, setting)
     VALUE self, setting;
{
    gtk_statusbar_set_has_resize_grip(_SELF(self), RTEST(setting));
    return self;
}

static VALUE
statusbar_get_has_resize_grip(self)
     VALUE self;
{
    return gtk_statusbar_get_has_resize_grip(_SELF(self)) ? Qtrue : Qfalse;
}

void 
Init_gtk_statusbar()
{
    VALUE gStatusBar = G_DEF_CLASS(GTK_TYPE_STATUSBAR, "Statusbar", mGtk);

    rb_define_method(gStatusBar, "initialize", statusbar_initialize, 0);
    rb_define_method(gStatusBar, "get_context_id", statusbar_get_context_id, 1);
    rb_define_method(gStatusBar, "push", statusbar_push, 2);
    rb_define_method(gStatusBar, "pop", statusbar_pop, 1);
    rb_define_method(gStatusBar, "remove", statusbar_remove, 2);
    rb_define_method(gStatusBar, "set_has_resize_grip", statusbar_set_has_resize_grip, 1);
    rb_define_method(gStatusBar, "has_resize_grip?", statusbar_get_has_resize_grip, 0);

    G_DEF_SETTERS(gStatusBar);
}
