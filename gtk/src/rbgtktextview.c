/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextview.c -

  $Author $
  $Date: 2002/10/13 17:32:38 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/
#include "global.h"

static VALUE
textview_set_buffer(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    GtkTextBuffer *gv0;
    g_self = G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                        GtkTextView);
    gv0 = NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                        GTK_TYPE_TEXT_BUFFER,
                                                        GtkTextBuffer);
    gtk_text_view_set_buffer(g_self, gv0);
    return self;
}

static VALUE
textview_get_buffer(self)
    VALUE self;
{
    GtkTextView *g_self;
    GtkTextBuffer *ret;
    g_self = G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                        GtkTextView);
    ret = gtk_text_view_get_buffer(g_self);
    return ret ? GOBJ2RVAL(ret) : Qnil;
}

static VALUE
textview_scroll_to_iter(self, v0, v1, v2, v3, v4)
    VALUE self, v0, v1, v2, v3, v4;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    gdouble gv1;
    gboolean gv2;
    gdouble gv3;
    gdouble gv4;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    gv1 = NUM2DBL(v1);
    gv2 = RTEST(v2);
    gv3 = NUM2DBL(v3);
    gv4 = NUM2DBL(v4);
    ret = gtk_text_view_scroll_to_iter(g_self, gv0, gv1, gv2, gv3, gv4);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_scroll_to_mark(self, v0, v1, v2, v3, v4)
    VALUE self, v0, v1, v2, v3, v4;
{
    GtkTextView *g_self;
    GtkTextMark *gv0;
    gdouble gv1;
    gboolean gv2;
    gdouble gv3;
    gdouble gv4;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_TEXT_MARK,
                                                      GtkTextMark);
    gv1 = NUM2DBL(v1);
    gv2 = RTEST(v2);
    gv3 = NUM2DBL(v3);
    gv4 = NUM2DBL(v4);
    gtk_text_view_scroll_to_mark(g_self, gv0, gv1, gv2, gv3, gv4);
    return self;

}

static VALUE
textview_scroll_mark_onscreen(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    GtkTextMark *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_TEXT_MARK,
                                                      GtkTextMark);
    gtk_text_view_scroll_mark_onscreen(g_self, gv0);
    return self;

}

static VALUE
textview_move_mark_onscreen(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextMark *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_TEXT_MARK,
                                                      GtkTextMark);
    ret = gtk_text_view_move_mark_onscreen(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_place_cursor_onscreen(self)
    VALUE self;
{
    GtkTextView *g_self;
    gboolean ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_place_cursor_onscreen(g_self);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_get_visible_rect(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    GdkRectangle *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GdkRectangle *) RVAL2BOXED(v0,
                                                        GDK_TYPE_RECTANGLE);
    gtk_text_view_get_visible_rect(g_self, gv0);
    return self;

}

static VALUE
textview_get_iter_location(self, v0, v1)
    VALUE self, v0, v1;
{
    GtkTextView *g_self;
    const GtkTextIter *gv0;
    GdkRectangle *gv1;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (const GtkTextIter *) RVAL2BOXED(v0,
                                                             GTK_TYPE_TEXT_ITER);
    gv1 =
        NIL_P(v1) ? NULL : (GdkRectangle *) RVAL2BOXED(v1,
                                                        GDK_TYPE_RECTANGLE);
    gtk_text_view_get_iter_location(g_self, gv0, gv1);
    return self;

}

static VALUE
textview_get_iter_at_location(self, v0, v1, v2)
    VALUE self, v0, v1, v2;
{
    GtkTextView *g_self;
    GtkTextIter *gv0;
    gint gv1;
    gint gv2;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    gv1 = NUM2INT(v1);
    gv2 = NUM2INT(v2);
    gtk_text_view_get_iter_at_location(g_self, gv0, gv1, gv2);
    return self;

}

/* gtk_text_view_get_line_yrange not generated */

/* gtk_text_view_get_line_at_y not generated */

/* gtk_text_view_buffer_to_window_coords not generated */

/* gtk_text_view_window_to_buffer_coords not generated */

static VALUE
textview_get_window(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    GdkWindow *ret;
    GtkTextWindowType gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2UINT(v0);
    ret = gtk_text_view_get_window(g_self, gv0);
    return ret ? GOBJ2RVAL(ret) : Qnil;

}

static VALUE
textview_get_window_type(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    GtkTextWindowType ret;
    GdkWindow *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GDK_TYPE_WINDOW,
                                                      GdkWindow);
    ret = gtk_text_view_get_window_type(g_self, gv0);
    return UINT2NUM(ret);

}

static VALUE
textview_set_border_window_size(self, v0, v1)
    VALUE self, v0, v1;
{
    GtkTextView *g_self;
    GtkTextWindowType gv0;
    gint gv1;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2UINT(v0);
    gv1 = NUM2INT(v1);
    gtk_text_view_set_border_window_size(g_self, gv0, gv1);
    return self;

}

static VALUE
textview_get_border_window_size(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gint ret;
    GtkTextWindowType gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2UINT(v0);
    ret = gtk_text_view_get_border_window_size(g_self, gv0);
    return INT2NUM(ret);

}

