/************************************************

  rbgtkfontselectiondialog.c -

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

static VALUE
fsd_get_font(self)
    VALUE self;
{
    return make_gdkfont(gtk_font_selection_dialog_get_font(
                GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self))));
}

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
        button = make_widget(gButton, w);
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
        button = make_widget(gButton, w);
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
        button = make_widget(gButton, w);
        rb_iv_set(self, "@__apply_button", button);
    }

    return button;
}

void Init_gtk_font_selection_dialog()
{
    static RGObjClassInfo cinfo;
    gFontSelectionDialog = rb_define_class_under(mGtk,
                                "FontSelectionDialog", gWindow);
    cinfo.klass = gFontSelectionDialog;
    cinfo.gtype = GTK_TYPE_FONT_SELECTION_DIALOG;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gFontSelectionDialog, "initialize", fsd_initialize, 1);
    rb_define_method(gFontSelectionDialog, "font", fsd_get_font, 0);
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
