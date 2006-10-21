/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtklabel.c -

  $Author: mutoh $
  $Date: 2006/10/21 16:58:00 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_LABEL(RVAL2GOBJ(s)))

static VALUE
label_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, with_mnemonic;
    GtkWidget* widget = NULL;

    rb_scan_args(argc, argv, "02", &label, &with_mnemonic);
    if (NIL_P(with_mnemonic) || ! RTEST(with_mnemonic)){
        widget = gtk_label_new(NIL_P(label)?NULL:RVAL2CSTR(label));
    } else {
        widget = gtk_label_new_with_mnemonic(NIL_P(label)?NULL:RVAL2CSTR(label));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
label_set_text_only(self, text)
    VALUE self, text;
{
    gtk_label_set_text(_SELF(self), RVAL2CSTR(text));
    return text;
}

static VALUE
label_set_text(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, with_mnemonic;

    rb_scan_args(argc, argv, "02", &label, &with_mnemonic);
    if (NIL_P(with_mnemonic) || ! RTEST(with_mnemonic)){
        gtk_label_set_text(_SELF(self), RVAL2CSTR(label));
    } else {
        gtk_label_set_text_with_mnemonic(_SELF(self), RVAL2CSTR(label));
    }
    return self;
}

static VALUE
label_set_markup_only(self, text)
    VALUE self, text;
{
    gtk_label_set_markup(_SELF(self), RVAL2CSTR(text));
    return text;
}

static VALUE
label_set_markup(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE str, with_mnemonic;
    rb_scan_args(argc, argv, "02", &str, &with_mnemonic);

    if (NIL_P(with_mnemonic) || ! RTEST(with_mnemonic)){
        gtk_label_set_markup(_SELF(self), RVAL2CSTR(str));
    } else {
        gtk_label_set_markup_with_mnemonic(_SELF(self), RVAL2CSTR(str));
    }        
    return self;
}

static VALUE
label_get_layout_offsets(self)
    VALUE self;
{ 
    gint x, y;
    gtk_label_get_layout_offsets(_SELF(self), &x, &y);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

static VALUE
label_get_text(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_label_get_text(_SELF(self)));
}

static VALUE
label_select_region(self, start_offset, end_offset)
    VALUE self, start_offset, end_offset;
{
    gtk_label_select_region(_SELF(self), NUM2INT(start_offset), NUM2INT(end_offset));
    return self;
}

static VALUE
label_get_layout(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_label_get_layout(_SELF(self)));
}

static VALUE
label_get_selection_bounds(self)
    VALUE self;
{
    gint start, end;
    gboolean ret = gtk_label_get_selection_bounds(_SELF(self), &start, &end);
    return ret ? rb_ary_new3(2, INT2NUM(start), INT2NUM(end)) : Qnil;
}

/* Defined as Properties
gdouble     gtk_label_get_angle             (GtkLabel *label);
void        gtk_label_set_angle             (GtkLabel *label,
                                             gdouble angle);
void        gtk_label_set_ellipsize         (GtkLabel *label,
                                             PangoEllipsizeMode mode);
PangoEllipsizeMode gtk_label_get_ellipsize  (GtkLabel *label);
void        gtk_label_set_max_width_chars   (GtkLabel *label,
                                             gint n_chars);
gint        gtk_label_get_max_width_chars   (GtkLabel *label);

gboolean    gtk_label_get_single_line_mode  (GtkLabel *label);
void        gtk_label_set_single_line_mode  (GtkLabel *label,
                                             gboolean single_line_mode);
void        gtk_label_set_width_chars       (GtkLabel *label,
                                             gint n_chars);
gint        gtk_label_get_width_chars       (GtkLabel *label);
void        gtk_label_set_line_wrap_mode    (GtkLabel *label,
                                             PangoWrapMode wrap_mode);
PangoWrapMode gtk_label_get_line_wrap_mode  (GtkLabel *label);
*/

void 
Init_gtk_label()
{
    VALUE gLabel = G_DEF_CLASS(GTK_TYPE_LABEL, "Label", mGtk);

    rb_define_method(gLabel, "initialize", label_initialize, -1);
    rb_define_method(gLabel, "text", label_get_text, 0);
    rb_define_method(gLabel, "set_text", label_set_text, -1);
    rb_define_method(gLabel, "text=", label_set_text_only, 1);
    rb_define_method(gLabel, "set_markup", label_set_markup, -1);
    rb_define_method(gLabel, "markup=", label_set_markup_only, 1);
    rb_define_method(gLabel, "layout_offsets", label_get_layout_offsets, 0);
    rb_define_method(gLabel, "select_region", label_select_region, 2);
    rb_define_method(gLabel, "layout", label_get_layout, 0);
    rb_define_method(gLabel, "selection_bounds", label_get_selection_bounds, 0);

    G_DEF_SETTERS(gLabel);
}
