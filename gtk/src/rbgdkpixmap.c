/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkpixmap.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk::Pixmap
 */
VALUE gdkPixmap;

static VALUE
gdkpmap_s_new(self, win, w, h, depth)
    VALUE self, win, w, h, depth;
{
    GdkPixmap *new;
    GdkWindow *window = get_gdkwindow(win);

    new = gdk_pixmap_new(window, NUM2INT(w), NUM2INT(h), NUM2INT(depth));
    return new_gdkpixmap(new);
}

static VALUE
gdkpmap_create_from_data(self, win, data, w, h, depth, fg, bg)
    VALUE self, win, data, w, h, depth, fg, bg;
{
    GdkPixmap *new;
    GdkWindow *window = get_gdkwindow(win);

    Check_Type(data, T_STRING);
    new = gdk_pixmap_create_from_data(window,
				      RSTRING(data)->ptr,
				      NUM2INT(w), NUM2INT(h),
				      NUM2INT(depth),
				      get_gdkcolor(fg),
				      get_gdkcolor(bg));
    return new_gdkpixmap(new);
}

static VALUE
gdkpmap_create_from_xpm(self, win, tcolor, fname)
    VALUE self, win, tcolor, fname;
{
    GdkPixmap *new;
    GdkBitmap *mask;
    GdkWindow *window = get_gdkwindow(win);

    new = gdk_pixmap_create_from_xpm(window, &mask,
				     get_gdkcolor(tcolor),
				     STR2CSTR(fname));
    if (!new) {
	rb_raise(rb_eArgError, "Pixmap not created from %s", STR2CSTR(fname));
    }
    return rb_assoc_new(new_gdkpixmap(new),
			new_gdkbitmap(mask));
}

static VALUE
gdkpmap_create_from_xpm_d(self, win, tcolor, data)
    VALUE self, win, tcolor, data;
{
    GdkPixmap *new;
    GdkBitmap *mask;
    GdkWindow *window = get_gdkwindow(win);
    int i;
    gchar **buf;

    Check_Type(data, T_ARRAY);
    buf = ALLOCA_N(char*, RARRAY(data)->len);
    for (i=0; i<RARRAY(data)->len; i++) {
	buf[i] = STR2CSTR(RARRAY(data)->ptr[i]);
    }

    new = gdk_pixmap_create_from_xpm_d(window, &mask,
				       get_gdkcolor(tcolor),
				       buf);

    return rb_assoc_new(new_gdkpixmap(new),
			new_gdkbitmap(mask));
}

static VALUE
gdkpmap_colormap_create_from_xpm(self, win, colormap, tcolor, fname)
    VALUE self, win, colormap, tcolor, fname;
{
    GdkPixmap *new;
    GdkBitmap *mask;
    GdkWindow *window = get_gdkwindow(win);

    new = gdk_pixmap_colormap_create_from_xpm(window, 
					      get_gdkcmap(colormap),
					      &mask,
					      get_gdkcolor(tcolor),
					      STR2CSTR(fname));
    if (!new) {
	rb_raise(rb_eArgError, "Pixmap not created from %s", STR2CSTR(fname));
    }
    return rb_assoc_new(new_gdkpixmap(new),
			new_gdkbitmap(mask));
}

static VALUE
gdkpmap_colormap_create_from_xpm_d(self, win, colormap, tcolor, data)
    VALUE self, win, colormap, tcolor, data;
{
    GdkPixmap *new;
    GdkBitmap *mask;
    GdkWindow *window = get_gdkwindow(win);
    int i;
    gchar **buf;

    Check_Type(data, T_ARRAY);
    buf = ALLOCA_N(char*, RARRAY(data)->len);
    for (i=0; i<RARRAY(data)->len; i++) {
	buf[i] = STR2CSTR(RARRAY(data)->ptr[i]);
    }

    new = gdk_pixmap_colormap_create_from_xpm_d(window,
						get_gdkcmap(colormap),
						&mask,
						get_gdkcolor(tcolor),
						buf);

    return rb_assoc_new(new_gdkpixmap(new),
			new_gdkbitmap(mask));
}


/*
 * Gdk::Bitmap
 */

VALUE gdkBitmap;

static VALUE
gdkbmap_s_new(self, win, w, h)
    VALUE self, win, w, h;
{
    GdkPixmap *new;
    GdkWindow *window = get_gdkwindow(win);

    new = gdk_pixmap_new(window, NUM2INT(w), NUM2INT(h), 1);
    return new_gdkbitmap(new);
}

static VALUE
gdkbmap_create_from_data(self, win, data, w, h)
    VALUE self, win, data, w, h;
{
    GdkBitmap *new;
    GdkWindow *window = get_gdkwindow(win);

    Check_Type(data, T_STRING);
    new = gdk_bitmap_create_from_data(window,
				      RSTRING(data)->ptr,
				      NUM2INT(w), NUM2INT(h));
    return new_gdkbitmap(new);
}

#ifdef HAVE_XREADBITMAPFILEDATA
static VALUE
gdkbmap_create_from_xbm(self, win, fname)
    VALUE self, win, fname;
{
    GdkBitmap *new;
    GdkWindow *window = get_gdkwindow(win);
    unsigned char *data;
    unsigned int width, height;
    int x, y;

    Check_Type(fname, T_STRING);
    if (XReadBitmapFileData(STR2CSTR(fname), &width, &height, &data, &x, &y))
        rb_raise(rb_eArgError, "Bitmap not created from %s", STR2CSTR(fname));
    new = gdk_bitmap_create_from_data(window, data, width, height);
    XFree(data);
    return new_gdkbitmap(new);
}
#endif /* HAVE_XREADBITMAPFILEDATA */


void
Init_gtk_gdk_pixmap()
{
    /* 
     * GdkPixmap
     */
    gdkPixmap = rb_define_class_under(mGdk, "Pixmap", gdkDrawable);

    rb_define_singleton_method(gdkPixmap, "new", gdkpmap_s_new, 4);
    rb_define_singleton_method(gdkPixmap, "create_from_data",
			       gdkpmap_create_from_data, 7);
    rb_define_singleton_method(gdkPixmap, "create_from_xpm",
			       gdkpmap_create_from_xpm, 3);
    rb_define_singleton_method(gdkPixmap, "create_from_xpm_d",
			       gdkpmap_create_from_xpm_d, 3);
    rb_define_singleton_method(gdkPixmap, "colormap_create_from_xpm",
			       gdkpmap_colormap_create_from_xpm, 4);
    rb_define_singleton_method(gdkPixmap, "colormap_create_from_xpm_d",
			       gdkpmap_colormap_create_from_xpm_d, 4);

    /*
     * GdkBitmap
     */
    gdkBitmap = rb_define_class_under(mGdk, "Bitmap", gdkPixmap);

    rb_define_singleton_method(gdkBitmap, "new", gdkbmap_s_new, 3);
    rb_define_singleton_method(gdkBitmap, "create_from_data",
			       gdkbmap_create_from_data, 4);
#ifdef HAVE_XREADBITMAPFILEDATA
    rb_define_singleton_method(gdkBitmap, "create_from_xbm",
			       gdkbmap_create_from_xbm, 2);
#endif /* HAVE_XREADBITMAPFILEDATA */
}
