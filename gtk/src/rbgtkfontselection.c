/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************

  rbgtkfontselection.c -

  $Author: mutoh $
  $Date: 2002/09/14 15:43:40 $

  Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
**********************************************************/

#include "global.h"

static VALUE
fs_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_font_selection_new());
    return Qnil;
}

#ifndef GTK_DISABLE_DEPRECATED
static VALUE
fs_get_font(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_font_selection_get_font(
                          GTK_FONT_SELECTION(RVAL2GOBJ(self))), GDK_TYPE_FONT);
}
#endif

static VALUE
fs_get_font_name(self)
    VALUE self;
{
    gchar* name = gtk_font_selection_get_font_name(
        GTK_FONT_SELECTION(RVAL2GOBJ(self)));
    
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
fs_set_font_name(self, name)
    VALUE self, name;
{
    gboolean retval = gtk_font_selection_set_font_name(
        GTK_FONT_SELECTION(RVAL2GOBJ(self)),
        RVAL2CSTR(name));
    return retval ? Qtrue : Qfalse;
}

static VALUE
fs_get_preview_text(self)
    VALUE self;
{
    G_CONST_RETURN gchar* text = gtk_font_selection_get_preview_text(
        GTK_FONT_SELECTION(RVAL2GOBJ(self)));

    return text ? CSTR2RVAL(text) : Qnil;
}

static VALUE
fs_set_preview_text(self, text)
    VALUE self, text;
{
    gtk_font_selection_set_preview_text(
        GTK_FONT_SELECTION(RVAL2GOBJ(self)),
        RVAL2CSTR(text));
    return Qnil;
}

void 
Init_gtk_font_selection()
{
    VALUE gFontSelection = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION, "FontSelection", mGtk);
    rb_define_method(gFontSelection, "initialize", fs_initialize, 0);
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(gFontSelection, "font", fs_get_font, 0);
#endif
    rb_define_method(gFontSelection, "font_name", fs_get_font_name, 0);
    rb_define_method(gFontSelection, "font_name=", fs_set_font_name, 1);
    rb_define_method(gFontSelection, "preview_text", fs_get_preview_text, 0);
    rb_define_method(gFontSelection, "preview_text=", fs_set_preview_text, 1);

    rb_define_alias(gFontSelection, "set_font_name", "font_name=");
    rb_define_alias(gFontSelection, "set_preview_text", "preview_text=");
}
