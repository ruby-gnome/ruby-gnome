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

#define RG_TARGET_NAMESPACE cFileSelection
#define _SELF(self) GTK_FILE_SELECTION(RVAL2GOBJ(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_file_selection_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
rg_complete(VALUE self, VALUE pattern)
{
    gtk_file_selection_complete(_SELF(self), RVAL2CSTR(pattern));
    return self;
}

static VALUE
rg_show_fileop_buttons(VALUE self)
{
    gtk_file_selection_show_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
rg_hide_fileop_buttons(VALUE self)
{
    gtk_file_selection_hide_fileop_buttons(_SELF(self));
    return Qnil;
}

static VALUE
rg_selections(VALUE self)
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
rg_fileop_dialog(VALUE self)
{
    GtkWidget* widget = _SELF(self)->fileop_dialog;
    return widget ? GOBJ2RVAL(widget) : Qnil;
}

static VALUE
rg_dir_list(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->dir_list);
}

static VALUE
rg_file_list(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->file_list);
}

static VALUE
rg_ok_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
rg_cancel_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
rg_history_pulldown(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->history_pulldown);
}

static VALUE
rg_fileop_c_dir(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_c_dir);
}

static VALUE
rg_fileop_del_file(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_del_file);
}

static VALUE
rg_fileop_ren_file(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fileop_ren_file);
}

void 
Init_gtk_file_selection(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FILE_SELECTION, "FileSelection", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(complete, 1);
    RG_DEF_METHOD(show_fileop_buttons, 0);
    RG_DEF_METHOD(hide_fileop_buttons, 0);
    RG_DEF_METHOD(selections, 0);
    RG_DEF_METHOD(fileop_dialog, 0);
    RG_DEF_METHOD(ok_button, 0);
    RG_DEF_METHOD(cancel_button, 0);
    RG_DEF_METHOD(dir_list, 0);
    RG_DEF_METHOD(file_list, 0);
    RG_DEF_METHOD(history_pulldown, 0);
    RG_DEF_METHOD(fileop_c_dir, 0);
    RG_DEF_METHOD(fileop_del_file, 0);
    RG_DEF_METHOD(fileop_ren_file, 0);
}
