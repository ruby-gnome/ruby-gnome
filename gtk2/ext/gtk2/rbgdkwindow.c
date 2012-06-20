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

#define RG_TARGET_NAMESPACE cWindow
#define _SELF(s) GDK_WINDOW(RVAL2GOBJ(s))

static VALUE
rg_initialize(VALUE self, VALUE parent, VALUE attributes, VALUE attributes_mask)
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
rg_destroy(VALUE self)
{
    gdk_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
rg_window_type(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_window_type(_SELF(self)), GDK_TYPE_WINDOW_TYPE);
}

static VALUE
rg_s_at_pointer(G_GNUC_UNUSED VALUE self)
{
    gint x, y;
    GdkWindow* win = gdk_window_at_pointer(&x, &y);
    return rb_ary_new3(3, GOBJ2RVAL(win), INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_show(VALUE self)
{
    gdk_window_show(_SELF(self));
    return self;
}

static VALUE
rg_show_unraised(VALUE self)
{
    gdk_window_show_unraised(_SELF(self));
    return self;
}

static VALUE
rg_hide(VALUE self)
{
    gdk_window_hide(_SELF(self));
    return Qnil;
}

static VALUE
rg_visible_p(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_visible(_SELF(self)));
}

static VALUE
rg_viewable_p(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_viewable(_SELF(self)));
}

static VALUE
rg_state(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_state(_SELF(self)), GDK_TYPE_WINDOW_STATE);
}

static VALUE
rg_withdraw(VALUE self)
{
    gdk_window_withdraw(_SELF(self));
    return Qnil;
}

static VALUE
rg_iconify(VALUE self)
{
    gdk_window_iconify(_SELF(self));
    return self;
}

