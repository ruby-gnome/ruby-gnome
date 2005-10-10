/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgpui-font-dialog.c -

  $Author: ktou $
  $Date: 2005/10/10 01:45:36 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgpui.h"

#ifdef HAVE_LIBGNOMEPRINTUI_GNOME_FONT_DIALOG_H
#include <libgnomeprintui/gnome-font-dialog.h>

#define RVAL2GPFS(obj) (GNOME_FONT_SELECTION(RVAL2GOBJ(obj)))
#define RVAL2GPFP(obj) (GNOME_FONT_PREVIEW(RVAL2GOBJ(obj)))
#define RVAL2GPFD(obj) (GNOME_FONT_DIALOG(RVAL2GOBJ(obj)))
#define GPFS2RVAL(obj) (GOBJ2RVAL(obj))
#define GPFP2RVAL(obj) (GOBJ2RVAL(obj))
#define GPFD2RVAL(obj) (GOBJ2RVAL(obj))

/* GnomeFontSelection */
static VALUE
gpui_fs_new(VALUE self)
{
    RBGTK_INITIALIZE(self, gnome_font_selection_new());
    return Qnil;
}

static VALUE
gpui_fs_get_face(VALUE self)
{
    return GOBJ2RVAL(gnome_font_selection_get_face(RVAL2GPFS(self)));
}

static VALUE
gpui_fs_get_size(VALUE self)
{
    return rb_float_new(gnome_font_selection_get_size(RVAL2GPFS(self)));
}

static VALUE
gpui_fs_get_font(VALUE self)
{
    return GOBJ2RVAL(gnome_font_selection_get_font(RVAL2GPFS(self)));
}

static VALUE
gpui_fs_set_font(VALUE self, VALUE font)
{
    gnome_font_selection_set_font(RVAL2GPFS(self), RVAL2GOBJ(font));
    return self;
}

/* GnomeFontPreview */
static VALUE
gpui_fp_new(VALUE self)
{
    RBGTK_INITIALIZE(self, gnome_font_preview_new());
    return Qnil;
}

static VALUE
gpui_fp_set_phrase(VALUE self, VALUE phrase)
{
    gnome_font_preview_set_phrase(RVAL2GPFP(self),
                                  (const guchar *)StringValuePtr(phrase));
    return self;
}

static VALUE
gpui_fp_set_font(VALUE self, VALUE font)
{
    gnome_font_preview_set_font(RVAL2GPFP(self), RVAL2GOBJ(font));
    return self;
}

static VALUE
gpui_fp_set_color(VALUE self, VALUE color)
{
    gnome_font_preview_set_color(RVAL2GPFP(self), NUM2UINT(color));
    return self;
}

/* GnomeFontDialog */
static VALUE
gpui_fd_new(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_title;
    gchar *title;

    rb_scan_args(argc, argv, "01", &rb_title);

    if (NIL_P(rb_title)) {
        title = "Gnome::FontDialog";
    } else {
        title = StringValuePtr(rb_title);
    }

    RBGTK_INITIALIZE(self, gnome_font_dialog_new(title));
    return Qnil;
}

static VALUE
gpui_fd_get_fontsel(VALUE self)
{
    return GOBJ2RVAL(gnome_font_dialog_get_fontsel(RVAL2GPFD(self)));
}

static VALUE
gpui_fd_get_preview(VALUE self)
{
    return GOBJ2RVAL(gnome_font_dialog_get_preview(RVAL2GPFD(self)));
}
#endif

void
Init_gnome_font_dialog(VALUE mGnome)
{
#ifdef HAVE_LIBGNOMEPRINTUI_GNOME_FONT_DIALOG_H
    VALUE cFontSelection =
        G_DEF_CLASS(GNOME_TYPE_FONT_SELECTION, "FontSelection", mGnome);
    VALUE cFontPreview =
        G_DEF_CLASS(GNOME_TYPE_FONT_PREVIEW, "FontPreview", mGnome);
    VALUE cFontDialog =
        G_DEF_CLASS(GNOME_TYPE_FONT_DIALOG, "FontDialog", mGnome);

    /* GnomeFontSelection */
    rb_define_method(cFontSelection, "initialize", gpui_fs_new, 0);
    rb_define_method(cFontSelection, "face", gpui_fs_get_face, 0);
    rb_define_method(cFontSelection, "size", gpui_fs_get_size, 0);
    rb_define_method(cFontSelection, "font", gpui_fs_get_font, 0);
    rb_define_method(cFontSelection, "set_font", gpui_fs_set_font, 1);
    G_DEF_SETTERS(cFontSelection);

    /* GnomeFontPreview */
    rb_define_method(cFontPreview, "initialize", gpui_fp_new, 0);
    rb_define_method(cFontPreview, "set_phrase", gpui_fp_set_phrase, 1);
    rb_define_method(cFontPreview, "set_font", gpui_fp_set_font, 1);
    rb_define_method(cFontPreview, "set_color", gpui_fp_set_color, 1);
    G_DEF_SETTERS(cFontPreview);

    /* GnomeFontDialog */
    rb_define_method(cFontDialog, "initialize", gpui_fd_new, -1);
    rb_define_method(cFontDialog, "fontsel", gpui_fd_get_fontsel, 0);
    rb_define_method(cFontDialog, "preview", gpui_fd_get_preview, 0);
    G_DEF_SETTERS(cFontDialog);
#endif
}
