/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkwindow.c -

  $Author: mutoh $
  $Date: 2003/10/13 13:28:28 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) GDK_WINDOW(RVAL2GOBJ(s))

static VALUE
gdkwin_initialize(self, parent, attributes, attributes_mask)
    VALUE self, parent, attributes, attributes_mask;
{
    GdkWindow* win;
    win = gdk_window_new(_SELF(parent), 
                         (GdkWindowAttr*)RVAL2BOXED(attributes, 
                                                    GDK_TYPE_WINDOW_ATTR), 
                         RVAL2GFLAGS(attributes_mask, 
                                    GDK_TYPE_WINDOW_ATTRIBUTES_TYPE));
    G_INITIALIZE(self, win);

    return Qnil;
}

static VALUE
gdkwin_destroy(self)
    VALUE self;
{
    gdk_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_get_window_type(self)
    VALUE self;
{
    return GENUM2RVAL(gdk_window_get_window_type(_SELF(self)), GDK_TYPE_WINDOW_TYPE);
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
gdkwin_foreign_new(argc, argv, self)
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

static VALUE
gdkwin_show(self)
    VALUE self;
{
    gdk_window_show(_SELF(self));
    return self;
}

static VALUE
gdkwin_show_unraised(self)
    VALUE self;
{
    gdk_window_show_unraised(_SELF(self));
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
    return GENUM2RVAL(gdk_window_get_state(_SELF(self)), GDK_TYPE_WINDOW_STATE);
}

static VALUE
gdkwin_withdraw(self)
    VALUE self;
{
    gdk_window_withdraw(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_iconify(self)
    VALUE self;
{
    gdk_window_iconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_deiconify(self)
    VALUE self;
{
    gdk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_stick(self)
    VALUE self;
{
    gdk_window_stick(_SELF(self));
    return self;
}

static VALUE
gdkwin_unstick(self)
    VALUE self;
{
    gdk_window_unstick(_SELF(self));
    return self;
}

static VALUE
gdkwin_maximize(self)
    VALUE self;
{
    gdk_window_maximize(_SELF(self));
    return self;
}

static VALUE
gdkwin_unmaximize(self)
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

static VALUE
gdkwin_begin_resize_drag(self, edge, button, root_x, root_y, timestamp)
    VALUE self, edge, button, root_x, root_y, timestamp;
{
    gdk_window_begin_resize_drag(_SELF(self),
                                 RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE),
                                 NUM2INT(button),
                                 NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_begin_move_drag(self, button, root_x, root_y, timestamp)
    VALUE self, button, root_x, root_y, timestamp;
{
    gdk_window_begin_move_drag(_SELF(self), NUM2INT(button),
                               NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_s_constrain_size(self, geometry, flags, w, h)
    VALUE self, geometry, flags, w, h;
{
    gint new_width, new_height;
    gdk_window_constrain_size((GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                              RVAL2GFLAGS(flags, GDK_TYPE_WINDOW_HINTS),
                              NUM2INT(w), NUM2INT(h), &new_width, &new_height);
    return rb_ary_new3(2, INT2NUM(new_width), INT2NUM(new_height));
}

static VALUE
gdkwin_begin_paint(self, area)
    VALUE self, area;
{
    if (rb_obj_is_kind_of(area, GTYPE2CLASS(GDK_TYPE_RECTANGLE))){
        gdk_window_begin_paint_rect(_SELF(self), 
                                    (GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE));
    } else {
        gdk_window_begin_paint_region(_SELF(self),
                                      (GdkRegion*)RVAL2BOXED(area, GDK_TYPE_REGION));
    }
    return self;
}

static VALUE
gdkwin_end_paint(self)
    VALUE self;
{
    gdk_window_end_paint(_SELF(self));
    return self;
}

static VALUE
gdkwin_invalidate(self, area, invalidate_children)
    VALUE self, area, invalidate_children;
{
    if (rb_obj_is_kind_of(area, GTYPE2CLASS(GDK_TYPE_RECTANGLE))){
        gdk_window_invalidate_rect(_SELF(self),
                                   RVAL2BOXED(area, GDK_TYPE_RECTANGLE),
                                   RTEST(invalidate_children));
    } else {
        gdk_window_invalidate_region(_SELF(self),
                                     RVAL2BOXED(area, GDK_TYPE_REGION),
                                     RTEST(invalidate_children));
    }
    return self;
}

static gboolean
invalidate_child_func_wrap(window, func)
    GdkWindow* window;
    VALUE func;
{
    VALUE result = rb_funcall(func, id_call, 1, GOBJ2RVAL(window));
    return RTEST(result);
}

static VALUE
gdkwin_invalidate_maybe_recurse(self, region)
    VALUE self, region;
{
    VALUE func = (VALUE)NULL;
    if (rb_block_given_p()){
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }
    gdk_window_invalidate_maybe_recurse(_SELF(self),
                                        RVAL2BOXED(region, GDK_TYPE_REGION),
                                        invalidate_child_func_wrap,
                                        (gpointer)func);
    return self;
}

static VALUE
gdkwin_get_update_area(self)
    VALUE self;
{
    GdkRegion* ret = gdk_window_get_update_area(_SELF(self));
    return ret ? BOXED2RVAL(ret, GDK_TYPE_REGION) : Qnil; 
}

static VALUE
gdkwin_freeze_updates(self)
    VALUE self;
{
    gdk_window_freeze_updates(_SELF(self));
    return self;
}

static VALUE
gdkwin_thaw_updates(self)
    VALUE self;
{
    gdk_window_thaw_updates(_SELF(self));
    return self;
}

static VALUE
gdkwin_s_process_all_updates(self)
    VALUE self;
{
    gdk_window_process_all_updates();
    return self;
}

static VALUE
gdkwin_process_updates(self, update_children)
    VALUE self, update_children;
{
    gdk_window_process_updates(_SELF(self), RTEST(update_children));
    return self;
}

static VALUE
gdkwin_s_set_debug_updates(self, setting)
    VALUE self, setting;
{
    gdk_window_set_debug_updates(RTEST(setting));
    return self;
}

static VALUE
gdkwin_get_internal_paint_info(self)
    VALUE self;
{
    GdkDrawable* real_drawable;
    gint x_offset, y_offset;

    gdk_window_get_internal_paint_info(_SELF(self), &real_drawable,
                                       &x_offset, &y_offset);
    return rb_ary_new3(3, GOBJ2RVAL(real_drawable),
                       INT2NUM(x_offset), INT2NUM(y_offset));
}

static VALUE
gdkwin_set_user_data(self, user_data)
    VALUE self, user_data;
{
    gdk_window_set_user_data(_SELF(self), (gpointer)RVAL2GOBJ(user_data));
    return self;
}

static VALUE
gdkwin_set_override_redirect(self, override_redirect)
    VALUE self, override_redirect;
{
    gdk_window_set_override_redirect(_SELF(self), 
                                     RTEST(override_redirect));
    return self;
}

/* GdkXEvent is not implemented.
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

static VALUE
gdkwin_shape_combine_region(self, shape_region, offset_x, offset_y)
    VALUE self, shape_region, offset_x, offset_y;
{
    gdk_window_shape_combine_region(_SELF(self),
                                    NIL_P(shape_region) ? NULL : (GdkRegion*)RVAL2BOXED(shape_region, GDK_TYPE_REGION),
                                    INT2NUM(offset_x), INT2NUM(offset_y));
    return self;
}

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

static VALUE
gdkwin_get_user_data(self)
    VALUE self;
{
    GObject data;
    gdk_window_get_user_data(_SELF(self), (gpointer)&data);
    return GOBJ2RVAL(&data);
}

static VALUE
gdkwin_get_geometry(self)
    VALUE self;
{
    gint x, y, w, h, d;
    gdk_window_get_geometry(_SELF(self), &x, &y, &w, &h, &d);
    return rb_ary_new3(5, INT2NUM(x), INT2NUM(y),
                       INT2NUM(w), INT2NUM(h), INT2NUM(d));
}

static VALUE
gdkwin_set_geometry_hints(self, geometry, geom_mask)
    VALUE self, geometry, geom_mask;
{
    gdk_window_set_geometry_hints(_SELF(self), 
                                  (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  RVAL2GFLAGS(geom_mask, GDK_TYPE_WINDOW_HINTS));
    return self;
}

static VALUE
gdkwin_set_icon_list(self, pixbufs)
    VALUE self, pixbufs;
{
    int i;
    GList *glist = NULL;

    Check_Type(pixbufs, T_ARRAY);
    for (i = 0; i < RARRAY(pixbufs)->len; i++) {
        glist = g_list_append(glist, GDK_PIXBUF(RVAL2GOBJ(RARRAY(pixbufs)->ptr[i])));
    }

    gdk_window_set_icon_list(_SELF(self), glist);
    return self;
}

static VALUE
gdkwin_set_modal_hint(self, modal)
    VALUE self, modal;
{
    gdk_window_set_modal_hint(_SELF(self), RTEST(modal));
    return self;
}

static VALUE
gdkwin_set_type_hint(self, hint)
    VALUE self, hint;
{
    gdk_window_set_type_hint(_SELF(self), RVAL2GENUM(hint, GDK_TYPE_WINDOW_TYPE_HINT));
    return self;
}

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

static VALUE
gdkwin_get_frame_extents(self)
    VALUE self;
{
    GdkRectangle rect;
    gdk_window_get_frame_extents(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

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
    return rb_ary_new3(3, INT2FIX(x), INT2FIX(y), GENUM2RVAL(state, GDK_TYPE_MODIFIER_TYPE));

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
    gdk_window_set_events(_SELF(self), RVAL2GENUM(mask, GDK_TYPE_EVENT_MASK));
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
    gdk_window_set_decorations(_SELF(self), RVAL2GENUM(decor, GDK_TYPE_WM_DECORATION));
    return self;
}

static VALUE
gdkwin_get_decorations(self)
    VALUE self;
{
    GdkWMDecoration decorations;
    gboolean ret = gdk_window_get_decorations(_SELF(self), &decorations);
    return ret ? GENUM2RVAL(decorations, GDK_TYPE_WM_DECORATION) : Qnil;
}

static VALUE
gdkwin_set_functions(self, func)
    VALUE self, func;
{
    gdk_window_set_functions(_SELF(self), RVAL2GENUM(func, GDK_TYPE_WM_FUNCTION));
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

/*
static VALUE
gdkwin_s_set_pointer_hooks(get_pointer_hook, window_at_pointer_hook)
    VALUE get_pointer_hook, window_at_pointer_hook;
{
    GdkPointerHooks hooks;
    hooks.get_pointer = get_pointer;
    hooks.window_at_pointer = window_at_pointer;
    GdkPointerHooks gdk_set_pointer_hooks((const GdkPointerHooks*)&hooks);
    return 
}
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
                        ntype, fmt, RVAL2GENUM(mode, GDK_TYPE_PROP_MODE), dat, len);

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

    rb_define_method(gdkWindow, "initialize", gdkwin_initialize, 3);
    rb_define_method(gdkWindow, "destroy", gdkwin_destroy, 0);
    rb_define_method(gdkWindow, "window_type", gdkwin_get_window_type, 0);
    rb_define_singleton_method(gdkWindow, "at_pointer", gdkwin_s_at_pointer, 0);
    rb_define_singleton_method(gdkWindow, "foreign_new", gdkwin_foreign_new, -1);
    rb_define_singleton_method(gdkWindow, "constrain_size", gdkwin_s_constrain_size, 4);
    rb_define_singleton_method(gdkWindow, "process_all_updates", gdkwin_s_process_all_updates, 0);
    rb_define_singleton_method(gdkWindow, "set_debug_updates", gdkwin_s_set_debug_updates, 1);
    rb_define_method(gdkWindow, "show", gdkwin_show, 0);
    rb_define_method(gdkWindow, "show_unraised", gdkwin_show_unraised, 0);
    rb_define_method(gdkWindow, "hide", gdkwin_hide, 0);
    rb_define_method(gdkWindow, "visible?", gdkwin_is_visible, 0);
    rb_define_method(gdkWindow, "viewable?", gdkwin_is_viewable, 0);
    rb_define_method(gdkWindow, "withdraw", gdkwin_withdraw, 0);
    rb_define_method(gdkWindow, "state", gdkwin_get_state, 0);
    rb_define_method(gdkWindow, "iconify", gdkwin_iconify, 0);
    rb_define_method(gdkWindow, "deiconify", gdkwin_deiconify, 0);
    rb_define_method(gdkWindow, "stick", gdkwin_stick, 0);
    rb_define_method(gdkWindow, "unstick", gdkwin_unstick, 0);
    rb_define_method(gdkWindow, "maximize", gdkwin_maximize, 0);
    rb_define_method(gdkWindow, "unmaximize", gdkwin_unmaximize, 0);
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
    rb_define_method(gdkWindow, "begin_resize_drag", gdkwin_begin_resize_drag, 5);
    rb_define_method(gdkWindow, "begin_move_drag", gdkwin_begin_move_drag, 4);
    rb_define_method(gdkWindow, "begin_paint", gdkwin_begin_paint, 1);
    rb_define_method(gdkWindow, "end_paint", gdkwin_end_paint, 0);
    rb_define_method(gdkWindow, "invalidate", gdkwin_invalidate, 2);
    rb_define_method(gdkWindow, "invalidate_maybe_recurse", gdkwin_invalidate_maybe_recurse, 1);
    rb_define_method(gdkWindow, "update_area", gdkwin_get_update_area, 0);
    rb_define_method(gdkWindow, "freeze_updates", gdkwin_freeze_updates, 0);
    rb_define_method(gdkWindow, "thaw_updates", gdkwin_thaw_updates, 0);
    rb_define_method(gdkWindow, "process_updates", gdkwin_process_updates, 1);
    rb_define_method(gdkWindow, "internal_paint_info", gdkwin_get_internal_paint_info, 0);
    rb_define_method(gdkWindow, "set_user_data", gdkwin_set_user_data, 1);
    rb_define_method(gdkWindow, "set_override_redirect", gdkwin_set_override_redirect, 1);
    rb_define_method(gdkWindow, "shape_combine_mask", gdkwin_shape_combine_mask, 3);
    rb_define_method(gdkWindow, "shape_combine_region", gdkwin_shape_combine_region, 3);
    rb_define_method(gdkWindow, "set_child_shapes", gdkwin_set_child_shapes, 0);
    rb_define_method(gdkWindow, "merge_child_shapes", gdkwin_merge_child_shapes, 0);
    rb_define_method(gdkWindow, "set_static_gravities", gdkwin_set_static_gravities, 1);
    rb_define_method(gdkWindow, "set_title", gdkwin_set_title, 1);
    rb_define_method(gdkWindow, "set_background", gdkwin_set_background, 1);
    rb_define_method(gdkWindow, "set_back_pixmap", gdkwin_set_back_pixmap, 2);
    rb_define_method(gdkWindow, "set_cursor", gdkwin_set_cursor, 1);
    rb_define_method(gdkWindow, "user_data", gdkwin_get_user_data, 0);
    rb_define_method(gdkWindow, "geometry", gdkwin_get_geometry, 0);
    rb_define_method(gdkWindow, "set_geometry_hints", gdkwin_set_geometry_hints, 2);
    rb_define_method(gdkWindow, "set_icon_list", gdkwin_set_icon_list, 1);
    rb_define_method(gdkWindow, "set_modal_hint", gdkwin_set_modal_hint, 1);
    rb_define_method(gdkWindow, "set_type_hint", gdkwin_set_type_hint, 1);
    rb_define_method(gdkWindow, "position", gdkwin_get_position, 0);
    rb_define_method(gdkWindow, "root_origin", gdkwin_get_root_origin, 0);
    rb_define_method(gdkWindow, "frame_extents", gdkwin_get_frame_extents, 0);
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
    rb_define_method(gdkWindow, "decorations", gdkwin_get_decorations, 0);
    rb_define_method(gdkWindow, "set_functions", gdkwin_set_functions, 1);
    rb_define_method(gdkWindow, "toplevels", gdkwin_get_toplevels, 0);
    rb_define_method(gdkWindow, "property_change", gdkwin_prop_change, -1);
    rb_define_method(gdkWindow, "property_get", gdkwin_prop_get, -1);
    rb_define_method(gdkWindow, "property_delete", gdkwin_prop_delete, 1);

    G_DEF_SETTERS(gdkWindow);

    /* GdkWindowType */
    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE, "Type", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_TYPE, "GDK_WINDOW_");
   
    /* GdkWindowClass */
    G_DEF_CLASS(GDK_TYPE_WINDOW_CLASS, "Class", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_CLASS, "GDK_");

    /* GdkWindowHints */
    G_DEF_CLASS(GDK_TYPE_WINDOW_HINTS, "Hints", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_HINTS, "GDK_");

    /* GdkGravity */
    G_DEF_CLASS(GDK_TYPE_GRAVITY, "Gravity", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_GRAVITY, "GDK_");

    /* GdkWindowEdge */
    G_DEF_CLASS(GDK_TYPE_WINDOW_EDGE, "Edge", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_EDGE, "GDK_WINDOW_");

    /* GdkWindowTypeHint */
    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE_HINT, "TypeHint", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_TYPE_HINT, "GDK_WINDOW_");

    /* GdkWindowAttibutesType */
    G_DEF_CLASS(GDK_TYPE_WINDOW_ATTRIBUTES_TYPE, "AttributesType", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WINDOW_ATTRIBUTES_TYPE, "GDK_");

    /* GdkFilterReturn */
    G_DEF_CLASS(GDK_TYPE_FILTER_RETURN, "FilterReturn", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_FILTER_RETURN, "GDK_");

    /* GdkModifierType */
    G_DEF_CLASS(GDK_TYPE_MODIFIER_TYPE, "ModifierType", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_MODIFIER_TYPE, "GDK_");
   
    /* GdkWMDecoration */
    G_DEF_CLASS(GDK_TYPE_WM_DECORATION, "WMDecoration", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WM_DECORATION, "GDK_");
   
    /* GdkWMFunction */
    G_DEF_CLASS(GDK_TYPE_WM_FUNCTION, "WMFunction", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_WM_FUNCTION, "GDK_");

    /* GdkPropMode from GdkProperties */
    G_DEF_CLASS(GDK_TYPE_PROP_MODE, "PropMode", gdkWindow);
    G_DEF_CONSTANTS(gdkWindow, GDK_TYPE_PROP_MODE, "GDK_");

    rb_define_const(gdkWindow, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));   
}

