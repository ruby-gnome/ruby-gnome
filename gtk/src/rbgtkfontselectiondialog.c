/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfontselectiondialog.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:16 $

  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
************************************************/

#include "global.h"

#define _SELF(self) (GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self)))

static VALUE
fsd_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_font_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
fsd_get_font_name(self)
    VALUE self;
{
    gchar* name = gtk_font_selection_dialog_get_font_name(_SELF(self));
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
fsd_set_font_name(self, fontname)
    VALUE self, fontname;
{
    return gtk_font_selection_dialog_set_font_name(_SELF(self),
                                                   RVAL2CSTR(fontname)) ? Qtrue : Qfalse;
}

static VALUE
fsd_get_preview_text(self)
    VALUE self;
{
    const gchar* text = gtk_font_selection_dialog_get_preview_text(_SELF(self));
    return text ? CSTR2RVAL(text) : Qnil;
}

static VALUE
fsd_set_preview_text(self, text)
    VALUE self, text;
{
    gtk_font_selection_dialog_set_preview_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
fsd_get_font_selection(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->fontsel);
}

static VALUE
fsd_get_ok_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
fsd_get_cancel_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
fsd_get_apply_button(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->apply_button);
}

void 
Init_gtk_font_selection_dialog()
{
    VALUE gFontSelectionDialog = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION_DIALOG, 
                                             "FontSelectionDialog", mGtk);

    rb_define_method(gFontSelectionDialog, "initialize", fsd_initialize, 1);
    rb_define_method(gFontSelectionDialog, "font_name", fsd_get_font_name, 0);
    rb_define_method(gFontSelectionDialog, "set_font_name", fsd_set_font_name, 1);
    rb_define_method(gFontSelectionDialog, "preview_text", fsd_get_preview_text, 0);
    rb_define_method(gFontSelectionDialog, "set_preview_text", fsd_set_preview_text, 1);
    rb_define_method(gFontSelectionDialog, "font_selection", fsd_get_font_selection, 0);
    rb_define_method(gFontSelectionDialog, "ok_button", fsd_get_ok_button, 0);
    rb_define_method(gFontSelectionDialog, "cancel_button", fsd_get_cancel_button, 0);
    rb_define_method(gFontSelectionDialog, "apply_button", fsd_get_apply_button, 0);

    G_DEF_SETTERS(gFontSelectionDialog);
}
