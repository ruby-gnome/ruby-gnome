/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextview.c -

  $Author $
  $Date: 2002/10/25 17:51:26 $

  Copyright (C) 2002 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GTK_TEXT_VIEW(RVAL2GOBJ(self)))
#define RVAL2BUFFER(b) (GTK_TEXT_BUFFER(RVAL2GOBJ(b)))
#define RVAL2MARK(m) (GTK_TEXT_MARK(RVAL2GOBJ(m)))
#define RVAL2ITR(i) ((GtkTextIter*)(RVAL2BOXED(i, GTK_TYPE_TEXT_ITER)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TEXT_ITER))

static VALUE
textview_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE buffer;
    GtkWidget* widget;
    rb_scan_args(argc, argv, "01", &buffer);

    if (NIL_P(buffer))
        widget = gtk_text_view_new();
    else
        widget = gtk_text_view_new_with_buffer(RVAL2BUFFER(buffer));

    RBGTK_INITIALIZE(self, widget);
    return self;
}

static VALUE
textview_set_buffer(self, buf)
    VALUE self, buf;
{
    gtk_text_view_set_buffer(_SELF(self), 
                             NIL_P(buf) ? NULL : RVAL2BUFFER(buf));
    return self;
}

static VALUE
textview_get_buffer(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_view_get_buffer(_SELF(self)));
}

