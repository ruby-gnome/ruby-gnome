/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cTextView
#define _SELF(self) (RVAL2GTKTEXTVIEW(self))

static ID id_buffer;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE buffer;
    GtkWidget* widget;
    rb_scan_args(argc, argv, "01", &buffer);

    if (NIL_P(buffer))
        widget = gtk_text_view_new();
    else {
        G_CHILD_SET(self, id_buffer, buffer);
        widget = gtk_text_view_new_with_buffer(RVAL2GTKTEXTBUFFER(buffer));
    }

    RBGTK_INITIALIZE(self, widget);
    return self;
}

static VALUE
rg_scroll_to_mark(VALUE self, VALUE mark, VALUE within_margin, VALUE use_align, VALUE xalign, VALUE yalign)
{
    gtk_text_view_scroll_to_mark(_SELF(self), RVAL2GTKTEXTMARK(mark), 
                                 NUM2DBL(within_margin), RVAL2CBOOL(use_align), 
                                 NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
rg_scroll_to_iter(VALUE self, VALUE iter, VALUE within_margin, VALUE use_align, VALUE xalign, VALUE yalign)
{
    return CBOOL2RVAL(gtk_text_view_scroll_to_iter(_SELF(self), RVAL2GTKTEXTITER(iter), 
                                                   NUM2DBL(within_margin), RVAL2CBOOL(use_align), 
                                                   NUM2DBL(xalign), NUM2DBL(yalign)));
}

static VALUE
rg_scroll_mark_onscreen(VALUE self, VALUE mark)
{
    gtk_text_view_scroll_mark_onscreen(_SELF(self), RVAL2GTKTEXTMARK(mark));
    return self;
}

static VALUE
rg_move_mark_onscreen(VALUE self, VALUE mark)
{
    return CBOOL2RVAL(gtk_text_view_move_mark_onscreen(_SELF(self), RVAL2GTKTEXTMARK(mark)));
}

static VALUE
rg_place_cursor_onscreen(VALUE self)
{
    return CBOOL2RVAL(gtk_text_view_place_cursor_onscreen(_SELF(self)));
}

static VALUE
rg_visible_rect(VALUE self)
{
    GdkRectangle rect;
    gtk_text_view_get_visible_rect(_SELF(self), &rect);
    return GDKRECTANGLE2RVAL(&rect);
}

static VALUE
rg_get_iter_location(VALUE self, VALUE iter)
{
    GdkRectangle rect;
    gtk_text_view_get_iter_location(_SELF(self), RVAL2GTKTEXTITER(iter), &rect);
    return GDKRECTANGLE2RVAL(&rect);
}

static VALUE
rg_get_line_at_y(VALUE self, VALUE y)
{
    GtkTextIter target_iter;
    gint line_top;
    gtk_text_view_get_line_at_y(_SELF(self), &target_iter, NUM2INT(y), &line_top);
    return rb_ary_new3(2, GTKTEXTITER2RVAL(&target_iter), INT2NUM(line_top));
}

static VALUE
rg_get_line_yrange(VALUE self, VALUE iter)
{
    int y, height;
    gtk_text_view_get_line_yrange(_SELF(self), RVAL2GTKTEXTITER(iter), &y, &height);

    return rb_ary_new3(2, INT2NUM(y), INT2NUM(height));
}

static VALUE
rg_get_iter_at_location(VALUE self, VALUE x, VALUE y)
{
    GtkTextIter iter;
    gtk_text_view_get_iter_at_location(_SELF(self), &iter, NUM2INT(x), NUM2INT(y));
    return GTKTEXTITER2RVAL(&iter);
}

static VALUE
rg_get_iter_at_position(VALUE self, VALUE x, VALUE y)
{
    GtkTextIter iter;
    gint trailing;
    gtk_text_view_get_iter_at_position(_SELF(self), &iter, &trailing, NUM2INT(x), NUM2INT(y));
    return rb_assoc_new(GTKTEXTITER2RVAL(&iter), INT2NUM(trailing));
}

static VALUE
rg_buffer_to_window_coords(VALUE self, VALUE wintype, VALUE buffer_x, VALUE buffer_y)
{
    int window_x, window_y;
    gtk_text_view_buffer_to_window_coords(_SELF(self), 
                                          RVAL2GTKTEXTWINDOWTYPE(wintype),
                                          NUM2INT(buffer_x), NUM2INT(buffer_y),
                                          &window_x, &window_y);
    return rb_ary_new3(2, INT2NUM(window_x), INT2NUM(window_y));
}

static VALUE
rg_window_to_buffer_coords(VALUE self, VALUE wintype, VALUE window_x, VALUE window_y)
{
    int buffer_x, buffer_y;
    gtk_text_view_window_to_buffer_coords(_SELF(self),
                                          RVAL2GTKTEXTWINDOWTYPE(wintype),
                                          NUM2INT(window_x), NUM2INT(window_y),
                                          &buffer_x, &buffer_y);
    return rb_ary_new3(2, INT2NUM(buffer_x), INT2NUM(buffer_y));
}

static VALUE
rg_get_window(VALUE self, VALUE wintype)
{
    GdkWindow* win = NULL;
    win = gtk_text_view_get_window(_SELF(self), 
                                   RVAL2GTKTEXTWINDOWTYPE(wintype));
    return win ? GOBJ2RVAL(win): Qnil;
}

static VALUE
rg_get_window_type(VALUE self, VALUE gdkwin)
{
    return GTKTEXTWINDOWTYPE2RVAL(gtk_text_view_get_window_type(_SELF(self),
                                                                RVAL2GDKWINDOW(gdkwin)));
}

static VALUE
rg_set_border_window_size(VALUE self, VALUE wintype, VALUE size)
{
    gtk_text_view_set_border_window_size(_SELF(self), 
                                         RVAL2GTKTEXTWINDOWTYPE(wintype), 
                                         NUM2INT(size));
    return self;
}

static VALUE
rg_get_border_window_size(VALUE self, VALUE wintype)
{
    return INT2NUM(gtk_text_view_get_border_window_size(
                       _SELF(self), 
                       RVAL2GTKTEXTWINDOWTYPE(wintype)));
}

static VALUE
rg_forward_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_forward_display_line(_SELF(self), RVAL2GTKTEXTITER(iter)));
}

