/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkwindow.c -

  $Author: geoff_youngs $
  $Date: 2003/06/09 14:10:12 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) GDK_WINDOW(RVAL2GOBJ(s))

/*
static VALUE
gdkwin_initialize(self, parent, attributes, attributes_mask)
    VALUE self;
    VALUE red, green, blue;
{
    GdkWindow* win;

    win = gdk_window_new(_SELF(parent), xxx, NUM2INT(attributes_mask));
    G_INITIALIZE(self, win);
    return Qnil;
}
GdkWindow*  gdk_window_new                  (GdkWindow *parent,
                                             GdkWindowAttr *attributes,
                                             gint attributes_mask);
*/

static VALUE
gdkwin_get_window_type(self)
    VALUE self;
{
    return INT2FIX(gdk_window_get_window_type(_SELF(self)));
}

static VALUE
gdkwin_s_at_pointer(self)
    VALUE self;
{
    gint x, y;
    GdkWindow* win = gdk_window_at_pointer(&x, &y);
    
    if (win == NULL)
        return Qnil;
    else {
        return rb_ary_new3(3, GOBJ2RVAL(win), INT2FIX(x), INT2FIX(y));
    }
}

static VALUE
gdkwin_foreign(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg[2];
    GdkWindow * win = NULL;
    
    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
    case 1:
    	win = gdk_window_foreign_new(NUM2UINT(arg[0]));
	break;
    case 2:
#if GTK_MINOR_VERSION >= 2
    	win = gdk_window_foreign_new_for_display(RVAL2GOBJ(arg[0]), NUM2UINT(arg[1])); 
#else
    	win = gdk_window_foreign_new(NUM2UINT(arg[1])); 
	/* XXX: Should the program be warned? */
#endif
    	break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}

static VALUE
gdkwin_show(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE show;
    rb_scan_args(argc, argv, "01", &show);

    if (! NIL_P(show) && RTEST(show)){
        gdk_window_show_unraised(_SELF(self));
    } else {
        gdk_window_show(_SELF(self));
    }
    return self;
}

