/************************************************

  rbgtkfontselectiondialog.c -

  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
************************************************/

#include "global.h"

static VALUE
fsd_initialize(self, title)
    VALUE self, title;
{
    set_widget(self, gtk_font_selection_dialog_new(STR2CSTR(title)));
    return Qnil;
}

static VALUE
fsd_get_font(self)
    VALUE self;
{
    return make_gdkfont(gtk_font_selection_dialog_get_font(
                GTK_FONT_SELECTION_DIALOG(get_widget(self))));
}

static VALUE
fsd_get_font_name(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_dialog_get_font_name(
                GTK_FONT_SELECTION_DIALOG(get_widget(self))));
}

static VALUE
fsd_set_font_name(self, fontname)
    VALUE self, fontname;
{
    gboolean retval = gtk_font_selection_dialog_set_font_name(
                            GTK_FONT_SELECTION_DIALOG(get_widget(self)),
                            STR2CSTR(fontname));
    return retval ? Qtrue : Qfalse;
}

static VALUE
fsd_get_preview_text(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_dialog_get_preview_text(
                GTK_FONT_SELECTION_DIALOG(get_widget(self))));
}

static VALUE
fsd_set_preview_text(self, text)
    VALUE self, text;
{
    gtk_font_selection_dialog_set_preview_text(
        GTK_FONT_SELECTION_DIALOG(get_widget(self)),
        STR2CSTR(text));
    return Qnil;
}

static VALUE
fsd_set_filter(self, filter_type, font_type, foundries, weights,
               slants, setwidths, spacings, charsets)
    VALUE self, filter_type, font_type, foundries, weights,
          slants, setwidths, spacings, charsets;
{
    int i, len;
    gchar** c_foundries = NULL;
    gchar** c_weights   = NULL;
    gchar** c_slants    = NULL;
    gchar** c_setwidths = NULL;
    gchar** c_spacings  = NULL;
    gchar** c_charsets  = NULL;

    if (! NIL_P(foundries)) {
        Check_Type(foundries, T_ARRAY);
        len = RARRAY(foundries)->len;
        c_foundries = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_foundries[i] = STR2CSTR(RARRAY(foundries)->ptr[i]);
        }
        c_foundries[i] = NULL;
    }

    if (! NIL_P(weights)) {
        Check_Type(weights, T_ARRAY);
        len = RARRAY(weights)->len;
        c_weights = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_weights[i] = STR2CSTR(RARRAY(weights)->ptr[i]);
        }
        c_weights[i] = NULL;
    }

    if (! NIL_P(slants)) {
        Check_Type(slants, T_ARRAY);
        len = RARRAY(slants)->len;
        c_slants = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_slants[i] = STR2CSTR(RARRAY(slants)->ptr[i]);
        }
        c_slants[i] = NULL;
    }

    if (! NIL_P(setwidths)) {
        Check_Type(setwidths, T_ARRAY);
        len = RARRAY(setwidths)->len;
        c_setwidths = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_setwidths[i] = STR2CSTR(RARRAY(setwidths)->ptr[i]);
        }
        c_setwidths[i] = NULL;
    }

    if (! NIL_P(spacings)) {
        Check_Type(spacings, T_ARRAY);
        len = RARRAY(spacings)->len;
        c_spacings = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_spacings[i] = STR2CSTR(RARRAY(spacings)->ptr[i]);
        }
        c_spacings[i] = NULL;
    }

    if (! NIL_P(charsets)) {
        Check_Type(charsets, T_ARRAY);
        len = RARRAY(charsets)->len;
        c_charsets = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            c_charsets[i] = STR2CSTR(RARRAY(charsets)->ptr[i]);
        }
        c_charsets[i] = NULL;
    }

    gtk_font_selection_dialog_set_filter(
            GTK_FONT_SELECTION_DIALOG(get_widget(self)),
            NUM2INT(filter_type),
            NUM2INT(font_type),
            c_foundries,
            c_weights,
            c_slants,
            c_setwidths,
            c_spacings,
            c_charsets);
    return Qnil;
}

static VALUE
fsd_get_font_selection(self)
    VALUE self;
{
    VALUE fs;

    fs = rb_iv_get(self, "@__font_selection");
    if (NIL_P(fs)) {
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(get_widget(self))->fontsel;
        fs = make_widget(gFontSelection, w);
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
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(get_widget(self))->ok_button;
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
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(get_widget(self))->cancel_button;
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
        GtkWidget *w = GTK_FONT_SELECTION_DIALOG(get_widget(self))->apply_button;
        button = make_widget(gButton, w);
        rb_iv_set(self, "@__apply_button", button);
    }

    return button;
}

void Init_gtk_font_selection_dialog()
{
    gFontSelectionDialog = rb_define_class_under(mGtk,
                                "FontSelectionDialog", gWindow);

    rb_define_method(gFontSelectionDialog, "initialize", fsd_initialize, 1);
    rb_define_method(gFontSelectionDialog, "font", fsd_get_font, 0);
    rb_define_method(gFontSelectionDialog, "font_name", fsd_get_font_name, 0);
    rb_define_method(gFontSelectionDialog, "font_name=", fsd_set_font_name, 1);
    rb_define_method(gFontSelectionDialog, "preview_text", fsd_get_preview_text, 0);
    rb_define_method(gFontSelectionDialog, "preview_text=", fsd_set_preview_text, 1);
    rb_define_method(gFontSelectionDialog, "filter=", fsd_set_filter, 8);
    rb_define_method(gFontSelectionDialog, "font_selection", fsd_get_font_selection, 0);
    rb_define_method(gFontSelectionDialog, "ok_button", fsd_get_ok_button, 0);
    rb_define_method(gFontSelectionDialog, "cancel_button", fsd_get_cancel_button, 0);
    rb_define_method(gFontSelectionDialog, "apply_button", fsd_get_apply_button, 0);

    rb_define_alias(gFontSelectionDialog, "set_font_name", "font_name=");
    rb_define_alias(gFontSelectionDialog, "set_preview_text", "preview_text=");
    rb_define_alias(gFontSelectionDialog, "set_filter", "filter=");
}
