/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkspinner.c -

  $Author$
  $Date$

  Copyright (C) 2011 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,20,0)

#define _SELF(s) (GTK_SPINNER(RVAL2GOBJ(s)))

static VALUE
spinner_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_spinner_new());
    return Qnil;
}

static VALUE
spinner_start(VALUE self)
{
    gtk_spinner_start(_SELF(self));
    return self;
}

static VALUE
spinner_stop(VALUE self)
{
    gtk_spinner_stop(_SELF(self));
    return self;
}

#endif

void 
Init_gtk_spinner()
{
#if GTK_CHECK_VERSION(2,20,0)
    VALUE gSpinner = G_DEF_CLASS(GTK_TYPE_SPINNER, "Spinner", mGtk);

    rb_define_method(gSpinner, "initialize", spinner_initialize, 0);
    rb_define_method(gSpinner, "start", spinner_start, 0);
    rb_define_method(gSpinner, "stop", spinner_stop, 0);
#endif
}
