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

#include "global.h"

#define RG_TARGET_NAMESPACE cSelectionData
#define _SELF(d) ((GtkSelectionData*)RVAL2BOXED(d, GTK_TYPE_SELECTION_DATA))

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
        int *i;
        i = ALLOC(int);
        *i = NUM2INT(src);
        dat = i;
        fmt = sizeof(int) * 8;
        len = 1;
    } else if(ntype == GDK_SELECTION_TYPE_STRING) {
        dat = (void *)RVAL2CSTR(src);
        if (NIL_P(size)) {
            fmt = sizeof(char) * 8;
            len = RSTRING_LEN(src);
        } else {
            len = NUM2UINT(size);
            fmt = (RSTRING_LEN(src) / len) * 8;
        }
    } else if(ntype == compound_text){
        guchar* str = (guchar*)dat;
        gdk_string_to_compound_text(RVAL2CSTR(src), &ntype, &fmt, &str, &len);
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
    } else if(type == compound_text) {
        gdk_free_compound_text(dat);
    }
}    
/************************************************************************/

static VALUE
rg_selection(VALUE self)
{
    GdkAtom selection = _SELF(self)->selection;
    return BOXED2RVAL(&selection, GDK_TYPE_ATOM);
}

static VALUE
rg_target(VALUE self)
{
    return BOXED2RVAL(_SELF(self)->target, GDK_TYPE_ATOM);
}

static VALUE
rg_type(VALUE self)
{
    return BOXED2RVAL(_SELF(self)->type, GDK_TYPE_ATOM);
}

static VALUE
rg_format(VALUE self)
{
    return INT2NUM(_SELF(self)->format);
}

static VALUE
rg_data(VALUE self)
{
    return RBG_STRING_SET_UTF8_ENCODING(rb_str_new((const char*)_SELF(self)->data,
                                                   _SELF(self)->length));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_display(VALUE self)
{
    return BOXED2RVAL(_SELF(self)->display, GDK_TYPE_DISPLAY);
}
#endif

/* Instance Methods */
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

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_set_pixbuf(VALUE self, VALUE pixbuf)
{
    if (!gtk_selection_data_set_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(pixbuf))))
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
#endif

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
            rb_ary_push(result, BOXED2RVAL(targets[i], GDK_TYPE_ATOM));
        }
    }
    g_free(targets);
    return result;
}
#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_targets_include_image(VALUE self, VALUE writable)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_image(_SELF(self), RVAL2CBOOL(writable)));
}
#endif

static VALUE
rg_targets_include_text(VALUE self)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_text(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_targets_include_uri(VALUE self)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_uri(_SELF(self)));
}
static VALUE
rg_targets_include_rich_text(VALUE self, VALUE buffer)
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_rich_text(_SELF(self),
                                                                   GTK_TEXT_BUFFER(RVAL2GOBJ(buffer))));
}
#endif

void
Init_gtk_selectiondata(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SELECTION_DATA, "SelectionData", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);

    compound_text = gdk_atom_intern("COMPOUND_TEXT", FALSE);

    RG_DEF_METHOD(selection, 0);
    RG_DEF_METHOD(target, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(format, 0);
    RG_DEF_METHOD(data, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(display, 0);
#endif 

    RG_DEF_METHOD(set, -1);
    RG_DEF_METHOD(text, 0);
    RG_DEF_METHOD(set_text, 1);

#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(pixbuf, 0);
    RG_DEF_METHOD(set_pixbuf, 1);
    RG_DEF_METHOD(uris, 0);
    RG_DEF_METHOD(set_uris, 1);
    RG_DEF_METHOD(targets_include_image, 1);
#endif
    RG_DEF_METHOD(targets, 0);
    RG_DEF_METHOD(targets_include_text, 0);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(targets_include_uri, 0);
    RG_DEF_METHOD(targets_include_rich_text, 1);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

} 
