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

void Init_gtk_font_selection()
{
    gFontSelection = rb_define_class_under(mGtk, "FontSelection", gNotebook);

    rb_define_method(gFontSelection, "initialize", fs_initialize, 0);
    rb_define_method(gFontSelection, "font", fs_get_font, 0);
    rb_define_method(gFontSelection, "font_name", fs_get_font_name, 0);
    rb_define_method(gFontSelection, "font_name=", fs_set_font_name, 1);
    rb_define_method(gFontSelection, "preview_text", fs_get_preview_text, 0);
    rb_define_method(gFontSelection, "preview_text=", fs_set_preview_text, 1);

    rb_define_alias(gFontSelection, "set_font_name", "font_name=");
    rb_define_alias(gFontSelection, "set_preview_text", "preview_text=");
}
