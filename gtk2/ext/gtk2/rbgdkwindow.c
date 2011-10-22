/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#define _SELF(s) GDK_WINDOW(RVAL2GOBJ(s))

static VALUE
gdkwin_initialize(VALUE self, VALUE parent, VALUE attributes, VALUE attributes_mask)
{
    GdkWindow* win;
    win = gdk_window_new(NIL_P(parent) ? NULL : _SELF(parent), 
                         (GdkWindowAttr*)RVAL2BOXED(attributes, 
                                                    GDK_TYPE_WINDOW_ATTR), 
                         RVAL2GFLAGS(attributes_mask, 
                                    GDK_TYPE_WINDOW_ATTRIBUTES_TYPE));
    G_INITIALIZE(self, win);

    return Qnil;
}

static VALUE
gdkwin_destroy(VALUE self)
{
    gdk_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_get_window_type(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_window_type(_SELF(self)), GDK_TYPE_WINDOW_TYPE);
}

static VALUE
gdkwin_s_at_pointer(G_GNUC_UNUSED VALUE self)
{
    gint x, y;
    GdkWindow* win = gdk_window_at_pointer(&x, &y);
    return rb_ary_new3(3, GOBJ2RVAL(win), INT2FIX(x), INT2FIX(y));
}

static VALUE
gdkwin_show(VALUE self)
{
    gdk_window_show(_SELF(self));
    return self;
}

static VALUE
gdkwin_show_unraised(VALUE self)
{
    gdk_window_show_unraised(_SELF(self));
    return self;
}

static VALUE
gdkwin_hide(VALUE self)
{
    gdk_window_hide(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_is_visible(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_visible(_SELF(self)));
}

static VALUE
gdkwin_is_viewable(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_viewable(_SELF(self)));
}

static VALUE
gdkwin_get_state(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_state(_SELF(self)), GDK_TYPE_WINDOW_STATE);
}

static VALUE
gdkwin_withdraw(VALUE self)
{
    gdk_window_withdraw(_SELF(self));
    return Qnil;
}

