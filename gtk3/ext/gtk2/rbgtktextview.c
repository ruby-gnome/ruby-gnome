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

#include "global.h"

#define RG_TARGET_NAMESPACE cTextView
#define _SELF(self) (GTK_TEXT_VIEW(RVAL2GOBJ(self)))
#define RVAL2BUFFER(b) (GTK_TEXT_BUFFER(RVAL2GOBJ(b)))
#define RVAL2MARK(m) (GTK_TEXT_MARK(RVAL2GOBJ(m)))
#define RVAL2ITR(i) ((GtkTextIter*)(RVAL2BOXED(i, GTK_TYPE_TEXT_ITER)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))

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
        widget = gtk_text_view_new_with_buffer(RVAL2BUFFER(buffer));
    }

    RBGTK_INITIALIZE(self, widget);
    return self;
}

static VALUE
textview_set_buffer(VALUE self, VALUE buf)
{
    G_CHILD_SET(self, id_buffer, buf);
    gtk_text_view_set_buffer(_SELF(self), 
                             NIL_P(buf) ? NULL : RVAL2BUFFER(buf));
    return self;
}

static VALUE
textview_get_buffer(VALUE self)
{
    VALUE buf = GOBJ2RVAL(gtk_text_view_get_buffer(_SELF(self)));
    G_CHILD_SET(self, id_buffer, buf);

    return buf;
}

