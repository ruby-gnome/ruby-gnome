/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define _SELF(self) GTK_FILE_SELECTION(RVAL2GOBJ(self))

static VALUE
fsel_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_file_selection_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
fsel_complete(VALUE self, VALUE pattern)
{
    gtk_file_selection_complete(_SELF(self), RVAL2CSTR(pattern));
    return self;
}

static VALUE
fsel_show_fileop_buttons(VALUE self)
{
    gtk_file_selection_show_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
fsel_hide_fileop_buttons(VALUE self)
{
    gtk_file_selection_hide_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
fsel_get_selections(VALUE self)
{
    gchar** ptr = gtk_file_selection_get_selections(_SELF(self));
    gchar** selections = ptr;
    VALUE ary = rb_ary_new();
    while(*selections){
        rb_ary_push(ary, CSTR2RVAL(*selections));
        selections++;
    }
    if (ptr)
        g_strfreev(ptr);

    return ary;
}

static VALUE
fsel_fileop_dialog(VALUE self)
{
    GtkWidget* widget = _SELF(self)->fileop_dialog;
    return widget ? GOBJ2RVAL(widget) : Qnil;
}

static VALUE
fsel_dir_list(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->dir_list);
}

static VALUE
fsel_file_list(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->file_list);
}

static VALUE
fsel_ok_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
fsel_cancel_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
fsel_history_pulldown(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->history_pulldown);
}

static VALUE
fsel_fileop_c_dir(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_c_dir);
}

static VALUE
fsel_fileop_del_file(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_del_file);
}

static VALUE
fsel_fileop_ren_file(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_ren_file);
}

void 
Init_gtk_file_selection(void)
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
