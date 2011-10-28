/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#if GTK_CHECK_VERSION(2,20,0)

#define RG_TARGET_NAMESPACE cSpinner
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
Init_gtk_spinner(void)
{
#if GTK_CHECK_VERSION(2,20,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SPINNER, "Spinner", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", spinner_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "start", spinner_start, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "stop", spinner_stop, 0);
#endif
}
