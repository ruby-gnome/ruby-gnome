/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkfilesel.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:12 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
fsel_initialize(self, title)
    VALUE self, title;
{
    set_widget(self, gtk_file_selection_new(STR2CSTR(title)));
    return Qnil;
}

static VALUE
fsel_set_fname(self, fname)
    VALUE self, fname;
{
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(get_widget(self)),
				    STR2CSTR(fname));
    return Qnil;
}

static VALUE
fsel_get_fname(self)
    VALUE self;
{
    return rb_str_new2(
            gtk_file_selection_get_filename(GTK_FILE_SELECTION(get_widget(self))));
}

static VALUE
fsel_complete(self, pattern)
    VALUE self, pattern;
{
    gtk_file_selection_complete(GTK_FILE_SELECTION(get_widget(self)), STR2CSTR(pattern));
    return self;
}

static VALUE
fsel_ok_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "ok_button");

    if (NIL_P(b)) {
	GtkWidget *w = GTK_FILE_SELECTION(get_widget(self))->ok_button;
	b = make_widget(gButton, w);
	rb_iv_set(self, "ok_button", b);
    }

    return b;
}

static VALUE
fsel_cancel_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "cancel_button");

    if (NIL_P(b)) {
	GtkWidget *w = GTK_FILE_SELECTION(get_widget(self))->cancel_button;
	b = make_widget(gButton, w);
	rb_iv_set(self, "cancel_button", b);
    }

    return b;
}

static VALUE
fsel_action_area(self)
    VALUE self;
{
    VALUE area = rb_iv_get(self, "action_area");

    if (NIL_P(area)) {
	GtkWidget *w = GTK_FILE_SELECTION(get_widget(self))->action_area;
	area = make_widget(gHBox, w);
	rb_iv_set(self, "action_area", area);
    }

    return area;
}

static VALUE
fsel_dir_list(self)
    VALUE self;
{
    GtkObject *obj = GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->dir_list);
    return get_value_from_gobject(obj);
}

static VALUE
fsel_file_list(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->file_list));
}

static VALUE
fsel_history_pulldown(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->history_pulldown));
}

static VALUE
fsel_fileop_c_dir(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->fileop_c_dir));
}

static VALUE
fsel_fileop_del_file(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->fileop_del_file));
}

static VALUE
fsel_fileop_ren_file(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GTK_FILE_SELECTION(get_widget(self))->fileop_ren_file));
}

static VALUE
fsel_show_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_show_fileop_buttons(GTK_FILE_SELECTION(get_widget(self)));
    return Qnil;
}

static VALUE
fsel_hide_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(get_widget(self)));
    return Qnil;
}

void Init_gtk_file_selection()
{
    gFileSel = rb_define_class_under(mGtk, "FileSelection", gWindow);

    rb_define_method(gFileSel, "initialize", fsel_initialize, 1);
    rb_define_method(gFileSel, "set_filename", fsel_set_fname, 1);
    rb_define_alias(gFileSel, "filename=", "set_filename");
    rb_define_method(gFileSel, "get_filename", fsel_get_fname, 0);
    rb_define_alias(gFileSel, "filename", "get_filename");
    rb_define_method(gFileSel, "complete", fsel_complete, 1);
    rb_define_method(gFileSel, "ok_button", fsel_ok_button, 0);
    rb_define_method(gFileSel, "cancel_button", fsel_cancel_button, 0);
    rb_define_method(gFileSel, "action_area", fsel_action_area, 0);
    rb_define_method(gFileSel, "dir_list", fsel_dir_list, 0);
    rb_define_method(gFileSel, "file_list", fsel_file_list, 0);
    rb_define_method(gFileSel, "history_pulldown", fsel_history_pulldown, 0);
    rb_define_method(gFileSel, "fileop_c_dir", fsel_fileop_c_dir, 0);
    rb_define_method(gFileSel, "fileop_del_file", fsel_fileop_del_file, 0);
    rb_define_method(gFileSel, "fileop_ren_file", fsel_fileop_ren_file, 0);
    rb_define_method(gFileSel, "show_fileop_buttons", fsel_show_fileop_buttons, 0);
    rb_define_method(gFileSel, "hide_fileop_buttons", fsel_hide_fileop_buttons, 0);
}
