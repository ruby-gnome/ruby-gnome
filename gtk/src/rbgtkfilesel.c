/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfilesel.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
fsel_initialize(self, title)
    VALUE self, title;
{
    RBGTK_INITIALIZE(self, gtk_file_selection_new(STR2CSTR(title)));
    return Qnil;
}

static VALUE
fsel_set_fname(self, fname)
    VALUE self, fname;
{
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(RVAL2GOBJ(self)),
									STR2CSTR(fname));
    return Qnil;
}

static VALUE
fsel_get_fname(self)
    VALUE self;
{
    return rb_str_new2(
		gtk_file_selection_get_filename(GTK_FILE_SELECTION(RVAL2GOBJ(self))));
}

static VALUE
fsel_complete(self, pattern)
    VALUE self, pattern;
{
    gtk_file_selection_complete(GTK_FILE_SELECTION(RVAL2GOBJ(self)), STR2CSTR(pattern));
    return self;
}

static VALUE
fsel_ok_button(self)
    VALUE self;
{
    VALUE b = rb_iv_get(self, "ok_button");

    if (NIL_P(b)) {
		GtkWidget *w = GTK_FILE_SELECTION(RVAL2GOBJ(self))->ok_button;
		b = GOBJ2RVAL(w);
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
		GtkWidget *w = GTK_FILE_SELECTION(RVAL2GOBJ(self))->cancel_button;
		b = GOBJ2RVAL(w);
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
		GtkWidget *w = GTK_FILE_SELECTION(RVAL2GOBJ(self))->action_area;
		area = GOBJ2RVAL(w);
		rb_iv_set(self, "action_area", area);
    }

    return area;
}

static VALUE
fsel_dir_list(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->dir_list);
}

static VALUE
fsel_file_list(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->file_list);
}

static VALUE
fsel_history_pulldown(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->history_pulldown);
}

static VALUE
fsel_fileop_c_dir(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->fileop_c_dir);
}

static VALUE
fsel_fileop_del_file(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->fileop_del_file);
}

static VALUE
fsel_fileop_ren_file(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_FILE_SELECTION(RVAL2GOBJ(self))->fileop_ren_file);
}

static VALUE
fsel_show_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_show_fileop_buttons(GTK_FILE_SELECTION(RVAL2GOBJ(self)));
    return Qnil;
}

static VALUE
fsel_hide_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(RVAL2GOBJ(self)));
    return Qnil;
}

void 
Init_gtk_file_selection()
{
    VALUE gFileSel = G_DEF_CLASS(GTK_TYPE_FILE_SELECTION, "FileSelection", mGtk);
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