static VALUE
gdkwin_iconify(VALUE self)
{
    gdk_window_iconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_deiconify(VALUE self)
{
    gdk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
gdkwin_stick(VALUE self)
{
    gdk_window_stick(_SELF(self));
    return self;
}

static VALUE
gdkwin_unstick(VALUE self)
{
    gdk_window_unstick(_SELF(self));
    return self;
}

static VALUE
gdkwin_maximize(VALUE self)
{
    gdk_window_maximize(_SELF(self));
    return self;
}

static VALUE
gdkwin_unmaximize(VALUE self)
{
    gdk_window_unmaximize(_SELF(self));
    return self;
}
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkwin_fullscreen(VALUE self)
{
    gdk_window_fullscreen(_SELF(self));
    return self;
}

static VALUE
gdkwin_unfullscreen(VALUE self)
{
    gdk_window_unfullscreen(_SELF(self));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkwin_set_keep_above(VALUE self, VALUE setting)
{
    gdk_window_set_keep_above(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
gdkwin_set_keep_below(VALUE self, VALUE setting)
{
    gdk_window_set_keep_below(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
#endif

static VALUE
gdkwin_move(VALUE self, VALUE x, VALUE y)
{
    gdk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_resize(VALUE self, VALUE w, VALUE h)
{
    gdk_window_resize(_SELF(self), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_move_resize(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
    gdk_window_move_resize(_SELF(self), NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_scroll(VALUE self, VALUE dx, VALUE dy)
{
    gdk_window_scroll(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkwin_move_region(VALUE self, VALUE region, VALUE dx, VALUE dy)
{
    gdk_window_move_region(_SELF(self),
                           (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION),
                           NUM2INT(dx),
                           NUM2INT(dy));
    return self;
}
#endif

static VALUE
gdkwin_reparent(VALUE self, VALUE new_parent, VALUE x, VALUE y)
{
    gdk_window_reparent(_SELF(self), _SELF(new_parent), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_clear(VALUE self)
{
    gdk_window_clear(_SELF(self));
    return self;
}

static VALUE
gdkwin_clear_area(int argc, VALUE *argv, VALUE self)
{
    VALUE gen_expose, x, y, w, h;
    rb_scan_args(argc, argv, "41", &x, &y, &w, &h, &gen_expose);

    if (! NIL_P(gen_expose) && RVAL2CBOOL(gen_expose)){
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
gdkwin_raise(VALUE self)
{
    gdk_window_raise(_SELF(self));
    return self;
}

static VALUE
gdkwin_lower(VALUE self)
{
    gdk_window_lower(_SELF(self));
    return self;
}

static VALUE
gdkwin_focus(VALUE self, VALUE timestamp)
{
    gdk_window_focus(_SELF(self), NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_register_dnd(VALUE self)
{
    gdk_window_register_dnd(_SELF(self));
    return self;
}

static VALUE
gdkwin_begin_resize_drag(VALUE self, VALUE edge, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_resize_drag(_SELF(self),
                                 RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE),
                                 NUM2INT(button),
                                 NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_begin_move_drag(VALUE self, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_move_drag(_SELF(self), NUM2INT(button),
                               NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
gdkwin_s_constrain_size(G_GNUC_UNUSED VALUE self, VALUE geometry, VALUE flags, VALUE w, VALUE h)
{
    gint new_width, new_height;
    gdk_window_constrain_size((GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                              RVAL2GFLAGS(flags, GDK_TYPE_WINDOW_HINTS),
                              NUM2INT(w), NUM2INT(h), &new_width, &new_height);
    return rb_assoc_new(INT2NUM(new_width), INT2NUM(new_height));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkwin_beep(VALUE self) 
{
    gdk_window_beep(_SELF(self));
    return self;
}
#endif

static VALUE
gdkwin_begin_paint(VALUE self, VALUE area)
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
gdkwin_end_paint(VALUE self)
{
    gdk_window_end_paint(_SELF(self));
    return self;
}

static VALUE
gdkwin_invalidate(VALUE self, VALUE area, VALUE invalidate_children)
{
    if (rb_obj_is_kind_of(area, GTYPE2CLASS(GDK_TYPE_REGION))){
        gdk_window_invalidate_region(_SELF(self),
                                     RVAL2BOXED(area, GDK_TYPE_REGION),
                                     RVAL2CBOOL(invalidate_children));
    } else {
        gdk_window_invalidate_rect(_SELF(self),
                                   RVAL2BOXED(area, GDK_TYPE_RECTANGLE),
                                   RVAL2CBOOL(invalidate_children));
    }
    return self;
}

static gboolean
invalidate_child_func_wrap(GdkWindow *window, VALUE func)
{
    VALUE result = rb_funcall(func, id_call, 1, GOBJ2RVAL(window));
    return RVAL2CBOOL(result);
}

typedef gboolean (*ChildFunc) (GdkWindow*, gpointer);

static VALUE
gdkwin_invalidate_maybe_recurse(VALUE self, VALUE region)
{
    VALUE func = (VALUE)NULL;
    if (rb_block_given_p()){
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }
    gdk_window_invalidate_maybe_recurse(_SELF(self),
                                        RVAL2BOXED(region, GDK_TYPE_REGION),
                                        (ChildFunc)invalidate_child_func_wrap,
                                        (gpointer)func);
    return self;
}

static VALUE
gdkwin_get_update_area(VALUE self)
{
    GdkRegion* ret = gdk_window_get_update_area(_SELF(self));
    return BOXED2RVAL(ret, GDK_TYPE_REGION); 
}

static VALUE
gdkwin_freeze_updates(VALUE self)
{
    gdk_window_freeze_updates(_SELF(self));
    return self;
}

static VALUE
gdkwin_thaw_updates(VALUE self)
{
    gdk_window_thaw_updates(_SELF(self));
    return self;
}

static VALUE
gdkwin_s_process_all_updates(VALUE self)
{
    gdk_window_process_all_updates();
    return self;
}

static VALUE
gdkwin_process_updates(VALUE self, VALUE update_children)
{
    gdk_window_process_updates(_SELF(self), RVAL2CBOOL(update_children));
    return self;
}

static VALUE
gdkwin_s_set_debug_updates(VALUE self, VALUE setting)
{
    gdk_window_set_debug_updates(RVAL2CBOOL(setting));
    return self;
}

static VALUE
gdkwin_get_internal_paint_info(VALUE self)
{
    GdkDrawable* real_drawable;
    gint x_offset, y_offset;

    gdk_window_get_internal_paint_info(_SELF(self), &real_drawable,
                                       &x_offset, &y_offset);
    return rb_ary_new3(3, GOBJ2RVAL(real_drawable),
                       INT2NUM(x_offset), INT2NUM(y_offset));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkwin_configure_finished(VALUE self)
{
    gdk_window_configure_finished(_SELF(self));
    return self;
}

static VALUE
gdkwin_enable_synchronized_configure(VALUE self)
{
    gdk_window_enable_synchronized_configure(_SELF(self));

    if (rb_block_given_p()) {
        rb_ensure(rb_yield, self, gdkwin_configure_finished, self);
    }
    return self;
}
#endif

static VALUE
gdkwin_set_user_data(VALUE self, VALUE user_data)
{
    gdk_window_set_user_data(_SELF(self), (gpointer)RVAL2GOBJ(user_data));
    return self;
}

static VALUE
gdkwin_set_override_redirect(VALUE self, VALUE override_redirect)
{
    gdk_window_set_override_redirect(_SELF(self), 
                                     RVAL2CBOOL(override_redirect));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkwin_set_accept_focus(VALUE self, VALUE accept_focus)
{
    gdk_window_set_accept_focus(_SELF(self), RVAL2CBOOL(accept_focus));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkwin_set_focus_on_map(VALUE self, VALUE focus_on_map)
{
    gdk_window_set_focus_on_map(_SELF(self), RVAL2CBOOL(focus_on_map));
    return self;
}
#endif

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
gdkwin_shape_combine_mask(VALUE self, VALUE shape_mask, VALUE offset_x, VALUE offset_y)
{
    gdk_window_shape_combine_mask(_SELF(self), 
                                  GDK_BITMAP(RVAL2GOBJ(shape_mask)), 
                                  NUM2INT(offset_x), NUM2INT(offset_y));
    return self;
}

static VALUE
gdkwin_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_shape_combine_region(_SELF(self),
                                    NIL_P(shape_region) ? NULL : (GdkRegion*)RVAL2BOXED(shape_region, GDK_TYPE_REGION),
                                    INT2NUM(offset_x), INT2NUM(offset_y));
    return self;
}

static VALUE
gdkwin_set_child_shapes(VALUE self)
{
    gdk_window_set_child_shapes(_SELF(self));
    return self;
}

static VALUE
gdkwin_merge_child_shapes(VALUE self)
{
    gdk_window_merge_child_shapes(_SELF(self));
    return self;
}   

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkwin_input_shape_combine_mask(VALUE self, VALUE mask, VALUE x, VALUE y)
{
    gdk_window_input_shape_combine_mask(_SELF(self),
                                        GDK_BITMAP(RVAL2GOBJ(mask)),
                                        NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_input_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_input_shape_combine_region(_SELF(self),
                                          (GdkRegion*)RVAL2BOXED(shape_region, GDK_TYPE_REGION),
                                          NUM2INT(offset_x),
                                          NUM2INT(offset_y));
    return self;
}

static VALUE
gdkwin_set_child_input_shapes(VALUE self)
{
    gdk_window_set_child_input_shapes(_SELF(self));
    return self;
}

static VALUE
gdkwin_merge_child_input_shapes(VALUE self)
{
    gdk_window_merge_child_input_shapes(_SELF(self));
    return self;
}

#endif

static VALUE
gdkwin_set_static_gravities(VALUE self, VALUE use_static)
{
    gboolean ret = gdk_window_set_static_gravities(_SELF(self),
                                                   RVAL2CBOOL(use_static));
    if (! ret)
        rb_raise(rb_eRuntimeError, "couldn't turn on static gravity");

    return self;
}

/* Obsoleted.
   void        gdk_window_set_hints            (GdkWindow *window,
*/

static VALUE
gdkwin_set_title(VALUE self, VALUE title)
{
    gdk_window_set_title(_SELF(self),RVAL2CSTR(title));
    return self;
}

static VALUE
gdkwin_set_background(VALUE self, VALUE color)
{
    gdk_window_set_background(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
gdkwin_set_back_pixmap(VALUE self, VALUE pixmap, VALUE parent_relative)
{
    gdk_window_set_back_pixmap(_SELF(self), 
                               GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                               RVAL2CBOOL(parent_relative));
    return self;
}

/* Properties:
 *
 * void                gdk_window_set_cursor               (GdkWindow *window,
 *                                                          GdkCursor *cursor);
 * GdkCursor *         gdk_window_get_cursor               (GdkWindow *window);
 */

/* Obsolete
   #define     gdk_window_set_colormap
*/

static VALUE
gdkwin_get_user_data(VALUE self)
{
    GObject *data = NULL;
    gdk_window_get_user_data(_SELF(self), (gpointer)&data);
    return GOBJ2RVAL(data);
}

static VALUE
gdkwin_get_geometry(VALUE self)
{
    gint x, y, w, h, d;
    gdk_window_get_geometry(_SELF(self), &x, &y, &w, &h, &d);
    return rb_ary_new3(5, INT2NUM(x), INT2NUM(y),
                       INT2NUM(w), INT2NUM(h), INT2NUM(d));
}

static VALUE
gdkwin_set_geometry_hints(VALUE self, VALUE geometry, VALUE geom_mask)
{
    gdk_window_set_geometry_hints(_SELF(self), 
                                  NIL_P(geometry) ? (GdkGeometry*)NULL : (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  RVAL2GFLAGS(geom_mask, GDK_TYPE_WINDOW_HINTS));
    return self;
}

static VALUE
gdkwin_set_icon_list(VALUE self, VALUE rbpixbufs)
{
    GdkWindow *window = _SELF(self);
    GList *pixbufs = RVAL2GDKPIXBUFGLIST(rbpixbufs);

    gdk_window_set_icon_list(window, pixbufs);

    g_list_free(pixbufs);

    return self;
}

static VALUE
gdkwin_set_modal_hint(VALUE self, VALUE modal)
{
    gdk_window_set_modal_hint(_SELF(self), RVAL2CBOOL(modal));
    return self;
}

static VALUE
gdkwin_set_type_hint(VALUE self, VALUE hint)
{
    gdk_window_set_type_hint(_SELF(self), RVAL2GENUM(hint, GDK_TYPE_WINDOW_TYPE_HINT));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkwin_get_type_hint(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_type_hint(_SELF(self)), GDK_TYPE_WINDOW_TYPE_HINT);
}
#endif

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkwin_set_skip_taskbar_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_taskbar_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}

static VALUE
gdkwin_set_skip_pager_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_pager_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkwin_set_urgency_hint(VALUE self, VALUE hint)
{
    gdk_window_set_urgency_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}
#endif

static VALUE
gdkwin_get_position(VALUE self)
{
    gint x, y;
    gdk_window_get_position(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
gdkwin_get_root_origin(VALUE self)
{
    int x, y;
    gdk_window_get_root_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
gdkwin_get_frame_extents(VALUE self)
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
gdkwin_get_origin(VALUE self)
{
    gint x, y;
    gdk_window_get_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

/* Obsolete
   gboolean    gdk_window_get_deskrelative_origin
   (GdkWindow *window,
   gint *x,
   gint *y);
*/

static VALUE
gdkwin_get_pointer(VALUE self)
{
    gint x, y;
    GdkModifierType state;
    GdkWindow* ret = gdk_window_get_pointer(_SELF(self), &x, &y, &state);
    return rb_ary_new3(4, GOBJ2RVAL(ret), INT2NUM(x), INT2NUM(y), GFLAGS2RVAL(state, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
gdkwin_get_parent(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_parent(_SELF(self)));
}

static VALUE
gdkwin_get_toplevel(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_toplevel(_SELF(self)));
}

static VALUE
gdkwin_get_children(VALUE self)
{
    /* Don't use gdk_window_get_children() here */
    GList* list = gdk_window_peek_children(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    return ary;
}

static VALUE
gdkwin_get_events(VALUE self)
{
    return GFLAGS2RVAL(gdk_window_get_events(_SELF(self)), GDK_TYPE_EVENT_MASK);
}

static VALUE
gdkwin_set_events(VALUE self, VALUE mask)
{
    gdk_window_set_events(_SELF(self), RVAL2GFLAGS(mask, GDK_TYPE_EVENT_MASK));
    return self;
}

static VALUE
gdkwin_set_icon(VALUE self, VALUE icon, VALUE pixmap, VALUE mask)
{
    gdk_window_set_icon(_SELF(self), NIL_P(icon) ? NULL :_SELF(icon),
                        NIL_P(pixmap) ? NULL : GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                        NIL_P(mask) ? NULL : GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
gdkwin_set_icon_name(VALUE self, VALUE name)
{
    gdk_window_set_icon_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
gdkwin_set_transient_for(VALUE self, VALUE parent)
{
    gdk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
gdkwin_set_role(VALUE self, VALUE role)
{
    gdk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
gdkwin_set_group(VALUE self, VALUE leader)
{
    gdk_window_set_group(_SELF(self), _SELF(leader));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkwin_get_group(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_group(_SELF(self)));
}
#endif

static VALUE
gdkwin_set_decorations(VALUE self, VALUE decor)
{
    gdk_window_set_decorations(_SELF(self), RVAL2GFLAGS(decor, GDK_TYPE_WM_DECORATION));
    return self;
}

static VALUE
gdkwin_get_decorations(VALUE self)
{
    GdkWMDecoration decorations;
    gboolean ret = gdk_window_get_decorations(_SELF(self), &decorations);
    return ret ? GFLAGS2RVAL(decorations, GDK_TYPE_WM_DECORATION) : Qnil;
}

static VALUE
gdkwin_set_functions(VALUE self, VALUE func)
{
    gdk_window_set_functions(_SELF(self), RVAL2GFLAGS(func, GDK_TYPE_WM_FUNCTION));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkwin_set_composited(VALUE self, VALUE composited)
{
    gdk_window_set_composited(_SELF(self), RVAL2CBOOL(composited));
    return self;
}

static VALUE
gdkwin_set_opacity(VALUE self, VALUE opacity)
{
    gdk_window_set_opacity(_SELF(self), NUM2DBL(opacity));
    return self;
}

static VALUE
gdkwin_set_startup_id(VALUE self, VALUE startup_id)
{
    gdk_window_set_startup_id(_SELF(self), RVAL2CSTR_ACCEPT_NIL(startup_id));
    return self;
}
#endif

static VALUE
gdkwin_s_get_toplevels(G_GNUC_UNUSED VALUE self)
{
    return GLIST2ARYF(gdk_window_get_toplevels());
}

static VALUE
gdkwin_s_get_default_root_window(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_get_default_root_window());
}

/* Would you need this?
GdkPointerHooks* gdk_set_pointer_hooks      (const GdkPointerHooks *new_hooks);
*/

/* From X Window System Interaction */
static VALUE
gdkwin_foreign_new(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[2];
    GdkWindow * win = NULL;
    
    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
    	win = gdk_window_foreign_new(RVAL2GDKNATIVEWINDOW(arg[0]));
	break;
      case 2:
#if GTK_CHECK_VERSION(2,2,0)
    	win = gdk_window_foreign_new_for_display(RVAL2GOBJ(arg[0]),
                                                 RVAL2GDKNATIVEWINDOW(arg[1])); 
#else
    	win = gdk_window_foreign_new(NUM2UINT(arg[1])); 
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

static VALUE
gdkwin_lookup(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[2];
    GdkWindow * win = NULL;
    
    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
    	win = gdk_window_lookup(RVAL2GDKNATIVEWINDOW(arg[0]));
	break;
      case 2:
#if GTK_CHECK_VERSION(2,2,0)
    	win = gdk_window_lookup_for_display(RVAL2GOBJ(arg[0]), RVAL2GDKNATIVEWINDOW(arg[1])); 
#else
    	win = gdk_window_lookup(NUM2UINT(arg[1])); 
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

#ifdef GDK_WINDOWING_X11
static VALUE
gdkwin_get_server_time(VALUE self)
{
    return UINT2NUM(gdk_x11_get_server_time(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkwin_set_user_time(VALUE self, VALUE time)
{
    gdk_x11_window_set_user_time(_SELF(self), NUM2UINT(time));
    return Qnil;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkwin_move_to_current_desktop(VALUE self)
{
    gdk_x11_window_move_to_current_desktop(_SELF(self));
    return self;
}
#endif

#endif

void
Init_gtk_gdk_window(void)
{
    GObjectClass *g_class;
    VALUE gdkWindow = G_DEF_CLASS(GDK_TYPE_WINDOW, "Window", mGdk);

    g_class = g_type_class_peek(GDK_TYPE_WINDOW);

    rb_define_method(gdkWindow, "initialize", gdkwin_initialize, 3);
    rb_define_method(gdkWindow, "destroy", gdkwin_destroy, 0);
    rb_define_method(gdkWindow, "window_type", gdkwin_get_window_type, 0);
    rb_define_singleton_method(gdkWindow, "at_pointer", gdkwin_s_at_pointer, 0);
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
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkWindow, "fullscreen", gdkwin_fullscreen, 0);
    rb_define_method(gdkWindow, "unfullscreen", gdkwin_unfullscreen, 0);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkWindow, "set_keep_above", gdkwin_set_keep_above, 1);
    rb_define_method(gdkWindow, "set_keep_below", gdkwin_set_keep_below, 1);
#endif
    rb_define_method(gdkWindow, "move", gdkwin_move, 2);
    rb_define_method(gdkWindow, "resize", gdkwin_resize, 2);
    rb_define_method(gdkWindow, "move_resize", gdkwin_move_resize, 4);
    rb_define_method(gdkWindow, "scroll", gdkwin_scroll, 2);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkWindow, "move_region", gdkwin_move_region, 3);
#endif
    rb_define_method(gdkWindow, "reparent", gdkwin_reparent, 3);
    rb_define_method(gdkWindow, "clear", gdkwin_clear, 0);
    rb_define_method(gdkWindow, "clear_area", gdkwin_clear_area, -1);
    rb_define_method(gdkWindow, "raise", gdkwin_raise, 0);
    rb_define_method(gdkWindow, "lower", gdkwin_lower, 0);
    rb_define_method(gdkWindow, "focus", gdkwin_focus, 1);
    rb_define_method(gdkWindow, "register_dnd", gdkwin_register_dnd, 0);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(gdkWindow, "beep", gdkwin_beep, 0);
#endif
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
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkWindow, "configure_finished", gdkwin_configure_finished, 0);
    rb_define_method(gdkWindow, "enable_synchronized_configure", gdkwin_enable_synchronized_configure, 0);
#endif
    rb_define_method(gdkWindow, "set_user_data", gdkwin_set_user_data, 1);
    rb_define_method(gdkWindow, "set_override_redirect", gdkwin_set_override_redirect, 1);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkWindow, "set_accept_focus", gdkwin_set_accept_focus, 1);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkWindow, "set_focus_on_map", gdkwin_set_focus_on_map, 1);
#endif
    rb_define_method(gdkWindow, "shape_combine_mask", gdkwin_shape_combine_mask, 3);
    rb_define_method(gdkWindow, "shape_combine_region", gdkwin_shape_combine_region, 3);
    rb_define_method(gdkWindow, "set_child_shapes", gdkwin_set_child_shapes, 0);
    rb_define_method(gdkWindow, "merge_child_shapes", gdkwin_merge_child_shapes, 0);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gdkWindow, "input_shape_combine_mask", gdkwin_input_shape_combine_mask, 3);
    rb_define_method(gdkWindow, "input_shape_combine_region", gdkwin_input_shape_combine_region, 3);
    rb_define_method(gdkWindow, "set_child_input_shapes", gdkwin_set_child_input_shapes, 0);
    rb_define_method(gdkWindow, "merge_child_input_shapes", gdkwin_merge_child_input_shapes, 0);
#endif
    rb_define_method(gdkWindow, "set_static_gravities", gdkwin_set_static_gravities, 1);
    rb_define_method(gdkWindow, "set_title", gdkwin_set_title, 1);
    rb_define_method(gdkWindow, "set_background", gdkwin_set_background, 1);
    rb_define_method(gdkWindow, "set_back_pixmap", gdkwin_set_back_pixmap, 2);
    rb_define_method(gdkWindow, "user_data", gdkwin_get_user_data, 0);
    rb_define_method(gdkWindow, "geometry", gdkwin_get_geometry, 0);
    rb_define_method(gdkWindow, "set_geometry_hints", gdkwin_set_geometry_hints, 2);
    rb_define_method(gdkWindow, "set_icon_list", gdkwin_set_icon_list, 1);
    rb_define_method(gdkWindow, "set_modal_hint", gdkwin_set_modal_hint, 1);
    rb_define_method(gdkWindow, "set_type_hint", gdkwin_set_type_hint, 1);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gdkWindow, "type_hint", gdkwin_get_type_hint, 0);
#endif

#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkWindow, "set_skip_taskbar_hint", gdkwin_set_skip_taskbar_hint, 1);
    rb_define_method(gdkWindow, "set_skip_pager_hint", gdkwin_set_skip_pager_hint, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkWindow, "set_urgency_hint", gdkwin_set_urgency_hint, 1);
#endif
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
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkWindow, "group", gdkwin_get_group, 0);
#endif
    rb_define_method(gdkWindow, "set_decorations", gdkwin_set_decorations, 1);
    rb_define_method(gdkWindow, "decorations", gdkwin_get_decorations, 0);
    rb_define_method(gdkWindow, "set_functions", gdkwin_set_functions, 1);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(gdkWindow, "set_composited", gdkwin_set_composited, 1);
    rb_define_method(gdkWindow, "set_opacity", gdkwin_set_opacity, 1);
    rb_define_method(gdkWindow, "set_startup_id", gdkwin_set_startup_id, 1);
#endif
    rb_define_singleton_method(gdkWindow, "toplevels", gdkwin_s_get_toplevels, 0);

    rb_define_singleton_method(gdkWindow, "foreign_new", gdkwin_foreign_new, -1);
    rb_define_singleton_method(gdkWindow, "lookup", gdkwin_lookup, -1);

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

    rb_define_const(gdkWindow, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));   

#ifdef GDK_WINDOWING_X11
    rb_define_method(gdkWindow, "server_time", gdkwin_get_server_time, 0);

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkWindow, "set_user_time", gdkwin_set_user_time, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkWindow, "move_to_current_desktop", gdkwin_move_to_current_desktop, 0);
#endif

    G_DEF_CLASS3("GdkWindowImplX11", "WindowImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkWindowImplWin32", "WindowImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkWindowFB", "WindowFB", mGdk);
#endif
}

