/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cSelectionData
#define _SELF(d) (RVAL2GTKSELECTIONDATA(d))

static GdkAtom compound_text;

/************************************************************************/
void
rbgtk_atom2selectiondata(VALUE type, VALUE size, VALUE src, GdkAtom *gtype,
                         void **data, gint *format, gint *length)
{
    void* dat = NULL;
    gint fmt, len;
    GdkAtom ntype = RVAL2ATOM(type);

    if(ntype == GDK_SELECTION_TYPE_INTEGER){
        glong *i;
        i = ALLOC(glong);
        *i = NUM2INT(src);
        dat = i;
        fmt = 32;
        len = 1;
    } else if(ntype == GDK_SELECTION_TYPE_STRING) {
        dat = (void *)RVAL2CSTR(src);
        fmt = 8;
        if (NIL_P(size)) {
            len = RSTRING_LEN(src);
        } else {
            len = NUM2UINT(size);
        }
/* TODO
    } else if(ntype == compound_text){
        guchar* str = (guchar*)dat;
        gdk_string_to_compound_text(RVAL2CSTR(src), &ntype, &fmt, &str, &len);
*/
    } else if(type != Qnil && size != Qnil && src != Qnil) {
        dat = (void *)RVAL2CSTR(src);
        fmt = NUM2INT(size);
        len = (RSTRING_LEN(src) * sizeof(char) / fmt);
    } else {
        rb_raise(rb_eArgError, "no supported type.");
    }

    *gtype = ntype;
    *data = dat;
    *format = fmt;
    *length = len;
}

void
rbgtk_atom2selectiondata_free(GdkAtom type, void *dat)
{
    if (type == GDK_SELECTION_TYPE_INTEGER ||
        type == GDK_SELECTION_TYPE_ATOM) {
        xfree(dat);
/* TODO
    } else if(type == compound_text) {
        gdk_free_compound_text(dat);
*/
    }
}    
/************************************************************************/

static VALUE
rg_selection(VALUE self)
{
    return GDKATOM2RVAL(gtk_selection_data_get_selection(_SELF(self)));
}

static VALUE
rg_target(VALUE self)
{
    return GDKATOM2RVAL(gtk_selection_data_get_target(_SELF(self)));
}

static VALUE
rg_data_type(VALUE self)
{
    return GDKATOM2RVAL(gtk_selection_data_get_data_type(_SELF(self)));
}

static VALUE
rg_format(VALUE self)
{
    return INT2NUM(gtk_selection_data_get_format(_SELF(self)));
}

static VALUE
rg_data(VALUE self)
{
    return RBG_STRING_SET_UTF8_ENCODING(rb_str_new((const gchar *)gtk_selection_data_get_data(_SELF(self)),
                                                   gtk_selection_data_get_length(_SELF(self))));
}

static VALUE
rg_display(VALUE self)
{
    return GOBJ2RVAL(gtk_selection_data_get_display(_SELF(self)));
}

static VALUE
rg_set(int argc, VALUE *argv, VALUE self)
{
    void* dat;
    VALUE type, src;
    gint fmt, len;
    GdkAtom ntype;
    VALUE size = Qnil;

    if(3 == argc)
        rb_scan_args(argc, argv, "30", &type, &size, &src);
    else
        rb_scan_args(argc, argv, "20", &type, &src);

    rbgtk_atom2selectiondata(type, size, src, &ntype, &dat, &fmt, &len);

    gtk_selection_data_set(_SELF(self), ntype, fmt, dat, len);

    rbgtk_atom2selectiondata_free(ntype, dat);

    return self;
}

static VALUE
rg_set_text(VALUE self, VALUE str)
{
    StringValue(str);

    if (!gtk_selection_data_set_text(_SELF(self), RSTRING_PTR(str), RSTRING_LEN(str)))
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully set");

    return self;
}

static VALUE
rg_text(VALUE self)
{
    return CSTR2RVAL_FREE((gchar *)gtk_selection_data_get_text(_SELF(self)));
}

static VALUE
rg_set_pixbuf(VALUE self, VALUE pixbuf)
{
    if (!gtk_selection_data_set_pixbuf(_SELF(self), RVAL2GDKPIXBUF(pixbuf)))
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully set");

    return self;
}

static VALUE
rg_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gtk_selection_data_get_pixbuf(_SELF(self)));
}

static VALUE
rg_set_uris(VALUE self, VALUE uris)
{
    gboolean ret;
    gchar **guris;

    guris = (gchar **)RVAL2STRV(uris);
    ret = gtk_selection_data_set_uris(_SELF(self), guris);
    g_free(guris);
    if (!ret) 
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully.");

    return self;
}

static VALUE
rg_uris(VALUE self)
{
    VALUE ary = rb_ary_new();
    gchar** uris = gtk_selection_data_get_uris(_SELF(self));

    if (uris) {
        gchar **uri = uris;
        while (*uri) {
            rb_ary_push(ary, CSTR2RVAL(*uri));
            uri++;
        }
        g_strfreev(uris);
    }
    return ary;
}

static VALUE
rg_targets(VALUE self)
{
    GdkAtom* targets;
    gint n_atoms;
    VALUE result = Qnil;
    gboolean ret = gtk_selection_data_get_targets(_SELF(self),
                                                  &targets, &n_atoms);
    if (ret) {
        int i;
        result = rb_ary_new2(n_atoms);
        for (i = 0; i < n_atoms; i++){
            rb_ary_push(result, GDKATOM2RVAL(targets[i]));
        }
    }
    g_free(targets);
    return result;
}

static VALUE
rg_targets_include_image(VALUE self, VALUE writable)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_image(_SELF(self), RVAL2CBOOL(writable)));
}

static VALUE
rg_targets_include_text(VALUE self)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_text(_SELF(self)));
}

static VALUE
rg_targets_include_uri(VALUE self)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_uri(_SELF(self)));
}

static VALUE
rg_targets_include_rich_text(VALUE self, VALUE buffer)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_rich_text(_SELF(self),
                                                                   RVAL2GTKTEXTBUFFER(buffer)));
}

void
Init_gtk_selectiondata(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SELECTION_DATA, "SelectionData", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);

    compound_text = gdk_atom_intern("COMPOUND_TEXT", FALSE);

    RG_DEF_METHOD(selection, 0);
    RG_DEF_METHOD(target, 0);
    RG_DEF_METHOD(data_type, 0);
    RG_DEF_METHOD(format, 0);
    RG_DEF_METHOD(data, 0);
    RG_DEF_METHOD(display, 0);
    RG_DEF_METHOD(set, -1);
    RG_DEF_METHOD(text, 0);
    RG_DEF_METHOD(set_text, 1);
    RG_DEF_METHOD(pixbuf, 0);
    RG_DEF_METHOD(set_pixbuf, 1);
    RG_DEF_METHOD(uris, 0);
    RG_DEF_METHOD(set_uris, 1);
    RG_DEF_METHOD(targets_include_image, 1);
    RG_DEF_METHOD(targets, 0);
    RG_DEF_METHOD(targets_include_text, 0);
    RG_DEF_METHOD(targets_include_uri, 0);
    RG_DEF_METHOD(targets_include_rich_text, 1);
} 
