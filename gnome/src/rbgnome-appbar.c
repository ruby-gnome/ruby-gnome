/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-appbar.c,v 1.4 2002/10/17 14:34:39 tkubo Exp $ */
/* base on libgnomeui/gnome-appbar.h */

/* Gnome::AppBar widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_APPBAR(RVAL2GOBJ(self))

static VALUE
appbar_initialize(self, has_progress, has_status, interactivity)
    VALUE self, has_progress, has_status, interactivity;
{
    GtkWidget *appbar;
    appbar = gnome_appbar_new(RTEST(has_progress),
                              RTEST(has_status),
                              NUM2INT(interactivity));
	RBGTK_INITIALIZE(self, appbar);
    return Qnil;
}

/* Sets the status label without changing widget state; next set or push
   will destroy this permanently. */
static VALUE
appbar_set_status(self, status)
    VALUE self, status;
{
    gnome_appbar_set_status(_SELF(self),
                            RVAL2CSTR(status));
    return self;
}

/* get the statusbar */
static VALUE
appbar_get_status(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_appbar_get_status(_SELF(self)));
}

/* What to show when showing nothing else; defaults to nothing */
static VALUE
appbar_set_default(self, default_status)
    VALUE self, default_status;
{
    gnome_appbar_set_default(_SELF(self), RVAL2CSTR(default_status));
    return self;
}

static VALUE
appbar_push(self, status)
    VALUE self, status;
{
    gnome_appbar_push(_SELF(self), RVAL2CSTR(status));
    return self;
}

/* OK to call on empty stack */
static VALUE
appbar_pop(self)
    VALUE self;
{
    gnome_appbar_pop(_SELF(self));
    return self;
}

/* Nuke the stack. */
static VALUE
appbar_clear_stack(self)
    VALUE self;
{
    gnome_appbar_clear_stack(_SELF(self));
    return self;
}

/* pure sugar - with a bad name, in light of the get_progress name
   which is not the opposite of set_progress. Maybe this function
   should die.*/
static VALUE
appbar_set_progress_percentage(self, percentage)
    VALUE self, percentage;
{
    gnome_appbar_set_progress_percentage(_SELF(self), (gfloat)NUM2DBL(percentage));
    return self;
}

/* use GtkProgress functions on returned value */
static VALUE
appbar_get_progress(self)
    VALUE self;
{
    GtkProgressBar* progress;
    progress = gnome_appbar_get_progress(_SELF(self));
    return GOBJ2RVAL(progress);
}

/* Reflect the current state of stack/default. Useful to force a set_status
   to disappear. */
static VALUE
appbar_refresh(self)
    VALUE self;
{
    gnome_appbar_refresh(_SELF(self));
    return self;
}

/* Put a prompt in the appbar and wait for a response. When the 
   user responds or cancels, a user_response signal is emitted. */
static VALUE
appbar_set_prompt(self, prompt, modal)
    VALUE self, prompt, modal;
{
    gnome_appbar_set_prompt(_SELF(self),
                            RVAL2CSTR(prompt),
                            RTEST(modal));
    return self;
}

/* Remove any prompt */
static VALUE
appbar_clear_prompt(self)
    VALUE self;
{
    gnome_appbar_clear_prompt(_SELF(self));
    return self;
}

/* Get the response to the prompt, if any. Result must be g_free'd. */
static VALUE
appbar_get_response(self)
    VALUE self;
{
    gchar *response;
    VALUE obj;
    response = gnome_appbar_get_response(_SELF(self));
    obj = rb_str_new2(response);
    g_free(response);
    return obj;
}

void
Init_gnome_appbar(mGnome)
    VALUE mGnome;
{
    VALUE gnoAppBar = G_DEF_CLASS(GNOME_TYPE_APPBAR, "AppBar", mGnome);

    /* Instance methods */
    rb_define_method(gnoAppBar, "initialize", appbar_initialize, 3);
    rb_define_method(gnoAppBar, "set_status", appbar_set_status, 1);
    rb_define_method(gnoAppBar, "status", appbar_get_status, 0);
    rb_define_method(gnoAppBar, "set_default", appbar_set_default, 1);
    rb_define_method(gnoAppBar, "push", appbar_push, 1);
    rb_define_method(gnoAppBar, "pop", appbar_pop, 0);
    rb_define_method(gnoAppBar, "clear_stack", appbar_clear_stack, 0);
    rb_define_method(gnoAppBar, "set_progress_percentage", appbar_set_progress_percentage, 1);
    rb_define_method(gnoAppBar, "get_progress", appbar_get_progress, 0);
    rb_define_method(gnoAppBar, "refresh", appbar_refresh, 0);
    rb_define_method(gnoAppBar, "set_prompt", appbar_set_prompt, 2);
    rb_define_method(gnoAppBar, "clear_prompt", appbar_clear_prompt, 0);
    rb_define_method(gnoAppBar, "get_response", appbar_get_response, 0);

    G_DEF_SETTERS(gnoAppBar);
}