static VALUE
textview_scroll_to_mark(self, mark, within_margin, use_align, xalign, yalign)
    VALUE self, mark, within_margin, use_align, xalign, yalign;
{
    gtk_text_view_scroll_to_mark(_SELF(self), RVAL2MARK(mark), 
                                 NUM2DBL(within_margin), RTEST(use_align), 
                                 NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
textview_scroll_to_iter(self, iter, within_margin, use_align, xalign, yalign)
    VALUE self, iter, within_margin, use_align, xalign, yalign;
{
    gtk_text_view_scroll_to_iter(_SELF(self), RVAL2ITR(iter), 
                                 NUM2DBL(within_margin), RTEST(use_align), 
                                 NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
textview_scroll_mark_onscreen(self, mark)
    VALUE self, mark;
{
    gtk_text_view_scroll_mark_onscreen(_SELF(self), RVAL2MARK(mark));
    return self;
}

static VALUE
textview_move_mark_onscreen(self, mark)
    VALUE self, mark;
{
    return gtk_text_view_move_mark_onscreen(_SELF(self), RVAL2MARK(mark)) ? Qtrue: Qfalse;
}

static VALUE
textview_place_cursor_onscreen(self)
    VALUE self;
{
    return gtk_text_view_place_cursor_onscreen(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
textview_get_visible_rect(self)
    VALUE self;
{
    GdkRectangle rect;
    gtk_text_view_get_visible_rect(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
textview_get_iter_location(self, iter)
    VALUE self, iter;
{
    GdkRectangle rect;
    gtk_text_view_get_iter_location(_SELF(self), RVAL2ITR(iter), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
textview_get_line_at_y(self, y)
    VALUE self, y;
{
    GtkTextIter target_iter;
    gint line_top;
    gtk_text_view_get_line_at_y(_SELF(self), &target_iter, NUM2INT(y), &line_top);
    return rb_ary_new3(2, ITR2RVAL(&target_iter), INT2NUM(line_top));
}

static VALUE
textview_get_line_yrange(self, iter)
    VALUE self, iter;
{
    int y, height;
    gtk_text_view_get_line_yrange(_SELF(self), RVAL2ITR(iter), &y, &height);

    return rb_ary_new3(2, INT2NUM(y), INT2NUM(height));
}

static VALUE
textview_get_iter_at_location(self, x, y)
    VALUE self, x, y;
{
    GtkTextIter iter;
    gtk_text_view_get_iter_at_location(_SELF(self), &iter, NUM2INT(x), NUM2INT(y));
    return ITR2RVAL(&iter);
}

static VALUE
textview_buffer_to_window_coords(self, wintype, buffer_x, buffer_y)
    VALUE self, wintype, buffer_x, buffer_y;
{
    int window_x, window_y;
    gtk_text_view_buffer_to_window_coords(_SELF(self), FIX2INT(wintype),
                                          NUM2INT(buffer_x), NUM2INT(buffer_y),
                                          &window_x, &window_y);
    return rb_ary_new3(2, INT2NUM(window_x), INT2NUM(window_y));
}

static VALUE
textview_window_to_buffer_coords(self, wintype, window_x, window_y)
    VALUE self, wintype, window_x, window_y;
{
    int buffer_x, buffer_y;
    gtk_text_view_window_to_buffer_coords(_SELF(self), FIX2INT(wintype),
                                          NUM2INT(window_x), NUM2INT(window_y),
                                          &buffer_x, &buffer_y);
    return rb_ary_new3(2, INT2NUM(buffer_x), INT2NUM(buffer_y));
}

static VALUE
textview_get_window(self, wintype)
    VALUE self, wintype;
{
    GdkWindow* win = gtk_text_view_get_window(_SELF(self), NUM2INT(win));
    return win ? GOBJ2RVAL(win): Qnil;
}

static VALUE
textview_get_window_type(self, gdkwin)
    VALUE self, gdkwin;
{
    return INT2FIX(gtk_text_view_get_window_type(_SELF(self), RVAL2GOBJ(gdkwin)));
}

static VALUE
textview_set_border_window_size(self, wintype, size)
    VALUE self, wintype, size;
{
    gtk_text_view_set_border_window_size(_SELF(self), FIX2INT(wintype), NUM2INT(size));
    return self;
}

static VALUE
textview_get_border_window_size(self, wintype)
    VALUE self, wintype;
{
    return INT2NUM(gtk_text_view_get_border_window_size(_SELF(self), NUM2INT(wintype)));
}

static VALUE
textview_forward_display_line(self, iter)
    VALUE self, iter;
{
    return gtk_text_view_forward_display_line(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
}

static VALUE
textview_backward_display_line(self, iter)
    VALUE self, iter;
{
    return gtk_text_view_backward_display_line(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
}

static VALUE
textview_forward_display_line_end(self, iter)
    VALUE self, iter;
{
    return gtk_text_view_forward_display_line_end(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
}

static VALUE
textview_backward_display_line_start(self, iter)
    VALUE self, iter;
{
    return gtk_text_view_backward_display_line_start(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
}

static VALUE
textview_starts_display_line(self, iter)
    VALUE self, iter;
{
    return gtk_text_view_starts_display_line(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
}

static VALUE
textview_move_visually(self, iter, count)
    VALUE self, iter, count;
{
    return gtk_text_view_move_visually(_SELF(self), RVAL2ITR(iter), NUM2INT(count)) ? Qtrue: Qfalse;
}

static VALUE
textview_add_child_at_anchor(self, child, anchor)
    VALUE self, child, anchor;
{
    gtk_text_view_add_child_at_anchor(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                                      GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(anchor)));
    return self;
}

static VALUE
textview_add_child_in_window(self, child, which_window, xpos, ypos)
    VALUE self, child, which_window, xpos, ypos;
{
    gtk_text_view_add_child_in_window(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                                      FIX2INT(which_window), 
                                      NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
textview_move_child(self, child, xpos, ypos)
    VALUE self, child, xpos, ypos;
{
    gtk_text_view_move_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                             NUM2INT(xpos), NUM2INT(ypos));
    return self;
}

static VALUE
textview_get_default_attributes(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_text_view_get_default_attributes(_SELF(self)), 
                      GTK_TYPE_TEXT_ATTRIBUTES);
}

void
Init_gtk_textview()
{
    VALUE cTextView = G_DEF_CLASS(GTK_TYPE_TEXT_VIEW, "TextView", mGtk);

    rb_define_method(cTextView, "initialize", textview_initialize, -1);
    rb_define_method(cTextView, "set_buffer", textview_set_buffer, 1);
    rb_define_method(cTextView, "buffer", textview_get_buffer, 0);
    rb_define_method(cTextView, "scroll_to_mark", textview_scroll_to_mark, 5);
    rb_define_method(cTextView, "scroll_to_iter", textview_scroll_to_iter, 5);
    rb_define_method(cTextView, "scroll_mark_onscreen", textview_scroll_mark_onscreen, 1);
    rb_define_method(cTextView, "move_mark_onscreen", textview_move_mark_onscreen, 1);
    rb_define_method(cTextView, "place_sursor_onscreen", textview_place_cursor_onscreen, 0);
    rb_define_method(cTextView, "visible_rect", textview_get_visible_rect, 0);
    rb_define_method(cTextView, "get_iter_location", textview_get_iter_location, 1);
    rb_define_method(cTextView, "get_line_at_y", textview_get_line_at_y, 1);
    rb_define_method(cTextView, "get_line_yrange", textview_get_line_yrange, 1);
    rb_define_method(cTextView, "get_iter_at_location", textview_get_iter_at_location, 2);
    rb_define_method(cTextView, "buffer_to_window_coords", textview_buffer_to_window_coords, 3);
    rb_define_method(cTextView, "window_to_buffer_coords", textview_window_to_buffer_coords, 3);
    rb_define_method(cTextView, "get_window", textview_get_window, 1);
    rb_define_method(cTextView, "get_window_type", textview_get_window_type, 1);
    rb_define_method(cTextView, "set_border_window_size", textview_set_border_window_size, 2);
    rb_define_method(cTextView, "get_border_window_size", textview_get_border_window_size, 1);
    rb_define_method(cTextView, "forward_display_line", textview_forward_display_line, 1);
    rb_define_method(cTextView, "backward_display_line", textview_backward_display_line, 1);
    rb_define_method(cTextView, "forward_display_line_end", textview_forward_display_line_end, 1);
    rb_define_method(cTextView, "backward_display_line_start", textview_backward_display_line_start, 1);
    rb_define_method(cTextView, "starts_display_line", textview_starts_display_line, 1);
    rb_define_method(cTextView, "move_visually", textview_move_visually, 2);
    rb_define_method(cTextView, "add_child_at_anchor", textview_add_child_at_anchor, 2);
    rb_define_method(cTextView, "add_child_in_window", textview_add_child_in_window, 4);
    rb_define_method(cTextView, "move_child", textview_move_child, 2);
    rb_define_method(cTextView, "default_attributes", textview_get_default_attributes, 0);

    G_DEF_SETTERS(cTextView);

    /* GtkTextWindowType */
    rb_define_const(cTextView, "WINDOW_PRIVATE", GTK_TEXT_WINDOW_PRIVATE);
    rb_define_const(cTextView, "WINDOW_WIDGET", GTK_TEXT_WINDOW_WIDGET);
    rb_define_const(cTextView, "WINDOW_TEXT", GTK_TEXT_WINDOW_TEXT);
    rb_define_const(cTextView, "WINDOW_LEFT", GTK_TEXT_WINDOW_LEFT);
    rb_define_const(cTextView, "WINDOW_RIGHT", GTK_TEXT_WINDOW_RIGHT);
    rb_define_const(cTextView, "WINDOW_TOP", GTK_TEXT_WINDOW_TOP);
    rb_define_const(cTextView, "WINDOW_BOTTOM", GTK_TEXT_WINDOW_BOTTOM);
 
    rb_define_const(cTextView, "PRIORITY_VALIDATE", GTK_TEXT_VIEW_PRIORITY_VALIDATE);
}