static VALUE
rg_deiconify(VALUE self)
{
    gdk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
rg_stick(VALUE self)
{
    gdk_window_stick(_SELF(self));
    return self;
}

static VALUE
rg_unstick(VALUE self)
{
    gdk_window_unstick(_SELF(self));
    return self;
}

static VALUE
rg_maximize(VALUE self)
{
    gdk_window_maximize(_SELF(self));
    return self;
}

static VALUE
rg_unmaximize(VALUE self)
{
    gdk_window_unmaximize(_SELF(self));
    return self;
}
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_fullscreen(VALUE self)
{
    gdk_window_fullscreen(_SELF(self));
    return self;
}

static VALUE
rg_unfullscreen(VALUE self)
{
    gdk_window_unfullscreen(_SELF(self));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_set_keep_above(VALUE self, VALUE setting)
{
    gdk_window_set_keep_above(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_set_keep_below(VALUE self, VALUE setting)
{
    gdk_window_set_keep_below(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
#endif

static VALUE
rg_move(VALUE self, VALUE x, VALUE y)
{
    gdk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_resize(VALUE self, VALUE w, VALUE h)
{
    gdk_window_resize(_SELF(self), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_move_resize(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
    gdk_window_move_resize(_SELF(self), NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_scroll(VALUE self, VALUE dx, VALUE dy)
{
    gdk_window_scroll(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_move_region(VALUE self, VALUE region, VALUE dx, VALUE dy)
{
    gdk_window_move_region(_SELF(self),
                           (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION),
                           NUM2INT(dx),
                           NUM2INT(dy));
    return self;
}
#endif

static VALUE
rg_reparent(VALUE self, VALUE new_parent, VALUE x, VALUE y)
{
    gdk_window_reparent(_SELF(self), _SELF(new_parent), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_clear(VALUE self)
{
    gdk_window_clear(_SELF(self));
    return self;
}

static VALUE
rg_clear_area(int argc, VALUE *argv, VALUE self)
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

static VALUE
rg_raise(VALUE self)
{
    gdk_window_raise(_SELF(self));
    return self;
}

static VALUE
rg_lower(VALUE self)
{
    gdk_window_lower(_SELF(self));
    return self;
}

static VALUE
rg_focus(VALUE self, VALUE timestamp)
{
    gdk_window_focus(_SELF(self), NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_register_dnd(VALUE self)
{
    gdk_window_register_dnd(_SELF(self));
    return self;
}

static VALUE
rg_begin_resize_drag(VALUE self, VALUE edge, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_resize_drag(_SELF(self),
                                 RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE),
                                 NUM2INT(button),
                                 NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_begin_move_drag(VALUE self, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_move_drag(_SELF(self), NUM2INT(button),
                               NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_s_constrain_size(G_GNUC_UNUSED VALUE self, VALUE geometry, VALUE flags, VALUE w, VALUE h)
{
    gint new_width, new_height;
    gdk_window_constrain_size((GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                              RVAL2GFLAGS(flags, GDK_TYPE_WINDOW_HINTS),
                              NUM2INT(w), NUM2INT(h), &new_width, &new_height);
    return rb_assoc_new(INT2NUM(new_width), INT2NUM(new_height));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_beep(VALUE self) 
{
    gdk_window_beep(_SELF(self));
    return self;
}
#endif

static VALUE
rg_begin_paint(VALUE self, VALUE area)
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
rg_end_paint(VALUE self)
{
    gdk_window_end_paint(_SELF(self));
    return self;
}

static VALUE
rg_invalidate(VALUE self, VALUE area, VALUE invalidate_children)
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
rg_invalidate_maybe_recurse(VALUE self, VALUE region)
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
rg_update_area(VALUE self)
{
    GdkRegion* ret = gdk_window_get_update_area(_SELF(self));
    return BOXED2RVAL(ret, GDK_TYPE_REGION); 
}

static VALUE
rg_freeze_updates(VALUE self)
{
    gdk_window_freeze_updates(_SELF(self));
    return self;
}

static VALUE
rg_thaw_updates(VALUE self)
{
    gdk_window_thaw_updates(_SELF(self));
    return self;
}

static VALUE
rg_s_process_all_updates(VALUE self)
{
    gdk_window_process_all_updates();
    return self;
}

static VALUE
rg_process_updates(VALUE self, VALUE update_children)
{
    gdk_window_process_updates(_SELF(self), RVAL2CBOOL(update_children));
    return self;
}

static VALUE
rg_s_set_debug_updates(VALUE self, VALUE setting)
{
    gdk_window_set_debug_updates(RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_internal_paint_info(VALUE self)
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
rg_configure_finished(VALUE self)
{
    gdk_window_configure_finished(_SELF(self));
    return self;
}

static VALUE
rg_enable_synchronized_configure(VALUE self)
{
    gdk_window_enable_synchronized_configure(_SELF(self));

    if (rb_block_given_p()) {
        rb_ensure(rb_yield, self, rg_configure_finished, self);
    }
    return self;
}
#endif

static VALUE
rg_set_user_data(VALUE self, VALUE user_data)
{
    gdk_window_set_user_data(_SELF(self), (gpointer)RVAL2GOBJ(user_data));
    return self;
}

static VALUE
rg_set_override_redirect(VALUE self, VALUE override_redirect)
{
    gdk_window_set_override_redirect(_SELF(self), 
                                     RVAL2CBOOL(override_redirect));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_set_accept_focus(VALUE self, VALUE accept_focus)
{
    gdk_window_set_accept_focus(_SELF(self), RVAL2CBOOL(accept_focus));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_set_focus_on_map(VALUE self, VALUE focus_on_map)
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
rg_shape_combine_mask(VALUE self, VALUE shape_mask, VALUE offset_x, VALUE offset_y)
{
    gdk_window_shape_combine_mask(_SELF(self), 
                                  GDK_BITMAP(RVAL2GOBJ(shape_mask)), 
                                  NUM2INT(offset_x), NUM2INT(offset_y));
    return self;
}

static VALUE
rg_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_shape_combine_region(_SELF(self),
                                    NIL_P(shape_region) ? NULL : (GdkRegion*)RVAL2BOXED(shape_region, GDK_TYPE_REGION),
                                    INT2NUM(offset_x), INT2NUM(offset_y));
    return self;
}

static VALUE
rg_set_child_shapes(VALUE self)
{
    gdk_window_set_child_shapes(_SELF(self));
    return self;
}

static VALUE
rg_merge_child_shapes(VALUE self)
{
    gdk_window_merge_child_shapes(_SELF(self));
    return self;
}   

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_input_shape_combine_mask(VALUE self, VALUE mask, VALUE x, VALUE y)
{
    gdk_window_input_shape_combine_mask(_SELF(self),
                                        GDK_BITMAP(RVAL2GOBJ(mask)),
                                        NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_input_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_input_shape_combine_region(_SELF(self),
                                          (GdkRegion*)RVAL2BOXED(shape_region, GDK_TYPE_REGION),
                                          NUM2INT(offset_x),
                                          NUM2INT(offset_y));
    return self;
}

static VALUE
rg_set_child_input_shapes(VALUE self)
{
    gdk_window_set_child_input_shapes(_SELF(self));
    return self;
}

static VALUE
rg_merge_child_input_shapes(VALUE self)
{
    gdk_window_merge_child_input_shapes(_SELF(self));
    return self;
}

#endif

static VALUE
rg_set_static_gravities(VALUE self, VALUE use_static)
{
    gboolean ret = gdk_window_set_static_gravities(_SELF(self),
                                                   RVAL2CBOOL(use_static));
    if (! ret)
        rb_raise(rb_eRuntimeError, "couldn't turn on static gravity");

    return self;
}

static VALUE
rg_set_title(VALUE self, VALUE title)
{
    gdk_window_set_title(_SELF(self),RVAL2CSTR(title));
    return self;
}

static VALUE
rg_set_background(VALUE self, VALUE color)
{
    gdk_window_set_background(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
rg_set_back_pixmap(VALUE self, VALUE pixmap, VALUE parent_relative)
{
    gdk_window_set_back_pixmap(_SELF(self), 
                               GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                               RVAL2CBOOL(parent_relative));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_set_cursor(VALUE self, VALUE cursor)
{
    gdk_window_set_cursor(_SELF(self),
                          RVAL2BOXED(cursor, GDK_TYPE_CURSOR));
    return self;
}
#endif

static VALUE
rg_user_data(VALUE self)
{
    GObject *data = NULL;
    gdk_window_get_user_data(_SELF(self), (gpointer)&data);
    return GOBJ2RVAL(data);
}

static VALUE
rg_geometry(VALUE self)
{
    gint x, y, w, h, d;
    gdk_window_get_geometry(_SELF(self), &x, &y, &w, &h, &d);
    return rb_ary_new3(5, INT2NUM(x), INT2NUM(y),
                       INT2NUM(w), INT2NUM(h), INT2NUM(d));
}

static VALUE
rg_set_geometry_hints(VALUE self, VALUE geometry, VALUE geom_mask)
{
    gdk_window_set_geometry_hints(_SELF(self), 
                                  NIL_P(geometry) ? (GdkGeometry*)NULL : (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  RVAL2GFLAGS(geom_mask, GDK_TYPE_WINDOW_HINTS));
    return self;
}

static VALUE
rg_set_icon_list(VALUE self, VALUE rbpixbufs)
{
    GdkWindow *window = _SELF(self);
    GList *pixbufs = RVAL2GDKPIXBUFGLIST(rbpixbufs);

    gdk_window_set_icon_list(window, pixbufs);

    g_list_free(pixbufs);

    return self;
}

static VALUE
rg_set_modal_hint(VALUE self, VALUE modal)
{
    gdk_window_set_modal_hint(_SELF(self), RVAL2CBOOL(modal));
    return self;
}

static VALUE
rg_set_type_hint(VALUE self, VALUE hint)
{
    gdk_window_set_type_hint(_SELF(self), RVAL2GENUM(hint, GDK_TYPE_WINDOW_TYPE_HINT));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_type_hint(VALUE self)
{
    return GENUM2RVAL(gdk_window_get_type_hint(_SELF(self)), GDK_TYPE_WINDOW_TYPE_HINT);
}
#endif

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_set_skip_taskbar_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_taskbar_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}

static VALUE
rg_set_skip_pager_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_pager_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_set_urgency_hint(VALUE self, VALUE hint)
{
    gdk_window_set_urgency_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}
#endif

static VALUE
rg_position(VALUE self)
{
    gint x, y;
    gdk_window_get_position(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_root_origin(VALUE self)
{
    int x, y;
    gdk_window_get_root_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_frame_extents(VALUE self)
{
    GdkRectangle rect;
    gdk_window_get_frame_extents(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
rg_origin(VALUE self)
{
    gint x, y;
    gdk_window_get_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_pointer(VALUE self)
{
    gint x, y;
    GdkModifierType state;
    GdkWindow* ret = gdk_window_get_pointer(_SELF(self), &x, &y, &state);
    return rb_ary_new3(4, GOBJ2RVAL(ret), INT2NUM(x), INT2NUM(y), GFLAGS2RVAL(state, GDK_TYPE_MODIFIER_TYPE));
}

static VALUE
rg_parent(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_parent(_SELF(self)));
}

static VALUE
rg_toplevel(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_toplevel(_SELF(self)));
}

static VALUE
rg_children(VALUE self)
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
rg_events(VALUE self)
{
    return GFLAGS2RVAL(gdk_window_get_events(_SELF(self)), GDK_TYPE_EVENT_MASK);
}

static VALUE
rg_set_events(VALUE self, VALUE mask)
{
    gdk_window_set_events(_SELF(self), RVAL2GFLAGS(mask, GDK_TYPE_EVENT_MASK));
    return self;
}

static VALUE
rg_set_icon(VALUE self, VALUE icon, VALUE pixmap, VALUE mask)
{
    gdk_window_set_icon(_SELF(self), NIL_P(icon) ? NULL :_SELF(icon),
                        NIL_P(pixmap) ? NULL : GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                        NIL_P(mask) ? NULL : GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
rg_set_icon_name(VALUE self, VALUE name)
{
    gdk_window_set_icon_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_set_transient_for(VALUE self, VALUE parent)
{
    gdk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
rg_set_role(VALUE self, VALUE role)
{
    gdk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
rg_set_group(VALUE self, VALUE leader)
{
    gdk_window_set_group(_SELF(self), _SELF(leader));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_group(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_group(_SELF(self)));
}
#endif

static VALUE
rg_set_decorations(VALUE self, VALUE decor)
{
    gdk_window_set_decorations(_SELF(self), RVAL2GFLAGS(decor, GDK_TYPE_WM_DECORATION));
    return self;
}

static VALUE
rg_decorations(VALUE self)
{
    GdkWMDecoration decorations;
    gboolean ret = gdk_window_get_decorations(_SELF(self), &decorations);
    return ret ? GFLAGS2RVAL(decorations, GDK_TYPE_WM_DECORATION) : Qnil;
}

static VALUE
rg_set_functions(VALUE self, VALUE func)
{
    gdk_window_set_functions(_SELF(self), RVAL2GFLAGS(func, GDK_TYPE_WM_FUNCTION));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_set_composited(VALUE self, VALUE composited)
{
    gdk_window_set_composited(_SELF(self), RVAL2CBOOL(composited));
    return self;
}

static VALUE
rg_set_opacity(VALUE self, VALUE opacity)
{
    gdk_window_set_opacity(_SELF(self), NUM2DBL(opacity));
    return self;
}

static VALUE
rg_set_startup_id(VALUE self, VALUE startup_id)
{
    gdk_window_set_startup_id(_SELF(self), RVAL2CSTR_ACCEPT_NIL(startup_id));
    return self;
}
#endif

static VALUE
rg_s_toplevels(G_GNUC_UNUSED VALUE self)
{
    return GLIST2ARYF(gdk_window_get_toplevels());
}

static VALUE
rg_s_default_root_window(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_get_default_root_window());
}

/* Would you need this?
GdkPointerHooks* gdk_set_pointer_hooks      (const GdkPointerHooks *new_hooks);
*/

/* From X Window System Interaction */
static VALUE
rg_s_foreign_new(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_s_lookup(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_server_time(VALUE self)
{
    return UINT2NUM(gdk_x11_get_server_time(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_set_user_time(VALUE self, VALUE time)
{
    gdk_x11_window_set_user_time(_SELF(self), NUM2UINT(time));
    return Qnil;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_move_to_current_desktop(VALUE self)
{
    gdk_x11_window_move_to_current_desktop(_SELF(self));
    return self;
}
#endif

#endif

void
Init_gtk_gdk_window(VALUE mGdk)
{
    GObjectClass *g_class;
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_WINDOW, "Window", mGdk);

    g_class = g_type_class_peek(GDK_TYPE_WINDOW);

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(destroy, 0);
    RG_DEF_METHOD(window_type, 0);
    RG_DEF_SMETHOD(at_pointer, 0);
    RG_DEF_SMETHOD(constrain_size, 4);
    RG_DEF_SMETHOD(process_all_updates, 0);
    RG_DEF_SMETHOD(set_debug_updates, 1);
    RG_DEF_METHOD(show, 0);
    RG_DEF_METHOD(show_unraised, 0);
    RG_DEF_METHOD(hide, 0);
    RG_DEF_METHOD_P(visible, 0);
    RG_DEF_METHOD_P(viewable, 0);
    RG_DEF_METHOD(withdraw, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(iconify, 0);
    RG_DEF_METHOD(deiconify, 0);
    RG_DEF_METHOD(stick, 0);
    RG_DEF_METHOD(unstick, 0);
    RG_DEF_METHOD(maximize, 0);
    RG_DEF_METHOD(unmaximize, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(fullscreen, 0);
    RG_DEF_METHOD(unfullscreen, 0);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_keep_above, 1);
    RG_DEF_METHOD(set_keep_below, 1);
#endif
    RG_DEF_METHOD(move, 2);
    RG_DEF_METHOD(resize, 2);
    RG_DEF_METHOD(move_resize, 4);
    RG_DEF_METHOD(scroll, 2);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(move_region, 3);
#endif
    RG_DEF_METHOD(reparent, 3);
    RG_DEF_METHOD(clear, 0);
    RG_DEF_METHOD(clear_area, -1);
    RG_DEF_METHOD(raise, 0);
    RG_DEF_METHOD(lower, 0);
    RG_DEF_METHOD(focus, 1);
    RG_DEF_METHOD(register_dnd, 0);
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(beep, 0);
#endif
    RG_DEF_METHOD(begin_resize_drag, 5);
    RG_DEF_METHOD(begin_move_drag, 4);
    RG_DEF_METHOD(begin_paint, 1);
    RG_DEF_METHOD(end_paint, 0);
    RG_DEF_METHOD(invalidate, 2);
    RG_DEF_METHOD(invalidate_maybe_recurse, 1);
    RG_DEF_METHOD(update_area, 0);
    RG_DEF_METHOD(freeze_updates, 0);
    RG_DEF_METHOD(thaw_updates, 0);
    RG_DEF_METHOD(process_updates, 1);
    RG_DEF_METHOD(internal_paint_info, 0);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(configure_finished, 0);
    RG_DEF_METHOD(enable_synchronized_configure, 0);
#endif
    RG_DEF_METHOD(set_user_data, 1);
    RG_DEF_METHOD(set_override_redirect, 1);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_accept_focus, 1);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(set_focus_on_map, 1);
#endif
    RG_DEF_METHOD(shape_combine_mask, 3);
    RG_DEF_METHOD(shape_combine_region, 3);
    RG_DEF_METHOD(set_child_shapes, 0);
    RG_DEF_METHOD(merge_child_shapes, 0);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(input_shape_combine_mask, 3);
    RG_DEF_METHOD(input_shape_combine_region, 3);
    RG_DEF_METHOD(set_child_input_shapes, 0);
    RG_DEF_METHOD(merge_child_input_shapes, 0);
#endif
    RG_DEF_METHOD(set_static_gravities, 1);
    RG_DEF_METHOD(set_title, 1);
    RG_DEF_METHOD(set_background, 1);
    RG_DEF_METHOD(set_back_pixmap, 2);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(set_cursor, 1);
#endif
    RG_DEF_METHOD(user_data, 0);
    RG_DEF_METHOD(geometry, 0);
    RG_DEF_METHOD(set_geometry_hints, 2);
    RG_DEF_METHOD(set_icon_list, 1);
    RG_DEF_METHOD(set_modal_hint, 1);
    RG_DEF_METHOD(set_type_hint, 1);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(type_hint, 0);
#endif

#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(set_skip_taskbar_hint, 1);
    RG_DEF_METHOD(set_skip_pager_hint, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(set_urgency_hint, 1);
#endif
    RG_DEF_METHOD(position, 0);
    RG_DEF_METHOD(root_origin, 0);
    RG_DEF_METHOD(frame_extents, 0);
    RG_DEF_METHOD(origin, 0);
    RG_DEF_METHOD(pointer, 0);
    RG_DEF_METHOD(parent, 0);
    RG_DEF_METHOD(toplevel, 0);
    RG_DEF_SMETHOD(default_root_window, 0);
    RG_DEF_METHOD(children, 0);
    RG_DEF_METHOD(events, 0);
    RG_DEF_METHOD(set_events, 1);
    RG_DEF_METHOD(set_icon, 3);
    RG_DEF_METHOD(set_icon_name, 1);
    RG_DEF_METHOD(set_transient_for, 1);
    RG_DEF_METHOD(set_role, 1);
    RG_DEF_METHOD(set_group, 1);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(group, 0);
#endif
    RG_DEF_METHOD(set_decorations, 1);
    RG_DEF_METHOD(decorations, 0);
    RG_DEF_METHOD(set_functions, 1);
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(set_composited, 1);
    RG_DEF_METHOD(set_opacity, 1);
    RG_DEF_METHOD(set_startup_id, 1);
#endif
    RG_DEF_SMETHOD(toplevels, 0);

    RG_DEF_SMETHOD(foreign_new, -1);
    RG_DEF_SMETHOD(lookup, -1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GdkWindowType */
    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_TYPE, "GDK_WINDOW_");

    /* GdkWindowClass */
    G_DEF_CLASS(GDK_TYPE_WINDOW_CLASS, "Class", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_CLASS, "GDK_");

    /* GdkWindowHints */
    G_DEF_CLASS(GDK_TYPE_WINDOW_HINTS, "Hints", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_HINTS, "GDK_");

    /* GdkGravity */
    G_DEF_CLASS(GDK_TYPE_GRAVITY, "Gravity", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_GRAVITY, "GDK_");

    /* GdkWindowEdge */
    G_DEF_CLASS(GDK_TYPE_WINDOW_EDGE, "Edge", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_EDGE, "GDK_WINDOW_");

    /* GdkWindowTypeHint */
    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE_HINT, "TypeHint", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_TYPE_HINT, "GDK_WINDOW_");

    /* GdkWindowAttibutesType */
    G_DEF_CLASS(GDK_TYPE_WINDOW_ATTRIBUTES_TYPE, "AttributesType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WINDOW_ATTRIBUTES_TYPE, "GDK_");

    /* GdkFilterReturn */
    G_DEF_CLASS(GDK_TYPE_FILTER_RETURN, "FilterReturn", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_FILTER_RETURN, "GDK_");

    /* GdkModifierType */
    G_DEF_CLASS(GDK_TYPE_MODIFIER_TYPE, "ModifierType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_MODIFIER_TYPE, "GDK_");

    /* GdkWMDecoration */
    G_DEF_CLASS(GDK_TYPE_WM_DECORATION, "WMDecoration", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WM_DECORATION, "GDK_");

    /* GdkWMFunction */
    G_DEF_CLASS(GDK_TYPE_WM_FUNCTION, "WMFunction", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_WM_FUNCTION, "GDK_");

    rb_define_const(RG_TARGET_NAMESPACE, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));   

#ifdef GDK_WINDOWING_X11
    RG_DEF_METHOD(server_time, 0);

#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(set_user_time, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(move_to_current_desktop, 0);
#endif

    G_DEF_CLASS3("GdkWindowImplX11", "WindowImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkWindowImplWin32", "WindowImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkWindowFB", "WindowFB", mGdk);
#endif
}
