/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkselectiondata.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/


#include "global.h"

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
        dat = RVAL2CSTR(src);
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
    	dat = RVAL2CSTR(src);
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
rbgtk_atom2selectiondata_free(type, dat)
    GdkAtom type;
    void* dat;
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
gtkselectiondata_selection(self)
    VALUE self;
{
    GdkAtom selection = _SELF(self)->selection;
    return BOXED2RVAL(&selection, GDK_TYPE_ATOM);
}

static VALUE
gtkselectiondata_target(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->target, GDK_TYPE_ATOM);
}

static VALUE
gtkselectiondata_type(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->type, GDK_TYPE_ATOM);
}

static VALUE
gtkselectiondata_format(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->format);
}

static VALUE
gtkselectiondata_data(self)
    VALUE self;
{
    return rb_str_new((const char*)_SELF(self)->data, _SELF(self)->length);
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gtkselectiondata_display(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->display, GDK_TYPE_DISPLAY);
}
#endif

/* Instance Methods */
static VALUE
gtkselectiondata_set(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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
gtkselectiondata_set_text(self, str)
    VALUE self, str;
{
    gboolean ret;
    StringValue(str);

    ret = gtk_selection_data_set_text(_SELF(self), RVAL2CSTR(str),
                                      RSTRING_LEN(str));

    if (!ret) 
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully.");

    return self;
}

static VALUE
gtkselectiondata_get_text(self)
    VALUE self;
{
    VALUE ret = Qnil;
    guchar* text = gtk_selection_data_get_text(_SELF(self));
    if (text) {
        ret = CSTR2RVAL((const char*)text);
        g_free(text);
    }
    return ret;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gtkselectiondata_set_pixbuf(self, pixbuf)
    VALUE self, pixbuf;
{
    gboolean ret = gtk_selection_data_set_pixbuf(_SELF(self), 
                                                 GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    if (!ret) 
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully.");

    return self;
}

static VALUE
gtkselectiondata_get_pixbuf(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_selection_data_get_pixbuf(_SELF(self)));
}

static VALUE
gtkselectiondata_set_uris(self, uris)
    VALUE self, uris;
{
    gboolean ret;
    gchar** guris;
    int i, n_targets;
    
    Check_Type(uris, T_ARRAY);
    n_targets = RARRAY_LEN(uris);
 
    guris = g_new(gchar*, n_targets + 1);

    for (i = 0; i < n_targets; i++) {
        guris[i] = RVAL2CSTR(RARRAY_PTR(uris)[i]);
    }
    guris[n_targets] = NULL;

    ret = gtk_selection_data_set_uris(_SELF(self), guris);

    g_strfreev(guris);

    if (!ret) 
        rb_raise(rb_eRuntimeError, "the selection wasn't successfully.");

    return self;
}

static VALUE
gtkselectiondata_get_uris(self)
    VALUE self;
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
gtkselectiondata_get_targets(self)
    VALUE self;
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
gtkselectiondata_targets_include_image(self, writable)
    VALUE self, writable;
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_image(_SELF(self), RVAL2CBOOL(writable)));
}
#endif

static VALUE
gtkselectiondata_targets_include_text(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_text(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gtkselectiondata_targets_include_uri(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_uri(_SELF(self)));
}
static VALUE
gtkselectiondata_targets_include_rich_text(self, buffer)
    VALUE self, buffer;
{
    return CBOOL2RVAL(gtk_selection_data_targets_include_rich_text(_SELF(self),
                                                                   GTK_TEXT_BUFFER(RVAL2GOBJ(buffer))));
}
#endif

void
Init_gtk_selectiondata()
{
    VALUE gSelectionData = G_DEF_CLASS(GTK_TYPE_SELECTION_DATA, "SelectionData", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);

    compound_text = gdk_atom_intern("COMPOUND_TEXT", FALSE);

    rb_define_method(gSelectionData, "selection", gtkselectiondata_selection, 0);
    rb_define_method(gSelectionData, "target", gtkselectiondata_target, 0);
    rb_define_method(gSelectionData, "type", gtkselectiondata_type, 0);
    rb_define_method(gSelectionData, "format", gtkselectiondata_format, 0);
    rb_define_method(gSelectionData, "data", gtkselectiondata_data, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gSelectionData, "display", gtkselectiondata_display, 0);
#endif 

    rb_define_method(gSelectionData, "set", gtkselectiondata_set, -1);
    rb_define_method(gSelectionData, "text", gtkselectiondata_get_text, 0);
    rb_define_method(gSelectionData, "set_text", gtkselectiondata_set_text, 1);

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gSelectionData, "pixbuf", gtkselectiondata_get_pixbuf, 0);
    rb_define_method(gSelectionData, "set_pixbuf", gtkselectiondata_set_pixbuf, 1);
    rb_define_method(gSelectionData, "uris", gtkselectiondata_get_uris, 0);
    rb_define_method(gSelectionData, "set_uris", gtkselectiondata_set_uris, 1);
    rb_define_method(gSelectionData, "targets_include_image", gtkselectiondata_targets_include_image, 1);
#endif
    rb_define_method(gSelectionData, "targets", gtkselectiondata_get_targets, 0);
    rb_define_method(gSelectionData, "targets_include_text", gtkselectiondata_targets_include_text, 0);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gSelectionData, "targets_include_uri", gtkselectiondata_targets_include_uri, 0);
    rb_define_method(gSelectionData, "targets_include_rich_text", gtkselectiondata_targets_include_rich_text, 1);
#endif

    G_DEF_SETTERS(gSelectionData);

} 

