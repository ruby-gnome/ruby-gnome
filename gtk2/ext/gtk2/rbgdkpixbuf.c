/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cPixbuf
#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s)) 

static VALUE
rg_render_threshold_alpha(VALUE self, VALUE bitmap, VALUE src_x, VALUE src_y, VALUE dest_x, VALUE dest_y, VALUE width, VALUE height, VALUE alpha_threshold)
{
    gdk_pixbuf_render_threshold_alpha(_SELF(self), GDK_BITMAP(RVAL2GOBJ(bitmap)),
                                      NUM2INT(src_x), NUM2INT(src_y),
                                      NUM2INT(dest_x), NUM2INT(dest_y),
                                      NUM2INT(width), NUM2INT(height),
                                      NUM2INT(alpha_threshold));
    return self;
}

static VALUE
rg_render_to_drawable(int argc, VALUE *argv, VALUE self)
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
rg_render_pixmap_and_mask(int argc, VALUE *argv, VALUE self)
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
rg_s_from_drawable(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_s_from_image(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
Init_gtk_gdk_pixbuf(VALUE mGdk)
{
    /*
     *  This defines Gdk::Pixbuf methods of GDK side.
     */
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS(GDK_TYPE_PIXBUF);

    RG_DEF_METHOD(render_threshold_alpha, 8);
    RG_DEF_METHOD(render_to_drawable, -1);
    RG_DEF_METHOD(render_pixmap_and_mask, -1);

    RG_DEF_SMETHOD(from_drawable, -1);
    RG_DEF_SMETHOD(from_image, -1);
}
