/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfilesel.c -

  $Author: mutoh $
  $Date: 2003/06/04 16:55:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) GTK_FILE_SELECTION(RVAL2GOBJ(self))

static VALUE
fsel_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_file_selection_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
fsel_complete(self, pattern)
    VALUE self, pattern;
{
    gtk_file_selection_complete(_SELF(self), RVAL2CSTR(pattern));
    return self;
}

static VALUE
fsel_show_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_show_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
fsel_hide_fileop_buttons(self)
    VALUE self;
{
    gtk_file_selection_hide_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
fsel_get_selections(self)
    VALUE self;
{
    gchar** ptr = gtk_file_selection_get_selections(_SELF(self));
    gchar** selections = ptr;
    VALUE ary = rb_ary_new();
    while(*selections){
        rb_ary_push(ary, CSTR2RVAL(*selections));
        *selections++;
    }
    if (ptr)
        g_strfreev(ptr);

    return ary;
}

static VALUE
fsel_fileop_dialog(self)
    VALUE self;
{
    GtkWidget* widget = _SELF(self)->fileop_dialog;
    return widget ? GOBJ2RVAL(widget) : Qnil;
}

static VALUE
fsel_dir_list(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->dir_list);
}

static VALUE
fsel_file_list(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->file_list);
}

static VALUE
fsel_ok_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
fsel_cancel_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
fsel_history_pulldown(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->history_pulldown);
}

static VALUE
fsel_fileop_c_dir(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->fileop_c_dir);
}

static VALUE
fsel_fileop_del_file(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->fileop_del_file);
}

static VALUE
fsel_fileop_ren_file(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->fileop_ren_file);
}

void 
Init_gtk_file_selection()
{
    VALUE gFileSel = G_DEF_CLASS(GTK_TYPE_FILE_SELECTION, "FileSelection", mGtk);

    rb_define_method(gFileSel, "initialize", fsel_initialize, -1);
    rb_define_method(gFileSel, "complete", fsel_complete, 1);
    rb_define_method(gFileSel, "show_fileop_buttons", fsel_show_fileop_buttons, 0);
    rb_define_method(gFileSel, "hide_fileop_buttons", fsel_hide_fileop_buttons, 0);
    rb_define_method(gFileSel, "selections", fsel_get_selections, 0);
    rb_define_method(gFileSel, "fileop_dialog", fsel_fileop_dialog, 0);
    rb_define_method(gFileSel, "ok_button", fsel_ok_button, 0);
    rb_define_method(gFileSel, "cancel_button", fsel_cancel_button, 0);
    rb_define_method(gFileSel, "dir_list", fsel_dir_list, 0);
    rb_define_method(gFileSel, "file_list", fsel_file_list, 0);
    rb_define_method(gFileSel, "history_pulldown", fsel_history_pulldown, 0);
    rb_define_method(gFileSel, "fileop_c_dir", fsel_fileop_c_dir, 0);
    rb_define_method(gFileSel, "fileop_del_file", fsel_fileop_del_file, 0);
    rb_define_method(gFileSel, "fileop_ren_file", fsel_fileop_ren_file, 0);
}