static VALUE
rg_scroll_to_mark(VALUE self, VALUE mark, VALUE within_margin, VALUE use_align, VALUE xalign, VALUE yalign)
{
    gtk_text_view_scroll_to_mark(_SELF(self), RVAL2MARK(mark), 
                                 NUM2DBL(within_margin), RVAL2CBOOL(use_align), 
                                 NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
rg_scroll_to_iter(VALUE self, VALUE iter, VALUE within_margin, VALUE use_align, VALUE xalign, VALUE yalign)
{
    return CBOOL2RVAL(gtk_text_view_scroll_to_iter(_SELF(self), RVAL2ITR(iter), 
                                                   NUM2DBL(within_margin), RVAL2CBOOL(use_align), 
                                                   NUM2DBL(xalign), NUM2DBL(yalign)));
}

static VALUE
rg_scroll_mark_onscreen(VALUE self, VALUE mark)
{
    gtk_text_view_scroll_mark_onscreen(_SELF(self), RVAL2MARK(mark));
    return self;
}

static VALUE
rg_move_mark_onscreen(VALUE self, VALUE mark)
{
    return CBOOL2RVAL(gtk_text_view_move_mark_onscreen(_SELF(self), RVAL2MARK(mark)));
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
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
rg_get_iter_location(VALUE self, VALUE iter)
{
    GdkRectangle rect;
    gtk_text_view_get_iter_location(_SELF(self), RVAL2ITR(iter), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
rg_get_line_at_y(VALUE self, VALUE y)
{
    GtkTextIter target_iter;
    gint line_top;
    gtk_text_view_get_line_at_y(_SELF(self), &target_iter, NUM2INT(y), &line_top);
    return rb_ary_new3(2, ITR2RVAL(&target_iter), INT2NUM(line_top));
}

static VALUE
rg_get_line_yrange(VALUE self, VALUE iter)
{
    int y, height;
    gtk_text_view_get_line_yrange(_SELF(self), RVAL2ITR(iter), &y, &height);

    return rb_ary_new3(2, INT2NUM(y), INT2NUM(height));
}

static VALUE
rg_get_iter_at_location(VALUE self, VALUE x, VALUE y)
{
    GtkTextIter iter;
    gtk_text_view_get_iter_at_location(_SELF(self), &iter, NUM2INT(x), NUM2INT(y));
    return ITR2RVAL(&iter);
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_get_iter_at_position(VALUE self, VALUE x, VALUE y)
{
    GtkTextIter iter;
    gint trailing;
    gtk_text_view_get_iter_at_position(_SELF(self), &iter, &trailing, NUM2INT(x), NUM2INT(y));
    return rb_assoc_new(ITR2RVAL(&iter), INT2NUM(trailing));
}
#endif

static VALUE
rg_buffer_to_window_coords(VALUE self, VALUE wintype, VALUE buffer_x, VALUE buffer_y)
{
    int window_x, window_y;
    gtk_text_view_buffer_to_window_coords(_SELF(self), 
                                          RVAL2GENUM(wintype, GTK_TYPE_TEXT_WINDOW_TYPE),
                                          NUM2INT(buffer_x), NUM2INT(buffer_y),
                                          &window_x, &window_y);
    return rb_ary_new3(2, INT2NUM(window_x), INT2NUM(window_y));
}

static VALUE
rg_window_to_buffer_coords(VALUE self, VALUE wintype, VALUE window_x, VALUE window_y)
{
    int buffer_x, buffer_y;
    gtk_text_view_window_to_buffer_coords(_SELF(self),
                                          RVAL2GENUM(wintype, GTK_TYPE_TEXT_WINDOW_TYPE),
                                          NUM2INT(window_x), NUM2INT(window_y),
                                          &buffer_x, &buffer_y);
    return rb_ary_new3(2, INT2NUM(buffer_x), INT2NUM(buffer_y));
}

static VALUE
rg_get_window(VALUE self, VALUE wintype)
{
    GdkWindow* win = NULL;
    win = gtk_text_view_get_window(_SELF(self), 
                                   RVAL2GENUM(wintype, GTK_TYPE_TEXT_WINDOW_TYPE));
    return win ? GOBJ2RVAL(win): Qnil;
}

static VALUE
rg_get_window_type(VALUE self, VALUE gdkwin)
{
    return GENUM2RVAL(gtk_text_view_get_window_type(_SELF(self), RVAL2GOBJ(gdkwin)), 
                      GTK_TYPE_TEXT_WINDOW_TYPE);
}

static VALUE
rg_set_border_window_size(VALUE self, VALUE wintype, VALUE size)
{
    gtk_text_view_set_border_window_size(_SELF(self), 
                                         RVAL2GENUM(wintype, GTK_TYPE_TEXT_WINDOW_TYPE), 
                                         NUM2INT(size));
    return self;
}

static VALUE
rg_get_border_window_size(VALUE self, VALUE wintype)
{
    return INT2NUM(gtk_text_view_get_border_window_size(
                       _SELF(self), 
                       RVAL2GENUM(wintype, GTK_TYPE_TEXT_WINDOW_TYPE)));
}

static VALUE
rg_forward_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_forward_display_line(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
rg_backward_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_backward_display_line(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
rg_forward_display_line_end(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_forward_display_line_end(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
rg_backward_display_line_start(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_backward_display_line_start(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
rg_starts_display_line(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_text_view_starts_display_line(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
rg_move_visually(VALUE self, VALUE iter, VALUE count)
{
    return CBOOL2RVAL(gtk_text_view_move_visually(_SELF(self), RVAL2ITR(iter), NUM2INT(count)));
}

static VALUE
rg_add_child_at_anchor(VALUE self, VALUE child, VALUE anchor)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_add_child_at_anchor(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                                      GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(anchor)));
    return self;
}

static VALUE
rg_add_child_in_window(VALUE self, VALUE child, VALUE which_window, VALUE xpos, VALUE ypos)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_add_child_in_window(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                                      RVAL2GENUM(which_window, GTK_TYPE_TEXT_WINDOW_TYPE),
                                      NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
rg_move_child(VALUE self, VALUE child, VALUE xpos, VALUE ypos)
{
    G_CHILD_ADD(self, child);
    gtk_text_view_move_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                             NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
rg_default_attributes(VALUE self)
{
    return BOXED2RVAL(gtk_text_view_get_default_attributes(_SELF(self)), 
                      GTK_TYPE_TEXT_ATTRIBUTES);
}

void
Init_gtk_textview(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_VIEW, "TextView", mGtk);

    id_buffer = rb_intern("buffer");

    RG_DEF_METHOD(initialize, -1);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "buffer", textview_set_buffer, 1);
    G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "buffer", textview_get_buffer, 0);
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
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(get_iter_at_position, 2);
#endif
    RG_DEF_METHOD(buffer_to_window_coords, 3);
    RG_DEF_METHOD(window_to_buffer_coords, 3);
    RG_DEF_METHOD(get_window, 1);
    RG_DEF_METHOD(get_window_type, 1);
    RG_DEF_METHOD(set_border_window_size, 2);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "border_window_size");
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

    /* GtkTextWindowType */
    G_DEF_CLASS(GTK_TYPE_TEXT_WINDOW_TYPE, "WindowType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TEXT_WINDOW_TYPE, "GTK_TEXT_");

    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_VALIDATE", INT2FIX(GTK_TEXT_VIEW_PRIORITY_VALIDATE));
}
