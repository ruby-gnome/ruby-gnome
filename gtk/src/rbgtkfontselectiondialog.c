/************************************************

  rbgtkfontselectiondialog.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
************************************************/

#include "global.h"

static VALUE
fsd_initialize(self, title)
    VALUE self, title;
{
    RBGTK_INITIALIZE(self, gtk_font_selection_dialog_new(STR2CSTR(title)));
    return Qnil;
}

#ifndef GTK_DISABLE_DEPRECATED
static VALUE
fsd_get_font(self)
    VALUE self;
{
    return make_gdkfont(gtk_font_selection_dialog_get_font(
                GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))));
}
#endif

static VALUE
fsd_get_font_name(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_dialog_get_font_name(
                GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))));
}

static VALUE
fsd_set_font_name(self, fontname)
    VALUE self, fontname;
{
    gboolean retval = gtk_font_selection_dialog_set_font_name(
                            GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self)),
                            STR2CSTR(fontname));
    return retval ? Qtrue : Qfalse;
}

static VALUE
fsd_get_preview_text(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_dialog_get_preview_text(
                GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))));
}

static VALUE
fsd_set_preview_text(self, text)
    VALUE self, text;
{
    gtk_font_selection_dialog_set_preview_text(
        GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self)),
        STR2CSTR(text));
    return Qnil;
}

static VALUE
fsd_get_font_selection(self)
    VALUE self;
{
    VALUE fs;

    fs = rb_iv_get(self, "@__font_selection");
    if (NIL_P(fs)) {
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))->fontsel;
        fs = GOBJ2RVAL(w);
        rb_iv_set(self, "@__font_selection", fs);
    }

    return fs;
}

static VALUE
fsd_get_ok_button(self)
    VALUE self;
{
    VALUE button;

    button = rb_iv_get(self, "@__ok_button");
    if (NIL_P(button)) {
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))->ok_button;
        button = GOBJ2RVAL(w);
        rb_iv_set(self, "@__ok_button", button);
    }

    return button;
}

static VALUE
fsd_get_cancel_button(self)
    VALUE self;
{
    VALUE button;

    button = rb_iv_get(self, "@__cancel_button");
    if (NIL_P(button)) {
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))->cancel_button;
        button = GOBJ2RVAL(w);
        rb_iv_set(self, "@__cancel_button", button);
    }

    return button;
}

static VALUE
fsd_get_apply_button(self)
    VALUE self;
{
    VALUE button;

    button = rb_iv_get(self, "@__apply_button");
    if (NIL_P(button)) {
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))->apply_button;
        button = GOBJ2RVAL(w);
        rb_iv_set(self, "@__apply_button", button);
    }

    return button;
}

void Init_gtk_font_selection_dialog()
{
    VALUE gFontSelectionDialog = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION_DIALOG, "FontSelectionDialog", mGtk);

    rb_define_method(gFontSelectionDialog, "initialize", fsd_initialize, 1);
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(gFontSelectionDialog, "font", fsd_get_font, 0);
#endif
    rb_define_method(gFontSelectionDialog, "font_name", fsd_get_font_name, 0);
    rb_define_method(gFontSelectionDialog, "font_name=", fsd_set_font_name, 1);
    rb_define_method(gFontSelectionDialog, "preview_text", fsd_get_preview_text, 0);
    rb_define_method(gFontSelectionDialog, "preview_text=", fsd_set_preview_text, 1);
    rb_define_method(gFontSelectionDialog, "font_selection", fsd_get_font_selection, 0);
    rb_define_method(gFontSelectionDialog, "ok_button", fsd_get_ok_button, 0);
    rb_define_method(gFontSelectionDialog, "cancel_button", fsd_get_cancel_button, 0);
    rb_define_method(gFontSelectionDialog, "apply_button", fsd_get_apply_button, 0);

    rb_define_alias(gFontSelectionDialog, "set_font_name", "font_name=");
    rb_define_alias(gFontSelectionDialog, "set_preview_text", "preview_text=");
}