static VALUE
textview_forward_display_line(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    ret = gtk_text_view_forward_display_line(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_backward_display_line(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    ret = gtk_text_view_backward_display_line(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_forward_display_line_end(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    ret = gtk_text_view_forward_display_line_end(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_backward_display_line_start(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    ret = gtk_text_view_backward_display_line_start(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_starts_display_line(self, v0)
    VALUE self, v0;
{
    GtkTextView *g_self;
    gboolean ret;
    const GtkTextIter *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (const GtkTextIter *) RVAL2BOXED(v0,
                                                             GTK_TYPE_TEXT_ITER);
    ret = gtk_text_view_starts_display_line(g_self, gv0);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_move_visually(self, v0, v1)
    VALUE self, v0, v1;
{
    GtkTextView *g_self;
    gboolean ret;
    GtkTextIter *gv0;
    gint gv1;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (GtkTextIter *) RVAL2BOXED(v0,
                                                       GTK_TYPE_TEXT_ITER);
    gv1 = NUM2INT(v1);
    ret = gtk_text_view_move_visually(g_self, gv0, gv1);
    return ret ? Qtrue : Qfalse;

}

static VALUE
textview_add_child_at_anchor(self, v0, v1)
    VALUE self, v0, v1;
{
    GtkTextView *g_self;
    GtkWidget *gv0;
    GtkTextChildAnchor *gv1;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_WIDGET,
                                                      GtkWidget);
    gv1 =
        NIL_P(v1) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v1),
                                                      GTK_TYPE_TEXT_CHILD_ANCHOR,
                                                      GtkTextChildAnchor);
    gtk_text_view_add_child_at_anchor(g_self, gv0, gv1);
    return self;

}

static VALUE
textview_add_child_in_window(self, v0, v1, v2, v3)
    VALUE self, v0, v1, v2, v3;
{
    GtkTextView *g_self;
    GtkWidget *gv0;
    GtkTextWindowType gv1;
    gint gv2;
    gint gv3;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_WIDGET,
                                                      GtkWidget);
    gv1 = NUM2UINT(v1);
    gv2 = NUM2INT(v2);
    gv3 = NUM2INT(v3);
    gtk_text_view_add_child_in_window(g_self, gv0, gv1, gv2, gv3);
    return self;

}

static VALUE
textview_move_child(self, v0, v1, v2)
    VALUE self, v0, v1, v2;
{
    GtkTextView *g_self;
    GtkWidget *gv0;
    gint gv1;
    gint gv2;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_WIDGET,
                                                      GtkWidget);
    gv1 = NUM2INT(v1);
    gv2 = NUM2INT(v2);
    gtk_text_view_move_child(g_self, gv0, gv1, gv2);
    return self;

}

static VALUE
textview_get_default_attributes(self)
    VALUE self;
{
    GtkTextView *g_self;
    GtkTextAttributes *ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_default_attributes(g_self);
    return ret ? BOXED2RVAL(ret, GTK_TYPE_TEXT_ATTRIBUTES) : Qnil;

}


static VALUE
textview_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE buffer;
    GtkWidget* o;
    rb_scan_args(argc, argv, "01", &buffer);
    if (NIL_P(buffer))
        o = gtk_text_view_new();
    else
        o = gtk_text_view_new_with_buffer(GTK_TEXT_BUFFER(RVAL2GOBJ(buffer)));
    RBGTK_INITIALIZE(self, o);
    return self;
}


void
Init_gtk_textview()
{
    VALUE cTextView = G_DEF_CLASS(GTK_TYPE_TEXT_VIEW, "TextView", mGtk);
    rb_define_method(cTextView, "set_buffer", textview_set_buffer, 1);
    rb_define_method(cTextView, "get_buffer", textview_get_buffer, 0);
    rb_define_method(cTextView, "scroll_to_iter", textview_scroll_to_iter, 5);
    rb_define_method(cTextView, "scroll_to_mark", textview_scroll_to_mark, 5);
    rb_define_method(cTextView, "scroll_mark_onscreen", textview_scroll_mark_onscreen, 1);
    rb_define_method(cTextView, "move_mark_onscreen", textview_move_mark_onscreen, 1);
    rb_define_method(cTextView, "place_cursor_onscreen", textview_place_cursor_onscreen, 0);
    rb_define_method(cTextView, "get_visible_rect", textview_get_visible_rect, 1);
    rb_define_method(cTextView, "get_iter_location", textview_get_iter_location, 2);
    rb_define_method(cTextView, "get_iter_at_location", textview_get_iter_at_location, 3);
#if 0
    rb_define_method(cTextView, "get_line_yrange", textview_get_line_yrange, 3);
#endif
#if 0
    rb_define_method(cTextView, "get_line_at_y", textview_get_line_at_y, 3);
#endif
#if 0
    rb_define_method(cTextView, "buffer_to_window_coords", textview_buffer_to_window_coords, 5);
#endif
#if 0
    rb_define_method(cTextView, "window_to_buffer_coords", textview_window_to_buffer_coords, 5);
#endif
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
    rb_define_method(cTextView, "move_child", textview_move_child, 3);
    rb_define_method(cTextView, "get_default_attributes", textview_get_default_attributes, 0);
    rb_define_method(cTextView, "initialize", textview_initialize, -1);

    G_DEF_SETTERS(cTextView);
}
