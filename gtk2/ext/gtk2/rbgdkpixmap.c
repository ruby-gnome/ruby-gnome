/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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
#ifdef HAVE_XREADBITMAPFILEDATA
#include <X11/Xlib.h>
#endif /* HAVE_XREADBITMAPFILEDATA */

#define RG_TARGET_NAMESPACE cPixmap

static VALUE
rg_initialize(VALUE self, VALUE win, VALUE w, VALUE h, VALUE depth)
{
    G_INITIALIZE(self, gdk_pixmap_new(GDK_WINDOW(RVAL2GOBJ(win)), 
                                      NUM2INT(w), NUM2INT(h), 
                                      NUM2INT(depth)));
    return Qnil;
}

static VALUE
rg_s_create_from_data(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE win, data, w, h, depth, fg, bg, ret;

    rb_scan_args(argc, argv, "43", &win, &data, &w, &h, &depth, &fg, &bg);

    StringValue(data);
    if (NIL_P(depth)){
        ret = GOBJ2RVAL(gdk_bitmap_create_from_data(NIL_P(win) ? NULL : GDK_WINDOW(RVAL2GOBJ(win)),
                                                    RVAL2CSTR(data), NUM2INT(w), NUM2INT(h)));
    } else {
        ret = GOBJ2RVAL(gdk_pixmap_create_from_data(NIL_P(win) ? NULL : GDK_WINDOW(RVAL2GOBJ(win)),
                                                    RVAL2CSTR(data), NUM2INT(w), NUM2INT(h), 
                                                    NUM2INT(depth),
                                                    RVAL2GDKCOLOR(fg),
                                                    RVAL2GDKCOLOR(bg)));
    }
    return ret;
}

static VALUE
rg_s_create_from_xpm(G_GNUC_UNUSED VALUE self, VALUE win, VALUE color, VALUE fname)
{
    GdkPixmap *result;
    GdkBitmap *mask;

    result = gdk_pixmap_create_from_xpm(GDK_WINDOW(RVAL2GOBJ(win)),
                                        &mask,
                                        RVAL2GDKCOLOR(color),
                                        RVAL2CSTR(fname));
    if (result == NULL)
        rb_raise(rb_eArgError, "Pixmap not created from %s", RVAL2CSTR(fname));

    return rb_assoc_new(GOBJ2RVAL(result), GOBJ2RVAL(mask));
}

static VALUE
rg_s_create_from_xpm_d(G_GNUC_UNUSED VALUE self, VALUE win, VALUE tcolor, VALUE data)
{
    GdkPixmap *result;
    GdkBitmap *mask;
    const gchar **buf = RVAL2STRV(data);

    result = gdk_pixmap_create_from_xpm_d(GDK_WINDOW(RVAL2GOBJ(win)),
                                          &mask,
                                          RVAL2GDKCOLOR(tcolor),
                                          (gchar **)buf);

    g_free(buf);

    return rb_assoc_new(GOBJ2RVAL(result), GOBJ2RVAL(mask));
}

static VALUE
rg_s_colormap_create_from_xpm(G_GNUC_UNUSED VALUE self, VALUE win, VALUE colormap, VALUE tcolor, VALUE fname)
{
    GdkPixmap *result;
    GdkBitmap *mask;

    result = gdk_pixmap_colormap_create_from_xpm(NIL_P(win) ? NULL : GDK_WINDOW(RVAL2GOBJ(win)), 
                                                 GDK_COLORMAP(RVAL2GOBJ(colormap)),
                                                 &mask,
                                                 RVAL2GDKCOLOR(tcolor),
                                                 RVAL2CSTR(fname));
    if (result == NULL)
        rb_raise(rb_eArgError, "Pixmap not created from %s", RVAL2CSTR(fname));

    return rb_assoc_new(GOBJ2RVAL(result), GOBJ2RVAL(mask));
}

static VALUE
rg_s_colormap_create_from_xpm_d(G_GNUC_UNUSED VALUE self, VALUE win, VALUE colormap, VALUE tcolor, VALUE data)
{
    GdkPixmap *result;
    GdkBitmap *mask;
    const gchar **buf = RVAL2STRV(data);

    result = gdk_pixmap_colormap_create_from_xpm_d(NIL_P(win) ? NULL : GDK_WINDOW(RVAL2GOBJ(win)),
                                                   GDK_COLORMAP(RVAL2GOBJ(colormap)),
                                                   &mask,
                                                   RVAL2GDKCOLOR(tcolor),
                                                   (gchar **)buf);

    return rb_assoc_new(GOBJ2RVAL(result), GOBJ2RVAL(mask));
}

