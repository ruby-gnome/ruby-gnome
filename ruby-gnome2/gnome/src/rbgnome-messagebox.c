/* $Id: rbgnome-messagebox.c,v 1.1 2002/05/19 12:29:54 mutoh Exp $ */

/* Gnome::MessageBox widget for Ruby/Gnome
 * Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
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

VALUE gnoMessageBox;

static VALUE
mesbox_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE message, messagebox_type, buttons;
    const gchar **c_buttons;
    int i;
    GtkWidget* message_box;

    rb_scan_args(argc, argv, "2*", &message, &messagebox_type, &buttons);

    c_buttons = ALLOCA_N(const gchar*, RARRAY(buttons)->len+1);
    for (i=0; i<RARRAY(buttons)->len; i++) {
	c_buttons[i] = STR2CSTR(RARRAY(buttons)->ptr[i]);
    }
    c_buttons[i] = 0;

    message_box = gnome_message_box_newv(STR2CSTR(message),
					 STR2CSTR(messagebox_type),
					 c_buttons);
    set_widget(self, message_box);

    return Qnil;
}

void
Init_gnome_messagebox()
{
    gnoMessageBox = rb_define_class_under(mGnome, "MessageBox", gnoDialog);

    /* Constants */
    rb_define_const(gnoMessageBox, "INFO",
		    rb_str_new2(GNOME_MESSAGE_BOX_INFO));
    rb_define_const(gnoMessageBox, "WARNING",
		    rb_str_new2(GNOME_MESSAGE_BOX_WARNING));
    rb_define_const(gnoMessageBox, "ERROR",
		    rb_str_new2(GNOME_MESSAGE_BOX_ERROR));
    rb_define_const(gnoMessageBox, "QUESTION",
		    rb_str_new2(GNOME_MESSAGE_BOX_QUESTION));
    rb_define_const(gnoMessageBox, "GENERIC",
		    rb_str_new2(GNOME_MESSAGE_BOX_GENERIC));

    /* Instance methods */
    rb_define_method(gnoMessageBox, "initialize", mesbox_initialize, -1);
}
