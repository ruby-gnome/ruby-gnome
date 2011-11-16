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

#define RG_TARGET_NAMESPACE mEditable

static VALUE
rg_select_region(VALUE self, VALUE start, VALUE end)
{
    gtk_editable_select_region(GTK_EDITABLE(RVAL2GOBJ(self)),
                               NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
rg_selection_bounds(VALUE self)
{
    gint start, end;
    gboolean ret;
    ret = gtk_editable_get_selection_bounds(GTK_EDITABLE(RVAL2GOBJ(self)),
                                            &start, &end);
    return ret ? rb_ary_new3(2, INT2NUM(start), INT2NUM(end)) : Qnil;
}

static VALUE
rg_insert_text(VALUE self, VALUE new_text, VALUE pos)
{
    gint p = NUM2INT(pos);

    StringValue(new_text);
    gtk_editable_insert_text(GTK_EDITABLE(RVAL2GOBJ(self)),
                             RSTRING_PTR(new_text),
                             RSTRING_LEN(new_text),
                             &p);
    return INT2NUM(p);
}

static VALUE
rg_delete_text(VALUE self, VALUE start, VALUE end)
{
    gtk_editable_delete_text(GTK_EDITABLE(RVAL2GOBJ(self)),
                             NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
rg_get_chars(VALUE self, VALUE start, VALUE end)
{
    return CSTR2RVAL_FREE(
            gtk_editable_get_chars(GTK_EDITABLE(RVAL2GOBJ(self)),   /* check s */
                                   NUM2INT(start), NUM2INT(end)));  /* check start,end */
}

static VALUE
rg_delete_selection(VALUE self)
{
    gtk_editable_delete_selection(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_position(VALUE self)
{
    return INT2NUM(gtk_editable_get_position(GTK_EDITABLE(RVAL2GOBJ(self))));
}

static VALUE
rg_set_position(VALUE self, VALUE position)
{
    gtk_editable_set_position(GTK_EDITABLE(RVAL2GOBJ(self)),
                              NUM2INT(position));
    return self;
}

static VALUE
rg_set_editable(VALUE self, VALUE editable)
{
    gtk_editable_set_editable(GTK_EDITABLE(RVAL2GOBJ(self)), RVAL2CBOOL(editable));
    return self;
}

static VALUE
rg_editable_p(VALUE self)
{
    return CBOOL2RVAL(gtk_editable_get_editable(GTK_EDITABLE(RVAL2GOBJ(self))));
}

static VALUE
rg_cut_clipboard(VALUE self)
{
    gtk_editable_cut_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_copy_clipboard(VALUE self)
{
    gtk_editable_copy_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_paste_clipboard(VALUE self)
{
    gtk_editable_paste_clipboard(GTK_EDITABLE(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_editable(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_EDITABLE, "Editable", mGtk);

    RG_DEF_METHOD(select_region, 2);
    RG_DEF_METHOD(selection_bounds, 0);
    RG_DEF_METHOD(insert_text, 2);
    RG_DEF_METHOD(delete_text, 2);
    RG_DEF_METHOD(get_chars, 2);
    RG_DEF_METHOD(delete_selection, 0);
    RG_DEF_METHOD(position, 0);
    RG_DEF_METHOD(set_position, 1);
    RG_DEF_METHOD(set_editable, 1);
    RG_DEF_METHOD_P(editable, 0);
    RG_DEF_METHOD(copy_clipboard, 0);
    RG_DEF_METHOD(cut_clipboard, 0);
    RG_DEF_METHOD(paste_clipboard, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