static VALUE
gdkwin_hide(self)
    VALUE self;
{
    gdk_window_hide(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_is_visible(self)
    VALUE self;
{
    return gdk_window_is_visible(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkwin_is_viewable(self)
    VALUE self;
{
    return gdk_window_is_viewable(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkwin_get_state(self)
    VALUE self;
{
    return INT2FIX(gdk_window_get_state(_SELF(self)));
}

/* I think it should be not implemented in Ruby
void        gdk_window_withdraw             (GdkWindow *window);
*/

static VALUE
gdkwin_window_iconify(self)
    VALUE self;
{
    gdk_window_iconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_window_deiconify(self)
    VALUE self;
{
    gdk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_window_stick(self)
    VALUE self;
{
    gdk_window_stick(_SELF(self));
    return self;
}

static VALUE
gdkwin_window_unstick(self)
    VALUE self;
{
    gdk_window_unstick(_SELF(self));
    return self;
}

static VALUE
gdkwin_window_maximize(self)
    VALUE self;
{
    gdk_window_maximize(_SELF(self));
    return self;
}

static VALUE
gdkwin_window_unmaximize(self)
    VALUE self;
{
    gdk_window_unmaximize(_SELF(self));
    return self;
}

static VALUE
gdkwin_move(self, x, y)
    VALUE self, x, y;
{
    gdk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_resize(self, w, h)
    VALUE self, w, h;
{
    gdk_window_resize(_SELF(self), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_move_resize(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    gdk_window_move_resize(_SELF(self), NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_scroll(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_window_scroll(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
gdkwin_reparent(self, new_parent, x, y)
    VALUE self, new_parent, x, y;
{
    gdk_window_reparent(_SELF(self), _SELF(new_parent), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_clear(self)
    VALUE self;
{
    gdk_window_clear(_SELF(self));
    return self;
}

static VALUE
gdkwin_clear_area(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE gen_expose, x, y, w, h;
    rb_scan_args(argc, argv, "01", &x, &y, &w, &h, &gen_expose);

    if (! NIL_P(gen_expose) && RTEST(gen_expose)){
        gdk_window_clear_area_e(_SELF(self),
                              NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    } else {
        gdk_window_clear_area(_SELF(self),
                              NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    }
    return self;
}

/* Obsolete
#define gdk_window_copy_area()
*/

static VALUE
gdkwin_raise(self)
    VALUE self;
{
    gdk_window_raise(_SELF(self));
    return self;
}

static VALUE
gdkwin_lower(self)
    VALUE self;
{
    gdk_window_lower(_SELF(self));
    return self;
}

static VALUE
gdkwin_focus(self, timestamp)
    VALUE self, timestamp;
{
    gdk_window_focus(_SELF(self), NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_register_dnd(self)
    VALUE self;
{
    gdk_window_register_dnd(_SELF(self));
    return self;
}

/*
void        gdk_window_begin_resize_drag    (GdkWindow *window,
                                             GdkWindowEdge edge,
                                             gint button,
                                             gint root_x,
                                             gint root_y,
                                             guint32 timestamp);
void        gdk_window_begin_move_drag      (GdkWindow *window,
                                             gint button,
                                             gint root_x,
                                             gint root_y,
                                             guint32 timestamp);
void        gdk_window_constrain_size       (GdkGeometry *geometry,
                                             guint flags,
                                             gint width,
                                             gint height,
                                             gint *new_width,
                                             gint *new_height);

void        gdk_window_begin_paint_rect     (GdkWindow *window,
                                             GdkRectangle *rectangle);
void        gdk_window_begin_paint_region   (GdkWindow *window,
                                             GdkRegion *region);
void        gdk_window_end_paint            (GdkWindow *window);
*/

static gboolean
invalidate_child_func_wrap(window, func)
    GdkWindow* window;
    VALUE func;
{
    VALUE result = rb_funcall(func, id_call, 1, GOBJ2RVAL(window));
    g_print("invalidate_child_func_wrap\n");
    if (result == Qnil || result == Qfalse) {
        return FALSE;
    } else {
        return TRUE;
    }
}


static VALUE
gdkwin_invalidate(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE rect_or_region, invalidate_children_or_func;
    
    rb_scan_args(argc, argv, "11", &rect_or_region, &invalidate_children_or_func);

    
    if (rb_obj_is_kind_of(rect_or_region, GTYPE2CLASS(GDK_TYPE_RECTANGLE))){
        gdk_window_invalidate_rect(_SELF(self),
                                   RVAL2BOXED(rect_or_region, GDK_TYPE_RECTANGLE),
                                   RTEST(invalidate_children_or_func));
    } else if (rb_obj_is_kind_of(rect_or_region, GTYPE2CLASS(GDK_TYPE_REGION))) {
        VALUE type = TYPE(invalidate_children_or_func);

        if (type == T_TRUE || type == T_FALSE) {
            gdk_window_invalidate_region(_SELF(self),
                                         RVAL2BOXED(rect_or_region, GDK_TYPE_REGION),
                                         RTEST(invalidate_children_or_func));
        } else {
            VALUE child_func;
            if (type == T_NIL) {
                child_func = rb_f_lambda();
            } else if (rb_obj_is_kind_of(invalidate_children_or_func, rb_cProc)) {
                child_func = invalidate_children_or_func;
            } else {
                rb_raise(rb_eArgError, 
                         "invalid argument %s (expect Proc)",
                         rb_class2name(CLASS_OF(invalidate_children_or_func)));
            }
            gdk_window_invalidate_maybe_recurse(_SELF(self),
                                                RVAL2BOXED(rect_or_region, GDK_TYPE_REGION),
                                                invalidate_child_func_wrap,
                                                (gpointer) child_func);
        }
    } else {
        rb_raise(rb_eArgError, 
                 "invalid argument %s (expect Gdk::Rectangle or Gdk::Region)",
                 rb_class2name(CLASS_OF(rect_or_region)));
    }
    return self;
}

/*
GdkRegion*  gdk_window_get_update_area      (GdkWindow *window);
void        gdk_window_freeze_updates       (GdkWindow *window);
void        gdk_window_thaw_updates         (GdkWindow *window);
void        gdk_window_process_all_updates  (void);
void        gdk_window_process_updates      (GdkWindow *window,
                                             gboolean update_children);
void        gdk_window_set_debug_updates    (gboolean setting);
void        gdk_window_get_internal_paint_info
                                            (GdkWindow *window,
                                             GdkDrawable **real_drawable,
                                             gint *x_offset,
                                             gint *y_offset);

void        gdk_window_set_user_data        (GdkWindow *window,
                                             gpointer user_data);
*/

static VALUE
gdkwin_set_override_redirect(self, override_redirect)
    VALUE self, override_redirect;
{
    gdk_window_set_override_redirect(_SELF(self), 
                                     RTEST(override_redirect));
    return self;
}

/*
void        gdk_window_add_filter           (GdkWindow *window,
                                             GdkFilterFunc function,
                                             gpointer data);
void        gdk_window_remove_filter        (GdkWindow *window,
                                             GdkFilterFunc function,
                                             gpointer data);
GdkFilterReturn (*GdkFilterFunc)            (GdkXEvent *xevent,
                                             GdkEvent *event,
                                             gpointer data);
*/

static VALUE
gdkwin_shape_combine_mask(self, shape_mask, offset_x, offset_y)
    VALUE self, shape_mask, offset_x, offset_y;
{
    gdk_window_shape_combine_mask(_SELF(self), 
                                  GDK_BITMAP(RVAL2GOBJ(shape_mask)), 
                                  NUM2INT(offset_x), NUM2INT(offset_y));
    return self;
}


/*
void        gdk_window_shape_combine_region (GdkWindow *window,
                                             GdkRegion *shape_region,
                                             gint offset_x,
                                             gint offset_y);
*/


static VALUE
gdkwin_set_child_shapes(self)
    VALUE self;
{
    gdk_window_set_child_shapes(_SELF(self));
    return self;
}

static VALUE
gdkwin_merge_child_shapes(self)
    VALUE self;
{
    gdk_window_merge_child_shapes(_SELF(self));
    return self;
}   

static VALUE
gdkwin_set_static_gravities(self, use_static)
    VALUE self, use_static;
{
    return (gdk_window_set_static_gravities(_SELF(self),
                                            RTEST(use_static))) ? Qtrue : Qfalse;
}

/* Obsoleted.
void        gdk_window_set_hints            (GdkWindow *window,
*/

static VALUE
gdkwin_set_title(self, title)
    VALUE self, title;
{
    gdk_window_set_title(_SELF(self),RVAL2CSTR(title));
    return self;
}

static VALUE
gdkwin_set_background(self, color)
    VALUE self, color;
{
    gdk_window_set_background(_SELF(self), 
                              (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
gdkwin_set_back_pixmap(self, pixmap, parent_relative)
    VALUE self, pixmap, parent_relative;
{
    gdk_window_set_back_pixmap(_SELF(self), 
                               GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                               RTEST(parent_relative));
    return self;
}

static VALUE
gdkwin_set_cursor(self, cursor)
    VALUE self, cursor;
{
    gdk_window_set_cursor(_SELF(self), 
                          (GdkCursor*)RVAL2BOXED(cursor, GDK_TYPE_CURSOR));
    return self;
}

/* Obsolete
#define     gdk_window_set_colormap
*/

/*
void        gdk_window_get_user_data        (GdkWindow *window,
                                             gpointer *data);
*/

static VALUE
gdkwin_get_geometry(self)
    VALUE self;
{
    gint x, y, w, h, d;
    gdk_window_get_geometry(_SELF(self), &x, &y, &w, &h, &d);
    return rb_ary_new3(5, INT2NUM(x), INT2NUM(y),
                       INT2NUM(w), INT2NUM(h), INT2NUM(d));
}

/*
void        gdk_window_set_geometry_hints   (GdkWindow *window,
                                             GdkGeometry *geometry,
                                             GdkWindowHints geom_mask);
void        gdk_window_set_icon_list        (GdkWindow *window,
                                             GList *pixbufs);
void        gdk_window_set_modal_hint       (GdkWindow *window,
                                             gboolean modal);
void        gdk_window_set_type_hint        (GdkWindow *window,
                                             GdkWindowTypeHint hint);
*/

static VALUE
gdkwin_get_position(self)
	VALUE self;
{
	gint x, y;
	gdk_window_get_position(_SELF(self), &x, &y);
	return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

static VALUE
gdkwin_get_root_origin(self)
    VALUE self;
{
    int x, y;
    gdk_window_get_root_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

/*
void        gdk_window_get_frame_extents    (GdkWindow *window,
                                             GdkRectangle *rect);
*/
/* Obsolete
#define     gdk_window_get_size
#define     gdk_window_get_visual
#define     gdk_window_get_colormap
#define     gdk_window_get_type
*/

static VALUE
gdkwin_get_origin(self)
	VALUE self;
{
	gint x, y;
	gdk_window_get_origin(_SELF(self), &x, &y);
	return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

/* Obsolete
gboolean    gdk_window_get_deskrelative_origin
                                            (GdkWindow *window,
                                             gint *x,
                                             gint *y);
*/

static VALUE
gdkwin_get_pointer(self)
    VALUE self;
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(_SELF(self), &x, &y, &state);
    return rb_ary_new3(3, INT2FIX(x), INT2FIX(y), INT2FIX((int)state));

}

static VALUE
gdkwin_get_parent(self)
	VALUE self;
{
	return GOBJ2RVAL(gdk_window_get_parent(_SELF(self)));
}

static VALUE
gdkwin_get_toplevel(self)
	VALUE self;
{
	return GOBJ2RVAL(gdk_window_get_toplevel(_SELF(self)));
}

static VALUE
gdkwin_get_children(self)
	VALUE self;
{
	GList* list = gdk_window_get_children(_SELF(self));
	VALUE ary = rb_ary_new();
	while (list) {
		rb_ary_push(ary, GOBJ2RVAL(list->data));
		list = list->next;
	}
	return ary;
}

static VALUE
gdkwin_get_events(self)
    VALUE self;
{
    return INT2NUM(gdk_window_get_events(_SELF(self)));
}

static VALUE
gdkwin_set_events(self, mask)
    VALUE self, mask;
{
    gdk_window_set_events(_SELF(self), NUM2INT(mask));
    return self;
}

static VALUE
gdkwin_set_icon(self, icon, pixmap, mask)
    VALUE self, icon, pixmap, mask;
{
    gdk_window_set_icon(_SELF(self), NIL_P(icon) ? NULL :_SELF(icon),
                        NIL_P(pixmap) ? NULL : GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                        NIL_P(mask) ? NULL : GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
gdkwin_set_icon_name(self, name)
    VALUE self, name;
{
    gdk_window_set_icon_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
gdkwin_set_transient_for(self, parent)
    VALUE self, parent;
{
    gdk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
gdkwin_set_role(self, role)
    VALUE self, role;
{
    gdk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
gdkwin_set_group(self, leader)
    VALUE self, leader;
{
    gdk_window_set_group(_SELF(self), _SELF(leader));
    return self;
}

static VALUE
gdkwin_set_decorations(self, decor)
    VALUE self, decor;
{
    gdk_window_set_decorations(_SELF(self), NUM2INT(decor));
    return self;
}

/*
gboolean    gdk_window_get_decorations      (GdkWindow *window,
                                             GdkWMDecoration *decorations);
*/

static VALUE
gdkwin_set_functions(self, func)
    VALUE self, func;
{
    gdk_window_set_functions(_SELF(self), NUM2INT(func));
    return self;
}

static VALUE
gdkwin_get_toplevels(self)
    VALUE self;
{
    return GLIST2ARY(gdk_window_get_toplevels());
}

static VALUE
gdkwin_s_get_default_root_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_get_default_root_window());
}

/*
GdkPointerHooks* gdk_set_pointer_hooks      (const GdkPointerHooks *new_hooks);
*/

/* Properties */
static VALUE
gdkwin_prop_change(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int        fmt, len;
    void*      dat;
    GdkAtom    ntype;
    VALUE property, type, size=Qnil, mode, src;
    
    if(5 == argc)
        rb_scan_args(argc, argv, "50", &property, &type, &size, &mode, &src);
    else
        rb_scan_args(argc, argv, "40", &property, &type, &mode, &src);
    
    rbgtk_atom2selectiondata(type, size, src, &ntype, &dat, &fmt, &len);
    
    gdk_property_change(_SELF(self), RVAL2ATOM(property), 
                        ntype, fmt, NUM2INT(mode), dat, len);

    rbgtk_atom2selectiondata_free(ntype, dat);

    return self;
}

static VALUE
gdkwin_prop_get(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    /* for argument processing */
    GdkAtom     rtype;
    gint        rfmt, rlen;
    void*	rdat;
    VALUE property, type, offset=INT2FIX(0), length=INT2FIX(9999), delete;
    
    /* for inner processing */
    int		i;
    VALUE	ret = 0;
    
    if(5 == argc)
      rb_scan_args(argc, argv, "50", &property, &type, &offset, &length, &delete);
    else
      rb_scan_args(argc, argv, "30", &property, &type, &delete);
    
    
    if(gdk_property_get(_SELF(self), RVAL2ATOM(property), RVAL2ATOM(type),
                        NUM2INT(offset), NUM2INT(length),
                        RTEST(delete), &rtype, &rfmt, &rlen, (guchar**)&rdat) == FALSE){
        return Qnil;
    }
    
    switch(rfmt){
      case 8:
      default:
        ret = rb_str_new(rdat, rlen);
        break;
        
      case 16:
        ret = rb_ary_new();
        
        for( i = 0; i < rlen; i++){
            rb_ary_push(ret, rb_Integer(((unsigned short*)rdat)[i]));
        }
        break;
        
      case 32:
/*
  ret = rb_ary_new();

  if(rtype != GDK_SELECTION_TYPE_ATOM){
  for(i = 0; i < rlen; i++){
  rb_ary_push(ret, INT2FIX(((unsigned long *)rdat)[i]));
  }
  } else {
  for(i = 0; i < rlen; i++){
  rb_ary_push(ret, RVAL2BOXED((GdkAtom)(unsigned long *)rdat[i], GDK_TYPE_ATOM));
  }
  }
*/
        rb_warning("not implemented yet.");
        break;
    }
    
    return rb_ary_new3(3, BOXED2RVAL(&rtype, GDK_TYPE_ATOM), 
                       ret, INT2NUM(rlen));
}

static VALUE
gdkwin_prop_delete(self, property)
    VALUE self, property;
{
    gdk_property_delete(_SELF(self), RVAL2ATOM(property));
    return self;
}

void
Init_gtk_gdk_window()
{
    VALUE gdkWindow = G_DEF_CLASS(GDK_TYPE_WINDOW, "Window", mGdk);

/*
    rb_define_method(gdkWindow, "initialize", gdkwin_initialize, 3);
*/
   rb_define_method(gdkWindow, "window_type", gdkwin_get_window_type, 0);
   rb_define_singleton_method(gdkWindow, "at_pointer", gdkwin_s_at_pointer, 0);
   rb_define_singleton_method(gdkWindow, "foreign", gdkwin_foreign, -1);
   rb_define_method(gdkWindow, "show", gdkwin_show, -1);
   rb_define_method(gdkWindow, "hide", gdkwin_hide, 0);
   rb_define_method(gdkWindow, "visible?", gdkwin_is_visible, 0);
   rb_define_method(gdkWindow, "viewable?", gdkwin_is_viewable, 0);
   rb_define_method(gdkWindow, "state", gdkwin_get_state, 0);
   rb_define_method(gdkWindow, "iconify", gdkwin_window_iconify, 0);
   rb_define_method(gdkWindow, "deiconify", gdkwin_window_deiconify, 0);
   rb_define_method(gdkWindow, "stick", gdkwin_window_stick, 0);
   rb_define_method(gdkWindow, "unstick", gdkwin_window_unstick, 0);
   rb_define_method(gdkWindow, "maximize", gdkwin_window_maximize, 0);
   rb_define_method(gdkWindow, "unmaximize", gdkwin_window_unmaximize, 0);
   rb_define_method(gdkWindow, "move", gdkwin_move, 2);
   rb_define_method(gdkWindow, "resize", gdkwin_resize, 2);
   rb_define_method(gdkWindow, "move_resize", gdkwin_move_resize, 4);
   rb_define_method(gdkWindow, "scroll", gdkwin_scroll, 2);
   rb_define_method(gdkWindow, "reparent", gdkwin_reparent, 3);
   rb_define_method(gdkWindow, "clear", gdkwin_clear, 0);
   rb_define_method(gdkWindow, "clear_area", gdkwin_clear_area, -1);
   rb_define_method(gdkWindow, "raise", gdkwin_raise, 0);
   rb_define_method(gdkWindow, "lower", gdkwin_lower, 0);
   rb_define_method(gdkWindow, "focus", gdkwin_focus, 1);
   rb_define_method(gdkWindow, "register_dnd", gdkwin_register_dnd, 0);
   rb_define_method(gdkWindow, "invalidate", gdkwin_invalidate, -1);
   rb_define_method(gdkWindow, "set_override_redirect", gdkwin_set_override_redirect, 1);
   rb_define_method(gdkWindow, "shape_combine_mask", gdkwin_shape_combine_mask, 3);
   rb_define_method(gdkWindow, "set_child_shapes", gdkwin_set_child_shapes, 0);
   rb_define_method(gdkWindow, "merge_child_shapes", gdkwin_merge_child_shapes, 0);
   rb_define_method(gdkWindow, "set_static_gravities", gdkwin_set_static_gravities, 1);
   rb_define_method(gdkWindow, "set_title", gdkwin_set_title, 1);
   rb_define_method(gdkWindow, "set_background", gdkwin_set_background, 1);
   rb_define_method(gdkWindow, "set_back_pixmap", gdkwin_set_back_pixmap, 2);
   rb_define_method(gdkWindow, "set_cursor", gdkwin_set_cursor, 1);
   rb_define_method(gdkWindow, "geometry", gdkwin_get_geometry, 0);
   rb_define_method(gdkWindow, "position", gdkwin_get_position, 0);
   rb_define_method(gdkWindow, "root_origin", gdkwin_get_root_origin, 0);
   rb_define_method(gdkWindow, "origin", gdkwin_get_origin, 0);
   rb_define_method(gdkWindow, "pointer", gdkwin_get_pointer, 0);
   rb_define_method(gdkWindow, "parent", gdkwin_get_parent, 0);
   rb_define_method(gdkWindow, "toplevel", gdkwin_get_toplevel, 0);
   rb_define_singleton_method(gdkWindow, "default_root_window", gdkwin_s_get_default_root_window, 0);
   rb_define_method(gdkWindow, "children", gdkwin_get_children, 0);
   rb_define_method(gdkWindow, "events", gdkwin_get_events, 0);
   rb_define_method(gdkWindow, "set_events", gdkwin_set_events, 1);
   rb_define_method(gdkWindow, "set_icon", gdkwin_set_icon, 3);
   rb_define_method(gdkWindow, "set_icon_name", gdkwin_set_icon_name, 1);
   rb_define_method(gdkWindow, "set_transient_for", gdkwin_set_transient_for, 1);
   rb_define_method(gdkWindow, "set_role", gdkwin_set_role, 1);
   rb_define_method(gdkWindow, "set_group", gdkwin_set_group, 1);
   rb_define_method(gdkWindow, "set_decorations", gdkwin_set_decorations, 1);
   rb_define_method(gdkWindow, "set_functions", gdkwin_set_functions, 1);
   rb_define_method(gdkWindow, "toplevels", gdkwin_get_toplevels, 0);
   rb_define_method(gdkWindow, "property_change", gdkwin_prop_change, -1);
   rb_define_method(gdkWindow, "property_get", gdkwin_prop_get, -1);
   rb_define_method(gdkWindow, "property_delete", gdkwin_prop_delete, 1);

   G_DEF_SETTERS(gdkWindow);

   /* GdkWindowType */
   rb_define_const(gdkWindow, "ROOT", INT2FIX(GDK_WINDOW_ROOT));
   rb_define_const(gdkWindow, "TOPLEVEL", INT2FIX(GDK_WINDOW_TOPLEVEL));
   rb_define_const(gdkWindow, "CHILD", INT2FIX(GDK_WINDOW_CHILD));
   rb_define_const(gdkWindow, "DIALOG", INT2FIX(GDK_WINDOW_DIALOG));
   rb_define_const(gdkWindow, "TEMP", INT2FIX(GDK_WINDOW_TEMP));
   rb_define_const(gdkWindow, "FOREIGN", INT2FIX(GDK_WINDOW_FOREIGN));
   
   /* GdkWindowClass */
   rb_define_const(gdkWindow, "INPUT_OUTPUT", INT2FIX(GDK_INPUT_OUTPUT));
   rb_define_const(gdkWindow, "INPUT_ONLY", INT2FIX(GDK_INPUT_ONLY));

   /* GdkWindowHints */
   rb_define_const(gdkWindow, "HINT_POS", INT2FIX(GDK_HINT_POS));
   rb_define_const(gdkWindow, "HINT_MIN_SIZE", INT2FIX(GDK_HINT_MIN_SIZE));
   rb_define_const(gdkWindow, "HINT_MAX_SIZE", INT2FIX(GDK_HINT_MAX_SIZE));
   rb_define_const(gdkWindow, "HINT_BASE_SIZE", INT2FIX(GDK_HINT_BASE_SIZE));
   rb_define_const(gdkWindow, "HINT_ASPECT", INT2FIX(GDK_HINT_ASPECT));
   rb_define_const(gdkWindow, "HINT_RESIZE_INC", INT2FIX(GDK_HINT_RESIZE_INC));
   rb_define_const(gdkWindow, "HINT_WIN_GRAVITY", INT2FIX(GDK_HINT_WIN_GRAVITY));
   rb_define_const(gdkWindow, "HINT_USER_POS", INT2FIX(GDK_HINT_USER_POS));
   rb_define_const(gdkWindow, "HINT_USER_SIZE", INT2FIX(GDK_HINT_USER_SIZE));

   /* GdkGravity */
   rb_define_const(gdkWindow, "GRAVITY_NORTH_WEST", INT2FIX(GDK_GRAVITY_NORTH_WEST));
   rb_define_const(gdkWindow, "GRAVITY_NORTH", INT2FIX(GDK_GRAVITY_NORTH));
   rb_define_const(gdkWindow, "GRAVITY_NORTH_EAST", INT2FIX(GDK_GRAVITY_NORTH_EAST));
   rb_define_const(gdkWindow, "GRAVITY_WEST", INT2FIX(GDK_GRAVITY_WEST));
   rb_define_const(gdkWindow, "GRAVITY_CENTER", INT2FIX(GDK_GRAVITY_CENTER));
   rb_define_const(gdkWindow, "GRAVITY_EAST", INT2FIX(GDK_GRAVITY_EAST));
   rb_define_const(gdkWindow, "GRAVITY_SOUTH_WEST", INT2FIX(GDK_GRAVITY_SOUTH_WEST));
   rb_define_const(gdkWindow, "GRAVITY_SOUTH", INT2FIX(GDK_GRAVITY_SOUTH));
   rb_define_const(gdkWindow, "GRAVITY_SOUTH_EAST", INT2FIX(GDK_GRAVITY_SOUTH_EAST));
   rb_define_const(gdkWindow, "GRAVITY_STATIC", INT2FIX(GDK_GRAVITY_STATIC));

   /* GdkWindowEdge */
   rb_define_const(gdkWindow, "EDGE_NORTH_WEST", INT2FIX(GDK_WINDOW_EDGE_WEST));
   rb_define_const(gdkWindow, "EDGE_NORTH", INT2FIX(GDK_WINDOW_EDGE_NORTH));
   rb_define_const(gdkWindow, "EDGE_NORTH_EAST", INT2FIX(GDK_WINDOW_EDGE_NORTH_EAST));
   rb_define_const(gdkWindow, "EDGE_WEST", INT2FIX(GDK_WINDOW_EDGE_WEST));
   rb_define_const(gdkWindow, "EDGE_EAST", INT2FIX(GDK_WINDOW_EDGE_EAST));
   rb_define_const(gdkWindow, "EDGE_SOUTH_WEST", INT2FIX(GDK_WINDOW_EDGE_SOUTH_WEST));
   rb_define_const(gdkWindow, "EDGE_SOUTH", INT2FIX(GDK_WINDOW_EDGE_SOUTH));
   rb_define_const(gdkWindow, "EDGE_SOUTH_EAST", INT2FIX(GDK_WINDOW_EDGE_SOUTH_EAST));

   /* GdkWindowTypeHint */
   rb_define_const(gdkWindow, "TYPE_HINT_NORMAL", INT2FIX(GDK_WINDOW_TYPE_HINT_NORMAL));
   rb_define_const(gdkWindow, "TYPE_HINT_DIALOG", INT2FIX(GDK_WINDOW_TYPE_HINT_DIALOG));
   rb_define_const(gdkWindow, "TYPE_HINT_MENU", INT2FIX(GDK_WINDOW_TYPE_HINT_MENU));
   rb_define_const(gdkWindow, "TYPE_HINT_TOOLBAR", INT2FIX(GDK_WINDOW_TYPE_HINT_TOOLBAR));

   /* GdkWindowAttibutesType */
   rb_define_const(gdkWindow, "WA_TITLE", INT2FIX(GDK_WA_TITLE));
   rb_define_const(gdkWindow, "WA_X", INT2FIX(GDK_WA_X));
   rb_define_const(gdkWindow, "WA_Y", INT2FIX(GDK_WA_Y));
   rb_define_const(gdkWindow, "WA_CURSOR", INT2FIX(GDK_WA_CURSOR));
   rb_define_const(gdkWindow, "WA_COLORMAP", INT2FIX(GDK_WA_COLORMAP));
   rb_define_const(gdkWindow, "WA_VISUAL", INT2FIX(GDK_WA_VISUAL));
   rb_define_const(gdkWindow, "WA_WMCLASS", INT2FIX(GDK_WA_WMCLASS));
   rb_define_const(gdkWindow, "WA_NOREDIR", INT2FIX(GDK_WA_NOREDIR));

   /* GdkFilterReturn */
   rb_define_const(gdkWindow, "FILTER_CONTINUE", INT2FIX(GDK_FILTER_CONTINUE));
   rb_define_const(gdkWindow, "FILTER_TRANSLATE", INT2FIX(GDK_FILTER_TRANSLATE));
   rb_define_const(gdkWindow, "FILTER_REMOVE", INT2FIX(GDK_FILTER_REMOVE));

   /* GdkModifierType */
   rb_define_const(gdkWindow, "SHIFT_MASK", INT2FIX(GDK_SHIFT_MASK));
   rb_define_const(gdkWindow, "LOCK_MASK", INT2FIX(GDK_LOCK_MASK));
   rb_define_const(gdkWindow, "CONTROL_MASK", INT2FIX(GDK_CONTROL_MASK));
   rb_define_const(gdkWindow, "MOD1_MASK", INT2FIX(GDK_MOD1_MASK));
   rb_define_const(gdkWindow, "MOD2_MASK", INT2FIX(GDK_MOD2_MASK));
   rb_define_const(gdkWindow, "MOD3_MASK", INT2FIX(GDK_MOD3_MASK));
   rb_define_const(gdkWindow, "MOD4_MASK", INT2FIX(GDK_MOD4_MASK));
   rb_define_const(gdkWindow, "MOD5_MASK", INT2FIX(GDK_MOD5_MASK));
   rb_define_const(gdkWindow, "BUTTON1_MASK", INT2FIX(GDK_BUTTON1_MASK));
   rb_define_const(gdkWindow, "BUTTON2_MASK", INT2FIX(GDK_BUTTON2_MASK));
   rb_define_const(gdkWindow, "BUTTON3_MASK", INT2FIX(GDK_BUTTON3_MASK));
   rb_define_const(gdkWindow, "BUTTON4_MASK", INT2FIX(GDK_BUTTON4_MASK));
   rb_define_const(gdkWindow, "BUTTON5_MASK", INT2FIX(GDK_BUTTON5_MASK));
   
   /* GdkWMDecoration */
   rb_define_const(gdkWindow, "DECOR_ALL", INT2FIX(GDK_DECOR_ALL));
   rb_define_const(gdkWindow, "DECOR_BORDER", INT2FIX(GDK_DECOR_BORDER));
   rb_define_const(gdkWindow, "DECOR_RESIZEH", INT2FIX(GDK_DECOR_RESIZEH));
   rb_define_const(gdkWindow, "DECOR_TITLE", INT2FIX(GDK_DECOR_TITLE));
   rb_define_const(gdkWindow, "DECOR_MENU", INT2FIX(GDK_DECOR_MENU));
   rb_define_const(gdkWindow, "DECOR_MINIMIZE", INT2FIX(GDK_DECOR_MINIMIZE));
   rb_define_const(gdkWindow, "DECOR_MAXIMIZE", INT2FIX(GDK_DECOR_MAXIMIZE));
   
   /* GdkWMFunction */
   rb_define_const(gdkWindow, "FUNC_ALL", INT2FIX(GDK_FUNC_ALL));
   rb_define_const(gdkWindow, "FUNC_RESIZE", INT2FIX(GDK_FUNC_RESIZE));
   rb_define_const(gdkWindow, "FUNC_MOVE", INT2FIX(GDK_FUNC_MOVE));
   rb_define_const(gdkWindow, "FUNC_MINIMIZE", INT2FIX(GDK_FUNC_MINIMIZE));
   rb_define_const(gdkWindow, "FUNC_MAXIMIZE", INT2FIX(GDK_FUNC_MAXIMIZE));
   rb_define_const(gdkWindow, "FUNC_CLOSE", INT2FIX(GDK_FUNC_CLOSE));

   /* GdkPropMode from GdkProperties */
   rb_define_const(gdkWindow, "PROP_MODE_REPLACE", INT2FIX(GDK_PROP_MODE_REPLACE));
   rb_define_const(gdkWindow, "PROP_MODE_PREPEND", INT2FIX(GDK_PROP_MODE_PREPEND));
   rb_define_const(gdkWindow, "PROP_MODE_APPEND", INT2FIX(GDK_PROP_MODE_APPEND));

   rb_define_const(gdkWindow, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));
   
}

