/* $Id: rbgnome-dialog.c,v 1.1 2002/05/19 12:29:53 mutoh Exp $ */

/* Gnome::Dialog widget for Ruby/Gnome
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

VALUE gnoDialog;

static VALUE
dialog_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE title, buttons;
    const gchar **c_buttons;
    int i;
    GtkWidget* dialog;

    rb_scan_args(argc, argv, "1*", &title, &buttons);

    c_buttons = ALLOCA_N(const gchar*, RARRAY(buttons)->len+1);
    for (i=0; i<RARRAY(buttons)->len; i++) {
	c_buttons[i] = STR2CSTR(RARRAY(buttons)->ptr[i]);
    }
    c_buttons[i] = 0;

    dialog = gnome_dialog_newv(STR2CSTR(title), c_buttons);

    set_widget(self, dialog);

    return Qnil;
}

/*
 * Gnome::Dialog::set_parent(parent)
 *
 * For now this just means the dialog can be centered over
 * its parent.
 */
static VALUE
dialog_set_parent(self, parent)
     VALUE self, parent;
{
    gnome_dialog_set_parent(GNOME_DIALOG(get_widget(self)),
			    GTK_WINDOW(get_widget(parent)));
    return self;
}


static VALUE
dialog_button_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE button, data, args;
    ID id = 0;
    GList *list;

    rb_scan_args(argc, argv, "1*", &button, &args);

    list = g_list_nth(GNOME_DIALOG(get_widget(self))->buttons,
		      NUM2INT(button));

    if (list == 0 || list->data == 0) {
	rb_raise(rb_eArgError, "Button number %d does not appear to exist\n",
		 NUM2INT(button));
    }

    id = rb_intern("clicked");
    data = rb_ary_new3(3, rb_f_lambda(), INT2NUM(id), args);
    add_relative(self, data);
    gtk_signal_connect_full(get_gobject(self),
			    "clicked", 0,
			    (GtkSignalFunc)signal_callback,
			    (gpointer)data,
			    0, FALSE, 0);
    return self;
}

static VALUE
dialog_run(self)
    VALUE self;
{
    gint button;
    button = gnome_dialog_run(GNOME_DIALOG(get_widget(self)));
    return INT2NUM(button);
}

static VALUE
dialog_run_and_close(self)
    VALUE self;
{
    gint button;
    button = gnome_dialog_run_and_close(GNOME_DIALOG(get_widget(self)));
    return INT2NUM(button);
}

static VALUE
dialog_set_default(self, button)
    VALUE self, button;
{
    gnome_dialog_set_default(GNOME_DIALOG(get_widget(self)),
			     NUM2INT(button));
    return self;
}

static VALUE
dialog_set_sensitive(self, button, setting)
    VALUE self, button, setting;
{
    gnome_dialog_set_sensitive(GNOME_DIALOG(get_widget(self)),
			       NUM2INT(button),
			       RTEST(setting));
    return self;
}

static VALUE
dialog_set_accelerator(self, button, accelerator_key, accelerator_mods)
    VALUE self, button, accelerator_key, accelerator_mods;
{
    gnome_dialog_set_accelerator(GNOME_DIALOG(get_widget(self)),
				 NUM2INT(button),
				 (guchar)NUM2INT(accelerator_key),
				 (guint8)NUM2INT(accelerator_mods));
    return self;
}

static VALUE
dialog_close(self)
    VALUE self;
{
    gnome_dialog_close(GNOME_DIALOG(get_widget(self)));
    return self;
}

static VALUE
dialog_close_hides(self, just_hide)
    VALUE self, just_hide;
{
    gnome_dialog_close_hides(GNOME_DIALOG(get_widget(self)),
			     RTEST(just_hide));
    return self;
}

static VALUE
dialog_set_close(self, click_closes)
    VALUE self, click_closes;
{
    gnome_dialog_set_close(GNOME_DIALOG(get_widget(self)),
			   RTEST(click_closes));
    return self;
}

static VALUE
dialog_editable_enters(self, editable)
    VALUE self, editable;
{
    gnome_dialog_editable_enters(GNOME_DIALOG(get_widget(self)),
				 GTK_EDITABLE(get_widget(editable)));
    return self;
}