static VALUE
rg_backward_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_backward_display_line(_SELF(self), RVAL2GTKTEXTITER(iter)));
}

static VALUE
rg_forward_display_line_end(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_forward_display_line_end(_SELF(self), RVAL2GTKTEXTITER(iter)));
}

static VALUE
rg_backward_display_line_start(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_backward_display_line_start(_SELF(self), RVAL2GTKTEXTITER(iter)));
}

static VALUE
rg_starts_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_starts_display_line(_SELF(self), RVAL2GTKTEXTITER(iter)));
}

static VALUE
rg_move_visually(VALUE self, VALUE iter, VALUE count)
{
    return CBOOL2RVAL(gtk_text_view_move_visually(_SELF(self), RVAL2GTKTEXTITER(iter), NUM2INT(count)));
}

static VALUE
rg_add_child_at_anchor(VALUE self, VALUE child, VALUE anchor)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_add_child_at_anchor(_SELF(self), RVAL2GTKWIDGET(child), 
                                      RVAL2GTKTEXTCHILDANCHOR(anchor));
    return self;
}

static VALUE
rg_add_child_in_window(VALUE self, VALUE child, VALUE which_window, VALUE xpos, VALUE ypos)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_add_child_in_window(_SELF(self), RVAL2GTKWIDGET(child),
                                      RVAL2GTKTEXTWINDOWTYPE(which_window),
                                      NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
rg_move_child(VALUE self, VALUE child, VALUE xpos, VALUE ypos)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_move_child(_SELF(self), RVAL2GTKWIDGET(child),
                             NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
rg_default_attributes(VALUE self)
{
    return GTKTEXTATTRIBUTES2RVAL(gtk_text_view_get_default_attributes(_SELF(self)));
}

static VALUE
rg_get_cursor_locations(int argc, VALUE *argv, VALUE self)
{
    VALUE iter;
    GdkRectangle strong, weak;

    rb_scan_args(argc, argv, "01", &iter);
    gtk_text_view_get_cursor_locations(_SELF(self),
                                       NIL_P(iter) ? NULL : RVAL2GTKTEXTITER(iter),
                                       &strong, &weak);

    return rb_ary_new3(2, GDKRECTANGLE2RVAL(&strong), GDKRECTANGLE2RVAL(&weak));
}

static VALUE
rg_im_context_filter_keypress(VALUE self, VALUE event)
{
    return CBOOL2RVAL(gtk_text_view_im_context_filter_keypress(_SELF(self),
                                                               RVAL2GDKEVENTKEY(event)));
}

static VALUE
rg_reset_im_context(VALUE self)
{
    gtk_text_view_reset_im_context(_SELF(self));

    return self;
}

void
Init_gtk_textview(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_VIEW, "TextView", mGtk);

    id_buffer = rb_intern("buffer");

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(scroll_to_mark, 5);
    RG_DEF_METHOD(scroll_to_iter, 5);
    RG_DEF_METHOD(scroll_mark_onscreen, 1);
    RG_DEF_METHOD(move_mark_onscreen, 1);
    RG_DEF_METHOD(place_cursor_onscreen, 0);
    RG_DEF_METHOD(visible_rect, 0);
    RG_DEF_METHOD(get_iter_location, 1);
    RG_DEF_METHOD(get_line_at_y, 1);
    RG_DEF_METHOD(get_line_yrange, 1);
    RG_DEF_METHOD(get_iter_at_location, 2);
    RG_DEF_METHOD(get_iter_at_position, 2);
    RG_DEF_METHOD(buffer_to_window_coords, 3);
    RG_DEF_METHOD(window_to_buffer_coords, 3);
    RG_DEF_METHOD(get_window, 1);
    RG_DEF_METHOD(get_window_type, 1);
    RG_DEF_METHOD(set_border_window_size, 2);
    RG_DEF_METHOD(get_border_window_size, 1);
    RG_DEF_METHOD(forward_display_line, 1);
    RG_DEF_METHOD(backward_display_line, 1);
    RG_DEF_METHOD(forward_display_line_end, 1);
    RG_DEF_METHOD(backward_display_line_start, 1);
    RG_DEF_METHOD(starts_display_line, 1);
    RG_DEF_METHOD(move_visually, 2);
    RG_DEF_METHOD(add_child_at_anchor, 2);
    RG_DEF_METHOD(add_child_in_window, 4);
    RG_DEF_METHOD(move_child, 3);
    RG_DEF_METHOD(default_attributes, 0);
    RG_DEF_METHOD(get_cursor_locations, -1);
    RG_DEF_METHOD(im_context_filter_keypress, 1);
    RG_DEF_METHOD(reset_im_context, 0);

    G_DEF_CLASS(GTK_TYPE_TEXT_WINDOW_TYPE, "WindowType", RG_TARGET_NAMESPACE);

    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_VALIDATE", INT2FIX(GTK_TEXT_VIEW_PRIORITY_VALIDATE));
}
