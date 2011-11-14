/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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
#ifdef GDK_WINDOWING_X11

#define RG_TARGET_NAMESPACE mX11

/* Can't implement
#define     GDK_ROOT_WINDOW                 ()
*/
/* Deprecated.
#define     GDK_ROOT_PARENT                 ()
*/
/* Can't implement
#define     GDK_DISPLAY                     ()
#define     GDK_WINDOW_XDISPLAY             (win)
*/
/* Use Gdk::Drawable#xid
#define     GDK_WINDOW_XID                  (win)
*/
/* Same as GDK_WINDOW_*
#define     GDK_PIXMAP_XDISPLAY             (pix)
#define     GDK_PIXMAP_XID                  (pix)
#define     GDK_DISPLAY_XDISPLAY            (display)
#define     GDK_DRAWABLE_XDISPLAY           (win)
#define     GDK_DRAWABLE_XID                (win)
*/
/* Can't implement
#define     GDK_IMAGE_XDISPLAY              (image)
#define     GDK_IMAGE_XIMAGE                (image)
#define     GDK_GC_XDISPLAY                 (gc)
#define     GDK_COLORMAP_XDISPLAY           (cmap)
#define     GDK_COLORMAP_XCOLORMAP          (cmap)
#define     GDK_SCREEN_XDISPLAY             (screen)
*/
/* Implement at Gdk::Screen
#define     GDK_SCREEN_XNUMBER              (screen)
*/
/* Can't implement
#define     GDK_SCREEN_XSCREEN              (screen)
#define     GDK_VISUAL_XVISUAL              (vis)
*/
/* Deprecated.
#define     GDK_FONT_XDISPLAY               (font)
#define     GDK_FONT_XFONT                  (font)
*/
/* Can't implement.
#define     GDK_CURSOR_XCURSOR              (cursor)
#define     GDK_CURSOR_XDISPLAY             (cursor)
#define     GDK_GC_XGC                      (gc)
#define     GDK_GC_GET_XGC                  (gc)
*/
/* Don't need this.
#define     GDK_WINDOW_XWINDOW
*/
/* Can't implement.
GdkVisual*  gdkx_visual_get                 (VisualID xvisualid);
GdkColormap* gdkx_colormap_get              (Colormap xcolormap);
*/
/* Implement at Gdk::Pixmap.
GdkPixmap*  gdk_pixmap_foreign_new          (GdkNativeWindow anid);
GdkPixmap*  gdk_pixmap_foreign_new_for_display
                                            (GdkDisplay *display,
                                             GdkNativeWindow anid);
*/
/* Implement at Gdk::Window.
GdkWindow*  gdk_window_foreign_new          (GdkNativeWindow anid);
GdkWindow*  gdk_window_foreign_new_for_display
                                            (GdkDisplay *display,
                                             GdkNativeWindow anid);
*/

static VALUE
rg_m_xid_table_lookup(int argc, VALUE *argv, VALUE self)
{
    VALUE arg[2];
    GdkPixmap* win = NULL;

    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
        win = gdk_xid_table_lookup(NUM2UINT(arg[0]));
        break;
      case 2:
#if GTK_CHECK_VERSION(2,2,0)
        win = gdk_xid_table_lookup_for_display(RVAL2GOBJ(arg[0]), NUM2UINT(arg[1])); 
#else
        win = gdk_xid_table_lookup(NUM2UINT(arg[1])); 
        rb_warn("Not supported in GTK+-2.0.x.");
#endif
        break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}

/* Implement at Gdk::Window
GdkWindow*  gdk_window_lookup               (GdkNativeWindow anid);
GdkWindow*  gdk_window_lookup_for_display   (GdkDisplay *display,
                                             GdkNativeWindow anid);
*/
/* Implement at Gdk::Pixmap
GdkPixmap*  gdk_pixmap_lookup               (GdkNativeWindow anid);
GdkPixmap*  gdk_pixmap_lookup_for_display   (GdkDisplay *display,
                                             GdkNativeWindow anid);
*/
/* Deprecated.
#define     gdk_font_lookup                 (xid)
#define     gdk_font_lookup_for_display     (display, xid)
*/
/* Can't implement.
GdkDisplay* gdk_x11_lookup_xdisplay         (Display *xdisplay);
*/
/* Implement at Gdk::Window
guint32     gdk_x11_get_server_time         (GdkWindow *window);
*/

static VALUE
rg_m_net_wm_supports_p(VALUE self, VALUE property)
{
    return CBOOL2RVAL(gdk_net_wm_supports(RVAL2ATOM(property)));
}

