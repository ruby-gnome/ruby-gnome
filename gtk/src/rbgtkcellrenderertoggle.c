/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellrenderertoggle.c -

  $Author: mutoh $
  $Date: 2002/10/02 15:39:06 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_CELL_RENDERER_TOGGLE(RVAL2GOBJ(s)))

static VALUE
crtoggle_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_cell_renderer_toggle_new());
    return Qnil;
}

static VALUE
crtoggle_get_radio(self)
    VALUE self;
{
    return gtk_cell_renderer_toggle_get_radio(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
crtoggle_set_radio(self, radio)
    VALUE self, radio;
{
    gtk_cell_renderer_toggle_set_radio(_SELF(self), RTEST(radio));
    return self;
}

static VALUE
crtoggle_set_radio_equal(self, radio)
    VALUE self, radio;
{
    gtk_cell_renderer_toggle_set_radio(_SELF(self), RTEST(radio));
    return radio;
}

static VALUE
crtoggle_get_active(self)
    VALUE self;
{
    return gtk_cell_renderer_toggle_get_active(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
crtoggle_set_active(self, setting)
    VALUE self, setting;
{
    gtk_cell_renderer_toggle_set_active(_SELF(self), RTEST(setting));
    return self;
}

static VALUE
crtoggle_set_active_equal(self, setting)
    VALUE self, setting;
{
    gtk_cell_renderer_toggle_set_active(_SELF(self), RTEST(setting));
    return setting;
}

void
Init_gtk_cellrenderertoggle()
{
    VALUE renderer = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_TOGGLE, "CellRendererToggle", mGtk);
    
    rb_define_method(renderer, "initialize", crtoggle_initialize, 0);
    rb_define_method(renderer, "radio?", crtoggle_get_radio, 0);
    rb_define_method(renderer, "set_radio", crtoggle_set_radio, 1);
    rb_define_method(renderer, "radio=", crtoggle_set_radio_equal, 1);
    rb_define_method(renderer, "active?", crtoggle_get_active, 0);
    rb_define_method(renderer, "set_active", crtoggle_set_active, 1);
    rb_define_method(renderer, "active=", crtoggle_set_active_equal, 1);
}


 
