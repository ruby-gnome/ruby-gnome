/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpixbuf.c -

  $Author: mutoh $
  $Date: 2003/10/21 13:05:38 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s)) 

static VALUE
render_to_drawable(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE args[11];
    int i;
    int dither = GDK_RGB_DITHER_NONE;
    int x_dither = 0;
    int y_dither = 0;

    rb_scan_args(argc, argv, "83", &args[0], &args[1], &args[2], &args[3], 
                 &args[4], &args[5], &args[6], &args[7], &args[8], &args[9], &args[10]);

    for (i=0; i<8; i++) {
	if (NIL_P(args[i]))
	    rb_raise(rb_eArgError, "arguments %d must be non nil", i);
    }

    if (!NIL_P(args[8]))
	dither = RVAL2GENUM(args[8], GDK_TYPE_RGB_DITHER);
    if (!NIL_P(args[9]))
	x_dither = NUM2INT(args[9]);
    if (!NIL_P(args[10]))
	y_dither = NUM2INT(args[10]);

    gdk_pixbuf_render_to_drawable(_SELF(self),
				  GDK_DRAWABLE(RVAL2GOBJ(args[0])),
				  GDK_GC(RVAL2GOBJ(args[1])),
				  NUM2INT(args[2]),
				  NUM2INT(args[3]),
				  NUM2INT(args[4]),
				  NUM2INT(args[5]),
				  NUM2INT(args[6]),
				  NUM2INT(args[7]),
				  dither, x_dither, y_dither);
    return self;
}

static VALUE
render_pm(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE colormap_or_alpha, alpha;
    GdkPixmap *pixmap;
    GdkBitmap *mask;

    rb_scan_args(argc, argv, "01", &colormap_or_alpha, &alpha);

    if (rb_obj_is_kind_of(colormap_or_alpha, GTYPE2CLASS(GDK_TYPE_COLORMAP))){
        gdk_pixbuf_render_pixmap_and_mask_for_colormap(_SELF(self),
                                                       RVAL2GOBJ(colormap_or_alpha),
                                                       &pixmap,
                                                       &mask,
                                                       NIL_P(alpha)?0:NUM2INT(alpha));
    } else {
        gdk_pixbuf_render_pixmap_and_mask(_SELF(self),
                                          &pixmap,
                                          &mask,
                                          NIL_P(colormap_or_alpha)?
                                          0:NUM2INT(colormap_or_alpha));
    }

    return rb_ary_new3(2,
                       pixmap ? GOBJ2RVAL(pixmap) : Qnil,
                       mask ? GOBJ2RVAL(mask) : Qnil);
}

static VALUE
pixbuf_s_from_drawable(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE cmap, src, src_x, src_y, width, height, dest, dest_x, dest_y;
    GdkPixbuf* buf;

    rb_scan_args(argc, argv, "63", &cmap, &src, &src_x, &src_y, &width, &height, 
                 &dest, &dest_x, &dest_y);

    buf = gdk_pixbuf_get_from_drawable(GDK_PIXBUF(RVAL2GOBJ(dest)),
                                       GDK_DRAWABLE(RVAL2GOBJ(src)),
                                       GDK_COLORMAP(RVAL2GOBJ(cmap)),
                                       NUM2INT(src_x), NUM2INT(src_y),
                                       NIL_P(dest_x) ? 0 : NUM2INT(dest_x),
                                       NIL_P(dest_y) ? 0 : NUM2INT(dest_y),
                                       NUM2INT(width), NUM2INT(height));
    if (NIL_P(dest)) {
        if (buf) {
            return GOBJ2RVAL(buf);
        } else {
            return Qnil;
        }
    } else {
        return dest;
    }
}
 
void
Init_gtk_gdk_pixbuf()
{
    /*
     *  This defines Gdk::Pixbuf methods of GDK side.
     */
    VALUE gdkPixbuf = GTYPE2CLASS(GDK_TYPE_PIXBUF);

    /* 
     * Rendering
     */
    /* rb_define_method(gdkPixbuf, "render_to_drawable_alpha", ..., ...,); */
    rb_define_method(gdkPixbuf, "render_to_drawable", render_to_drawable, -1);
    /* rb_define_method(gdkPixbuf, "render_threshold_alpha", ..., ...); */
    rb_define_method(gdkPixbuf, "render_pixmap_and_mask", render_pm, -1);

    rb_define_singleton_method(gdkPixbuf, "from_drawable", pixbuf_s_from_drawable, -1);
}
