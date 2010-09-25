/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpixbuf.c -

  $Author: ktou $
  $Date: 2006/10/14 04:59:14 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s)) 

static VALUE
pixbuf_render_threshold_alpha(self, bitmap, src_x, src_y, dest_x, dest_y,
                              width, height, alpha_threshold)
    VALUE self, bitmap, src_x, src_y, dest_x, dest_y,
    width, height, alpha_threshold;
{
    gdk_pixbuf_render_threshold_alpha(_SELF(self), GDK_BITMAP(RVAL2GOBJ(bitmap)),
                                      NUM2INT(src_x), NUM2INT(src_y),
                                      NUM2INT(dest_x), NUM2INT(dest_y),
                                      NUM2INT(width), NUM2INT(height),
                                      NUM2INT(alpha_threshold));
    return self;
}

static VALUE
pixbuf_render_to_drawable(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE gc, src_x, src_y, dest_x, dest_y, width, height,
        dither, x_dither, y_dither;

#if GTK_CHECK_VERSION(2,2,0)
    rb_warn("Gdk::Pixbuf#render_to_drawable is obsolete. Use Gdk::Drawable#draw_pixbuf instead.");
#endif

    rb_scan_args(argc, argv, "73", &gc, &src_x, &src_y, &dest_x, &dest_y,
                 &width, &height, &dither, &x_dither, &y_dither);

    if (NIL_P(gc))
        rb_raise(rb_eArgError, "arguments 1 must be non nil");
    if (NIL_P(src_x))
        rb_raise(rb_eArgError, "arguments 2 must be non nil");
    if (NIL_P(src_y))
        rb_raise(rb_eArgError, "arguments 3 must be non nil");
    if (NIL_P(dest_x))
        rb_raise(rb_eArgError, "arguments 4 must be non nil");
    if (NIL_P(dest_y))
        rb_raise(rb_eArgError, "arguments 5 must be non nil");
    if (NIL_P(width))
        rb_raise(rb_eArgError, "arguments 6 must be non nil");
    if (NIL_P(height))
        rb_raise(rb_eArgError, "arguments 7 must be non nil");

    gdk_pixbuf_render_to_drawable(_SELF(self),
				  GDK_DRAWABLE(RVAL2GOBJ(self)),
				  GDK_GC(RVAL2GOBJ(gc)),
				  NUM2INT(src_x), NUM2INT(src_y),
                                  NUM2INT(dest_x), NUM2INT(dest_y),
                                  NUM2INT(width), NUM2INT(height),
                                  NIL_P(dither) ? GDK_RGB_DITHER_NONE : RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                                  NIL_P(x_dither) ? 0 : NUM2INT(x_dither), 
                                  NIL_P(y_dither) ? 0 : NUM2INT(y_dither));
    return self;
}

static VALUE
pixbuf_render_pixmap_and_mask(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE colormap_or_alpha, alpha;
    GdkPixmap *pixmap;
    GdkBitmap *mask;

    rb_scan_args(argc, argv, "11", &colormap_or_alpha, &alpha);

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
            return GOBJ2RVALU(buf);
        } else {
            return Qnil;
        }
    } else {
        return dest;
    }
}
 
static VALUE
pixbuf_s_from_image(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE cmap, src, src_x, src_y, width, height, dest, dest_x, dest_y;
    GdkPixbuf* buf;

    rb_scan_args(argc, argv, "63", &cmap, &src, &src_x, &src_y, &width, &height, 
                 &dest, &dest_x, &dest_y);

    buf = gdk_pixbuf_get_from_image(GDK_PIXBUF(RVAL2GOBJ(dest)),
                                    GDK_IMAGE(RVAL2GOBJ(src)),
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

    rb_define_method(gdkPixbuf, "render_threshold_alpha", pixbuf_render_threshold_alpha, 8);
    rb_define_method(gdkPixbuf, "render_to_drawable", pixbuf_render_to_drawable, -1);
    rb_define_method(gdkPixbuf, "render_pixmap_and_mask", pixbuf_render_pixmap_and_mask, -1);

    rb_define_singleton_method(gdkPixbuf, "from_drawable", pixbuf_s_from_drawable, -1);
    rb_define_singleton_method(gdkPixbuf, "from_image", pixbuf_s_from_image, -1);
}