static VALUE
dialog_append_buttons(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int i;
    const gchar **buttons;
#if 0
    if (argc < 1) {
	rb_raise();
    }
#endif
    buttons = ALLOCA_N(const gchar*, argc+1);
    for (i=0; i<argc; i++) {
	buttons[i] = STR2CSTR(argv[i]);
    }
    buttons[i] = 0;
    gnome_dialog_append_buttonsv(GNOME_DIALOG(get_widget(self)),
				 buttons);
    return self;
}

static VALUE
dialog_append_button(self, name)
    VALUE self, name;
{
    gnome_dialog_append_button(GNOME_DIALOG(get_widget(self)),
			       STR2CSTR(name));
    return self;
}

static VALUE
dialog_append_button_with_pixmap(self, name, pixmap)
    VALUE self, name, pixmap;
{
    gnome_dialog_append_button_with_pixmap(GNOME_DIALOG(get_widget(self)),
					   STR2CSTR(name),
					   STR2CSTR(pixmap));
    return self;
}

static VALUE
dialog_append_buttons_with_pixmaps(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int i;
    const gchar **c_buttons;
    const gchar **c_pixmaps;

    c_buttons = ALLOCA_N(const gchar*, argc+1);
    c_pixmaps = ALLOCA_N(const gchar*, argc+1);
    for (i=0; i<argc; i++) {
	c_buttons[i] = STR2CSTR(RARRAY(argv[i])->ptr[0]);
	c_pixmaps[i] = STR2CSTR(RARRAY(argv[i])->ptr[1]);
    }
    c_buttons[i] = 0;
    c_pixmaps[i] = 0;
    gnome_dialog_append_buttons_with_pixmaps(GNOME_DIALOG(get_widget(self)),
					     c_buttons,
					     c_pixmaps);
    return self;
}

static VALUE
dialog_get_vbox(self)
    VALUE self;
{
    GtkObject *vbox;
    vbox = GTK_OBJECT(GNOME_DIALOG(get_widget(self))->vbox);
    return make_gnobject_auto_type(vbox);
}

void
Init_gnome_dialog()
{
    gnoDialog = rb_define_class_under(mGnome, "Dialog", gWindow);

    /*
     * instance methods
     */
    rb_define_method(gnoDialog, "initialize", dialog_initialize, -1);
    rb_define_method(gnoDialog, "set_parent", dialog_set_parent, 1);
    rb_define_method(gnoDialog, "button_connect", dialog_button_connect, -1);
    rb_define_method(gnoDialog, "run", dialog_run, 0);
    rb_define_method(gnoDialog, "run_and_close", dialog_run_and_close, 0);
    rb_define_method(gnoDialog, "set_default", dialog_set_default, 1);
    rb_define_method(gnoDialog, "set_sensitive", dialog_set_sensitive, 2);
    rb_define_method(gnoDialog, "set_accelerator", dialog_set_accelerator, 3);
    rb_define_method(gnoDialog, "close", dialog_close, 0);
    rb_define_method(gnoDialog, "close_hides", dialog_close_hides, 1);
    rb_define_method(gnoDialog, "set_close", dialog_set_close, 1);
    rb_define_method(gnoDialog, "editable_enters", dialog_editable_enters, 1);
    rb_define_method(gnoDialog, "append_buttons", dialog_append_buttons, -1);
    rb_define_method(gnoDialog, "append_button", dialog_append_button, 1);
    rb_define_method(gnoDialog, "append_button_with_pixmap",
		     dialog_append_button_with_pixmap, 2);
    rb_define_method(gnoDialog, "append_buttons_with_pixmaps",
		     dialog_append_buttons_with_pixmaps, -1);
    rb_define_method(gnoDialog, "vbox", dialog_get_vbox, 0);

    /* Signals */
    rb_define_const(gnoDialog, "SIGNAL_CLOSE", rb_str_new2("close"));
    rb_define_const(gnoDialog, "SIGNAL_CLICKED", rb_str_new2("clicked"));    

    /* child init */
    Init_gnome_about();
    Init_gnome_property_box();
}
