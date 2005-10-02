/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcetagstyle.c -

  $Author: mutoh $
  $Date: 2005/10/02 18:40:34 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceTagStyle
 * The style of a source tag.
 */
#define _SELF(self) ((GtkSourceTagStyle*)RVAL2BOXED(self, GTK_TYPE_SOURCE_TAG_STYLE))

#define ATTR_BOOL(name)\
static VALUE \
sourcetagstyle_ ## name (self)\
    VALUE self;\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}

/* Class method: new
 * Returns: a newly created Gtk::SourceTagStyle object.
 */
static VALUE
sourcetagstyle_new (self)
    VALUE self;
{
    G_INITIALIZE (self, gtk_source_tag_style_new ());
    return Qnil;
}

ATTR_BOOL(is_default);

static VALUE
sourcetagstyle_mask(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->mask);
}

static VALUE
sourcetagstyle_foreground(self)
    VALUE self;
{
    GdkColor color = _SELF(self)->foreground;
    return BOXED2RVAL(&color, GDK_TYPE_COLOR);
}

static VALUE
sourcetagstyle_background(self)
    VALUE self;
{
    GdkColor color = _SELF(self)->foreground;
    return BOXED2RVAL(&color, GDK_TYPE_COLOR);
}

ATTR_BOOL(italic);
ATTR_BOOL(bold);
ATTR_BOOL(underline);
ATTR_BOOL(strikethrough);

void
Init_gtk_sourcetagstyle ()
{
    VALUE tagstyle = G_DEF_CLASS (GTK_TYPE_SOURCE_TAG_STYLE, "SourceTagStyle", mGtk);

    rb_define_method(tagstyle, "initialize", sourcetagstyle_new, 0);
    rb_define_method(tagstyle, "default?", sourcetagstyle_is_default, 0);
    rb_define_method(tagstyle, "mask", sourcetagstyle_mask, 0);
    rb_define_method(tagstyle, "foreground", sourcetagstyle_foreground, 0);
    rb_define_method(tagstyle, "background", sourcetagstyle_background, 0);
    rb_define_method(tagstyle, "italic?", sourcetagstyle_italic, 0);
    rb_define_method(tagstyle, "bold?", sourcetagstyle_bold, 0);
    rb_define_method(tagstyle, "underline?", sourcetagstyle_underline, 0);
    rb_define_method(tagstyle, "strikethrough?", sourcetagstyle_strikethrough, 0);

    /* GtkSourceTagStyleMask */
    G_DEF_CLASS(GTK_TYPE_SOURCE_TAG_STYLE_MASK, "Mask", tagstyle);
    G_DEF_CONSTANTS(tagstyle, GTK_TYPE_SOURCE_TAG_STYLE_MASK, "GTK_SOURCE_TAG_STYLE_");
    
    G_DEF_SETTERS (tagstyle);
}
