/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkselectiondata.c -

  $Author: mutoh $
  $Date: 2003/07/14 18:12:53 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/


#include "global.h"

#define _SELF(d) ((GtkSelectionData*)RVAL2BOXED(d, GTK_TYPE_SELECTION_DATA))

static GdkAtom compound_text;

/************************************************************************/
void
rbgtk_atom2selectiondata(type, size, src, gtype, data, format, length)
    VALUE type, size, src;
    void** data;
    GdkAtom* gtype;
    gint* format;
    gint* length;
{
    void* dat;
    gint fmt, len;
    GdkAtom ntype = RVAL2ATOM(type);
    
    if(ntype == GDK_SELECTION_TYPE_INTEGER){
        int i = NUM2INT(src);
        dat = (void*)&i;
        fmt = 32;
        len = 1;
    } else if(ntype == GDK_SELECTION_TYPE_STRING) {
        dat = RVAL2CSTR(src);
        fmt = 8;
        len = RSTRING(src)->len;
    } else if(ntype == compound_text){
        gdk_string_to_compound_text(RVAL2CSTR(src), &ntype, &fmt, (guchar**)&dat, &len);
    } else if(type != Qnil && size != Qnil && src != Qnil) {
    	dat = RVAL2CSTR(src);
	fmt = NUM2INT(size);
	len = RSTRING(src)->len;
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
    if(type == GDK_SELECTION_TYPE_ATOM) {
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
    return rb_str_new(_SELF(self)->data, _SELF(self)->length);
}

#if GTK_MINOR_VERSION >= 2
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
    gtk_selection_data_set_text(_SELF(self), RVAL2CSTR(str),
                                RSTRING(str)->len);
    return self;
}

static VALUE
gtkselectiondata_get_text(self)
    VALUE self;
{
    gchar* text = gtk_selection_data_get_text(_SELF(self));
    return text ? CSTR2RVAL(text) : Qnil;
}

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
            rb_ary_push(result, BOXED2RVAL(targets, GDK_TYPE_ATOM));
            targets++;
        }
    }
    return result;
}

static VALUE
gtkselectiondata_targets_include_text(self)
    VALUE self;
{
    return gtk_selection_data_targets_include_text(_SELF(self)) ? Qtrue : Qfalse;
}

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
#if GTK_MINOR_VERSION >= 2
    rb_define_method(gSelectionData, "display", gtkselectiondata_display, 0);
#endif

    rb_define_method(gSelectionData, "set", gtkselectiondata_set, -1);
    rb_define_method(gSelectionData, "set_text", gtkselectiondata_set_text, 1);
    rb_define_method(gSelectionData, "text", gtkselectiondata_get_text, 0);
    rb_define_method(gSelectionData, "targets", gtkselectiondata_get_targets, 0);
    rb_define_method(gSelectionData, "targets_include_text?", gtkselectiondata_targets_include_text, 0);

    G_DEF_SETTERS(gSelectionData);
} 