#ifdef HAVE_XREADBITMAPFILEDATA
static VALUE
rg_s_create_from_xbm(VALUE self, VALUE win, VALUE fname)
{
    GdkBitmap *new;
    unsigned char *data;
    unsigned int width, height;
    int x, y;

    StringValue(fname);
    if (XReadBitmapFileData(RVAL2CSTR(fname), &width, &height, &data, &x, &y))
        rb_raise(rb_eArgError, "Bitmap not created from %s", RVAL2CSTR(fname));

    new = gdk_bitmap_create_from_data(GDK_WINDOW(RVAL2GOBJ(win)), 
                                      (const gchar*)data, width, height);
    XFree(data);
    return GOBJ2RVAL(new);
}
#endif /* HAVE_XREADBITMAPFILEDATA */

/* From X Window System Interaction */
/*
  Gdk::Window.foreign_new(anid)
  Gdk::Window.foreign_new(display, anid)                       # Since GTK+-2.2
  Gdk::Window.foreign_new(screen, anid, width, height, depth)  # Since GTK+-2.10
 */

static VALUE
rg_s_foreign_new(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[5];
    GdkPixmap* win = NULL;

    rb_scan_args(argc, argv, "14", &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]);

    switch(argc)
        {
        case 1:
            win = gdk_pixmap_foreign_new(RVAL2GDKNATIVEWINDOW(arg[0]));
            break;
        case 2:
#if GTK_CHECK_VERSION(2,2,0)
            win = gdk_pixmap_foreign_new_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(arg[0])), 
                                                     RVAL2GDKNATIVEWINDOW(arg[1])); 
#else
            win = gdk_pixmap_foreign_new(RVAL2GDKNATIVEWINDOW(arg[1])); 
            rb_warn("Not supported. Use GTK+-2.2.0 or later.");
#endif
            break;
        case 5:
#if GTK_CHECK_VERSION(2,10,0)
            win = gdk_pixmap_foreign_new_for_screen(GDK_SCREEN(RVAL2GOBJ(arg[0])), 
                                                    RVAL2GDKNATIVEWINDOW(arg[1]), /* anid */
                                                    NUM2INT(arg[2]),  /* width */
                                                    NUM2INT(arg[3]),  /* height */
                                                    NUM2INT(arg[4])); /* depth */
#else
            win = gdk_pixmap_foreign_new(NUM2UINT(arg[1])); 
            rb_warn("Not supported. GTK+-2.10.0 or later.");
#endif
            break;
        default:
            break;
        }
    return win ? GOBJ2RVAL(win) : Qnil;
}

static VALUE
rg_s_lookup(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[2];
    GdkPixmap* win = NULL;

    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
        win = gdk_pixmap_lookup(RVAL2GDKNATIVEWINDOW(arg[0]));
        break;
      case 2:
#if GTK_CHECK_VERSION(2,2,0)
        win = gdk_pixmap_lookup_for_display(RVAL2GOBJ(arg[0]), RVAL2GDKNATIVEWINDOW(arg[1])); 
#else
        win = gdk_pixmap_lookup(RVAL2GDKNATIVEWINDOW(arg[1])); 
        rb_warn("Not supported in GTK+-2.0.x.");
#endif
        break;
    default:
        break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}

void
Init_gtk_gdk_pixmap(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXMAP, "Pixmap", mGdk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_SMETHOD(create_from_data, -1);
    RG_DEF_SMETHOD(create_from_xpm, 3);
    RG_DEF_SMETHOD(create_from_xpm_d, 3);
    RG_DEF_SMETHOD(colormap_create_from_xpm, 4);
    RG_DEF_SMETHOD(colormap_create_from_xpm_d, 4);
#ifdef HAVE_XREADBITMAPFILEDATA
    RG_DEF_SMETHOD(create_from_xbm, 2);
#endif /* HAVE_XREADBITMAPFILEDATA */
#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkPixmapImplX11", "PixmapImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkPixmapImplWin32", "PixmapImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkPixmapFB", "PixmapFB", mGdk);
#endif

    RG_DEF_SMETHOD(foreign_new, -1);
    RG_DEF_SMETHOD(lookup, -1);

}
