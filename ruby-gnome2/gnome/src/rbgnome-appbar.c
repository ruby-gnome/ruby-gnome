/* $Id: rbgnome-appbar.c,v 1.1 2002/05/19 12:29:56 mutoh Exp $ */

/* Gnome::AppBar widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
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

VALUE gnoAppBar;

static VALUE
appbar_initialize(self, has_progress, has_status, interactivity)
    VALUE self, has_progress, has_status, interactivity;
{
    GtkWidget *appbar;
    appbar = gnome_appbar_new(RTEST(has_progress),
			      RTEST(has_status),
			      NUM2INT(interactivity));
	set_widget(self, appbar);
    return Qnil;
}

/* Sets the status label without changing widget state; next set or push
   will destroy this permanently. */
static VALUE
appbar_set_status(self, status)
    VALUE self, status;
{
    gnome_appbar_set_status(GNOME_APPBAR(get_widget(self)),
			    STR2CSTR(status));
    return self;
}

/* What to show when showing nothing else; defaults to nothing */
static VALUE
appbar_set_default(self, default_status)
    VALUE self, default_status;
{
    gnome_appbar_set_default(GNOME_APPBAR(get_widget(self)),
			     STR2CSTR(default_status));
    return self;
}

static VALUE
appbar_push(self, status)
    VALUE self, status;
{
    gnome_appbar_push(GNOME_APPBAR(get_widget(self)),
		      STR2CSTR(status));
    return self;
}

/* OK to call on empty stack */
static VALUE
appbar_pop(self)
    VALUE self;
{
    gnome_appbar_pop(GNOME_APPBAR(get_widget(self)));
    return self;
}

/* Nuke the stack. */
static VALUE
appbar_clear_stack(self)
    VALUE self;
{
    gnome_appbar_clear_stack(GNOME_APPBAR(get_widget(self)));
    return self;
}

/* pure sugar - with a bad name, in light of the get_progress name
   which is not the opposite of set_progress. Maybe this function
   should die.*/
static VALUE
appbar_set_progress(self, percentage)
    VALUE self, percentage;
{
    gnome_appbar_set_progress(GNOME_APPBAR(get_widget(self)),
			      (gfloat)NUM2DBL(percentage));
    return self;
}

/* use GtkProgress functions on returned value */
static VALUE
appbar_get_progress(self)
    VALUE self;
{
    GtkProgress* progress;
    progress = gnome_appbar_get_progress(GNOME_APPBAR(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(progress));
}

/* Reflect the current state of stack/default. Useful to force a set_status
   to disappear. */
static VALUE
appbar_refresh(self)
    VALUE self;
{
    gnome_appbar_refresh(GNOME_APPBAR(get_widget(self)));
    return self;
}

/* Put a prompt in the appbar and wait for a response. When the 
   user responds or cancels, a user_response signal is emitted. */
static VALUE
appbar_set_prompt(self, prompt, modal)
    VALUE self, prompt, modal;
{
    gnome_appbar_set_prompt(GNOME_APPBAR(get_widget(self)),
			    STR2CSTR(prompt),
			    RTEST(modal));
    return self;
}

/* Remove any prompt */
static VALUE
appbar_clear_prompt(self)
    VALUE self;
{
    gnome_appbar_clear_prompt(GNOME_APPBAR(get_widget(self)));
    return self;
}

/* Get the response to the prompt, if any. Result must be g_free'd. */
static VALUE
appbar_get_response(self)
    VALUE self;
{
    gchar *response;
    response = gnome_appbar_get_response(GNOME_APPBAR(get_widget(self)));
    return rb_str_new2(response);
}

void
Init_gnome_appbar()
{
    gnoAppBar = rb_define_class_under(mGnome, "AppBar", gHBox);

    /* Instance methods */
    rb_define_method(gnoAppBar, "initialize", appbar_initialize, 3);
    rb_define_method(gnoAppBar, "set_status", appbar_set_status, 1);
    rb_define_method(gnoAppBar, "set_default", appbar_set_default, 1);
    rb_define_method(gnoAppBar, "push", appbar_push, 1);
    rb_define_method(gnoAppBar, "pop", appbar_pop, 0);
    rb_define_method(gnoAppBar, "clear_stack", appbar_clear_stack, 0);
    rb_define_method(gnoAppBar, "set_progress", appbar_set_progress, 1);
    rb_define_method(gnoAppBar, "get_progress", appbar_get_progress, 0);
    rb_define_method(gnoAppBar, "refresh", appbar_refresh, 0);
    rb_define_method(gnoAppBar, "set_prompt", appbar_set_prompt, 2);
    rb_define_method(gnoAppBar, "clear_prompt", appbar_clear_prompt, 0);
    rb_define_method(gnoAppBar, "get_response", appbar_get_response, 0);

    /* Signals */
    rb_define_const(gnoAppBar, "SIGNAL_USER_RESPONSE",
		    rb_str_new2("user_response"));
    rb_define_const(gnoAppBar, "SIGNAL_CLEAR_PROMPT",
		    rb_str_new2("clear_prompt"));
}
