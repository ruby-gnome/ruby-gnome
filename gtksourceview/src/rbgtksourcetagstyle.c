/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcetagstyle.c -

  $Author: ktou $
  $Date: 2006/09/24 13:03:27 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

/* Class: Gtk::SourceTagStyle
 * The style of a source tag.
 */
#define _SELF(self) ((GtkSourceTagStyle*)RVAL2BOXED(self, GTK_TYPE_SOURCE_TAG_STYLE))

#define RVAL2MASK(obj) (RVAL2GFLAGS(obj, GTK_TYPE_SOURCE_TAG_STYLE_MASK))
#define MASK2RVAL(obj) (GFLAGS2RVAL(obj, GTK_TYPE_SOURCE_TAG_STYLE_MASK))

#define RVAL2COLOR(obj) ((GdkColor *)RVAL2BOXED(obj, GDK_TYPE_COLOR))
#define COLOR2RVAL(obj) (BOXED2RVAL(obj, GDK_TYPE_COLOR))

#define ATTR_READER_BOOL(name)                  \
static VALUE                                    \
sourcetagstyle_ ## name (self)                  \
    VALUE self;                                 \
{                                               \
    return CBOOL2RVAL(_SELF(self)->name);       \
}

#define ATTR_WRITER_BOOL(name)                  \
static VALUE                                    \
sourcetagstyle_set_ ## name (self, value)       \
    VALUE self, value;                          \
{                                               \
    _SELF(self)->name = RVAL2CBOOL(value);      \
    return Qnil;                                \
}

#define ATTR_BOOL(name)                         \
  ATTR_READER_BOOL(name)                        \
  ATTR_WRITER_BOOL(name)

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

ATTR_READER_BOOL(is_default);

static VALUE
sourcetagstyle_mask(self)
    VALUE self;
{
    return MASK2RVAL(_SELF(self)->mask);
}

static VALUE
sourcetagstyle_set_mask(self, mask)
    VALUE self, mask;
{
    _SELF(self)->mask = RVAL2MASK(mask);
    return Qnil;
}

static VALUE
sourcetagstyle_foreground(self)
    VALUE self;
{
    GdkColor color = _SELF(self)->foreground;
    return COLOR2RVAL(&color);
}

static VALUE
sourcetagstyle_set_foreground(self, rb_color)
    VALUE self, rb_color;
{
    GdkColor *new_color = RVAL2COLOR(rb_color);
    memcpy(&(_SELF(self)->foreground), new_color, sizeof(GdkColor));
    return Qnil;
}

static VALUE
sourcetagstyle_background(self)
    VALUE self;
{
    GdkColor color = _SELF(self)->foreground;
    return COLOR2RVAL(&color);
}

static VALUE
sourcetagstyle_set_background(self, rb_color)
    VALUE self, rb_color;
{
    GdkColor *new_color = RVAL2COLOR(rb_color);
    memcpy(&(_SELF(self)->background), new_color, sizeof(GdkColor));
    return Qnil;
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

    rb_define_method(tagstyle, "set_mask", sourcetagstyle_set_mask, 1);
    rb_define_method(tagstyle, "set_foreground",
                     sourcetagstyle_set_foreground, 1);
    rb_define_method(tagstyle, "set_background",
                     sourcetagstyle_set_background, 1);
    rb_define_method(tagstyle, "set_italic", sourcetagstyle_set_italic, 1);
    rb_define_method(tagstyle, "set_bold", sourcetagstyle_set_bold, 1);
    rb_define_method(tagstyle, "set_underline",
                     sourcetagstyle_set_underline, 1);
    rb_define_method(tagstyle, "set_strikethrough",
                     sourcetagstyle_set_strikethrough, 1);

    /* GtkSourceTagStyleMask */
    G_DEF_CLASS(GTK_TYPE_SOURCE_TAG_STYLE_MASK, "Mask", tagstyle);
    G_DEF_CONSTANTS(tagstyle, GTK_TYPE_SOURCE_TAG_STYLE_MASK, "GTK_SOURCE_TAG_STYLE_");
    
    G_DEF_SETTERS (tagstyle);
}
