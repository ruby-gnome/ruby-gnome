/************************************************

  rbgtkfontselection.c -

  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
************************************************/

#include "global.h"

static VALUE
fs_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_font_selection_new());
    return Qnil;
}

static VALUE
fs_get_font(self)
    VALUE self;
{
    return make_gdkfont(gtk_font_selection_get_font(
                GTK_FONT_SELECTION(get_widget(self))));
}

static VALUE
fs_get_font_name(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_get_font_name(
                GTK_FONT_SELECTION(get_widget(self))));
}

static VALUE
fs_set_font_name(self, name)
    VALUE self, name;
{
    gboolean retval = gtk_font_selection_set_font_name(
                            GTK_FONT_SELECTION(get_widget(self)),
                            STR2CSTR(name));
    return retval ? Qtrue : Qfalse;
}

static VALUE
fs_get_preview_text(self)
    VALUE self;
{
    return CSTR2OBJ(gtk_font_selection_get_preview_text(
                GTK_FONT_SELECTION(get_widget(self))));
}

static VALUE
fs_set_preview_text(self, text)
    VALUE self, text;
{
    gtk_font_selection_set_preview_text(
        GTK_FONT_SELECTION(get_widget(self)),
        STR2CSTR(text));
    return Qnil;
}

#if GTK_MAJOR_VERSION < 2

static VALUE
fs_set_filter(self, filter_type, font_type, foundries,
              weights, slants, setwidths, spacings, charsets)
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

    gtk_font_selection_set_filter(GTK_FONT_SELECTION(get_widget(self)),
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

#endif

void Init_gtk_font_selection()
{
    gFontSelection = rb_define_class_under(mGtk, "FontSelection", gNotebook);

    rb_define_method(gFontSelection, "initialize", fs_initialize, 0);
    rb_define_method(gFontSelection, "font", fs_get_font, 0);
    rb_define_method(gFontSelection, "font_name", fs_get_font_name, 0);
    rb_define_method(gFontSelection, "font_name=", fs_set_font_name, 1);
    rb_define_method(gFontSelection, "preview_text", fs_get_preview_text, 0);
    rb_define_method(gFontSelection, "preview_text=", fs_set_preview_text, 1);
#if GTK_MAJOR_VERSION < 2
    rb_define_method(gFontSelection, "filter=", fs_set_filter, 10);
#endif

    rb_define_alias(gFontSelection, "set_font_name", "font_name=");
    rb_define_alias(gFontSelection, "set_preview_text", "preview_text=");
#if GTK_MAJOR_VERSION < 2
    rb_define_alias(gFontSelection, "set_filter", "filter=");
#endif
}