/* Implement at Gdk::Screen
gboolean    gdk_x11_screen_supports_net_wm_hint
                                            (GdkScreen *screen,
                                             GdkAtom property);
const char* gdk_x11_screen_get_window_manager_name
                                            (GdkScreen *screen);
*/
/* Can't implement
GdkVisual*  gdk_x11_screen_lookup_visual    (GdkScreen *screen,
                                             VisualID xvisualid);
GdkColormap* gdk_x11_colormap_foreign_new   (GdkVisual *visual,
                                             Colormap xcolormap);
Colormap    gdk_x11_colormap_get_xcolormap  (GdkColormap *colormap);
Display*    gdk_x11_colormap_get_xdisplay   (GdkColormap *colormap);
Cursor      gdk_x11_cursor_get_xcursor      (GdkCursor *cursor);
Display*    gdk_x11_cursor_get_xdisplay     (GdkCursor *cursor);
Display*    gdk_x11_display_get_xdisplay    (GdkDisplay *display);
*/

/* Implement at Gdk::Display
void        gdk_x11_display_grab            (GdkDisplay *display);
void        gdk_x11_display_ungrab          (GdkDisplay *display);
void        gdk_x11_register_standard_event_type
                                            (GdkDisplay *display,
                                             gint event_base,
                                             gint n_events);
*/
/* Can't implement
Display*    gdk_x11_drawable_get_xdisplay   (GdkDrawable *drawable);
*/
/* Use Gdk::Drawable#xid instead
XID         gdk_x11_drawable_get_xid        (GdkDrawable *drawable);
*/
/* Deprecated
G_CONST_RETURN char* gdk_x11_font_get_name  (GdkFont *font);
Display*    gdk_x11_font_get_xdisplay       (GdkFont *font);
gpointer    gdk_x11_font_get_xfont          (GdkFont *font);
*/
/* Can't implement
Display*    gdk_x11_gc_get_xdisplay         (GdkGC *gc);
GC          gdk_x11_gc_get_xgc              (GdkGC *gc);
Window      gdk_x11_get_default_root_xwindow
                                            (void);
*/

static VALUE
rg_m_default_screen(VALUE self)
{
    return INT2NUM(gdk_x11_get_default_screen());
}
/* Can't implement
Display*    gdk_x11_get_default_xdisplay    (void);
*/

static VALUE
rg_m_grab_server(VALUE self)
{
    gdk_x11_grab_server();
    return Qnil;
}
/* Can't implement 
Display*    gdk_x11_image_get_xdisplay      (GdkImage *image);
XImage*     gdk_x11_image_get_ximage        (GdkImage *image);
*/
/* Implement at Gdk::Screen.
int         gdk_x11_screen_get_screen_number
                                            (GdkScreen *screen);
*/
/* Can't impelement
Screen*     gdk_x11_screen_get_xscreen      (GdkScreen *screen);
*/

static VALUE
rg_m_ungrab_server(VALUE self)
{
    gdk_x11_ungrab_server();
    return Qnil;
}
/* Can't implement
Visual*     gdk_x11_visual_get_xvisual      (GdkVisual *visual);

Atom        gdk_x11_atom_to_xatom           (GdkAtom atom);
Atom        gdk_x11_atom_to_xatom_for_display
                                            (GdkDisplay *display,
                                             GdkAtom atom);
GdkAtom     gdk_x11_xatom_to_atom           (Atom xatom);
GdkAtom     gdk_x11_xatom_to_atom_for_display
                                            (GdkDisplay *display,
                                             Atom xatom);
Atom        gdk_x11_get_xatom_by_name       (const gchar *atom_name);
Atom        gdk_x11_get_xatom_by_name_for_display
                                            (GdkDisplay *display,
                                             const gchar *atom_name);
G_CONST_RETURN gchar* gdk_x11_get_xatom_name
                                            (Atom xatom);
G_CONST_RETURN gchar* gdk_x11_get_xatom_name_for_display
                                            (GdkDisplay *display,
                                             Atom xatom);
*/
#endif
void
Init_gtk_gdk_x11(VALUE mGdk)
{
#ifdef GDK_WINDOWING_X11
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "X11");

    RG_DEF_MODFUNC(xid_table_lookup, -1);
    RG_DEF_MODFUNC_P(net_wm_supports, 1);

    RG_DEF_MODFUNC(default_screen, 0);
    RG_DEF_MODFUNC(grab_server, 0);

    RG_DEF_MODFUNC(ungrab_server, 0);
#endif
}
