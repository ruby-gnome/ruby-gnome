/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtklabel.c -

  $Author: mutoh $
  $Date: 2002/10/19 13:20:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
label_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    rb_scan_args(argc, argv, "01", &label);
    RBGTK_INITIALIZE(self, gtk_label_new(NIL_P(label)?NULL:RVAL2CSTR(label)));
    return Qnil;
}

static VALUE
label_set_text(self, str)
    VALUE self, str;
{
    gtk_label_set_text(GTK_LABEL(RVAL2GOBJ(self)), RVAL2CSTR(str));
    return self;
}

/*
void        gtk_label_set_markup            (GtkLabel *label,
                                             const gchar *str);
void        gtk_label_set_markup_with_mnemonic
                                            (GtkLabel *label,
                                             const gchar *str);
void        gtk_label_get_layout_offsets    (GtkLabel *label,
                                             gint *x,
                                             gint *y);
*/

static VALUE
label_get_text(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_label_get_text(GTK_LABEL(RVAL2GOBJ(self))));
}

/*
GtkWidget*  gtk_label_new_with_mnemonic     (const char *str);
void        gtk_label_select_region         (GtkLabel *label,
                                             gint start_offset,
                                             gint end_offset);
void        gtk_label_set_text_with_mnemonic
                                            (GtkLabel *label,
                                             const gchar *str);
PangoLayout* gtk_label_get_layout           (GtkLabel *label);
gboolean    gtk_label_get_selection_bounds  (GtkLabel *label,
                                             gint *start,
                                             gint *end);
*/

void 
Init_gtk_label()
{
    VALUE gLabel = G_DEF_CLASS(GTK_TYPE_LABEL, "Label", mGtk);

    rb_define_method(gLabel, "initialize", label_initialize, -1);
    rb_define_method(gLabel, "text", label_get_text, 0);
    rb_define_method(gLabel, "set_text", label_set_text, 1);

    G_DEF_SETTERS(gLabel);
}
