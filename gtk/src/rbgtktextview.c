#include "global.h"

static VALUE
rbgtk_text_view_set_buffer(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    GtkTextBuffer *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(v0),
                                                      GTK_TYPE_TEXT_BUFFER,
                                                      GtkTextBuffer);
    gtk_text_view_set_buffer(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_buffer(VALUE self)
{
    GtkTextView *g_self;
    GtkTextBuffer *ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_buffer(g_self);
    return ret ? GOBJ2RVAL(ret) : Qnil;

}

static VALUE
rbgtk_text_view_scroll_to_iter(VALUE self, VALUE v0, VALUE v1, VALUE v2,
                               VALUE v3, VALUE v4)
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
rbgtk_text_view_scroll_to_mark(VALUE self, VALUE v0, VALUE v1, VALUE v2,
                               VALUE v3, VALUE v4)
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
rbgtk_text_view_scroll_mark_onscreen(VALUE self, VALUE v0)
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
rbgtk_text_view_move_mark_onscreen(VALUE self, VALUE v0)
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
rbgtk_text_view_place_cursor_onscreen(VALUE self)
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
rbgtk_text_view_get_visible_rect(VALUE self, VALUE v0)
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
rbgtk_text_view_set_cursor_visible(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gboolean gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = RTEST(v0);
    gtk_text_view_set_cursor_visible(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_cursor_visible(VALUE self)
{
    GtkTextView *g_self;
    gboolean ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_cursor_visible(g_self);
    return ret ? Qtrue : Qfalse;

}

static VALUE
rbgtk_text_view_get_iter_location(VALUE self, VALUE v0, VALUE v1)
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
rbgtk_text_view_get_iter_at_location(VALUE self, VALUE v0, VALUE v1,
                                     VALUE v2)
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
rbgtk_text_view_get_window(VALUE self, VALUE v0)
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
rbgtk_text_view_get_window_type(VALUE self, VALUE v0)
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
rbgtk_text_view_set_border_window_size(VALUE self, VALUE v0, VALUE v1)
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
rbgtk_text_view_get_border_window_size(VALUE self, VALUE v0)
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
rbgtk_text_view_forward_display_line(VALUE self, VALUE v0)
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
rbgtk_text_view_backward_display_line(VALUE self, VALUE v0)
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
rbgtk_text_view_forward_display_line_end(VALUE self, VALUE v0)
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
rbgtk_text_view_backward_display_line_start(VALUE self, VALUE v0)
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
rbgtk_text_view_starts_display_line(VALUE self, VALUE v0)
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
rbgtk_text_view_move_visually(VALUE self, VALUE v0, VALUE v1)
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
rbgtk_text_view_add_child_at_anchor(VALUE self, VALUE v0, VALUE v1)
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
rbgtk_text_view_add_child_in_window(VALUE self, VALUE v0, VALUE v1,
                                    VALUE v2, VALUE v3)
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
rbgtk_text_view_move_child(VALUE self, VALUE v0, VALUE v1, VALUE v2)
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

#if 0

static VALUE
rbgtk_text_view_set_wrap_mode(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    GtkWrapMode gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2UINT(v0);
    gtk_text_view_set_wrap_mode(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_wrap_mode(VALUE self)
{
    GtkTextView *g_self;
    GtkWrapMode ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_wrap_mode(g_self);
    return UINT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_editable(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gboolean gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = RTEST(v0);
    gtk_text_view_set_editable(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_editable(VALUE self)
{
    GtkTextView *g_self;
    gboolean ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_editable(g_self);
    return ret ? Qtrue : Qfalse;

}

static VALUE
rbgtk_text_view_set_pixels_above_lines(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_pixels_above_lines(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_pixels_above_lines(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_pixels_above_lines(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_pixels_below_lines(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_pixels_below_lines(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_pixels_below_lines(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_pixels_below_lines(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_pixels_inside_wrap(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_pixels_inside_wrap(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_pixels_inside_wrap(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_pixels_inside_wrap(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_justification(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    GtkJustification gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2UINT(v0);
    gtk_text_view_set_justification(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_justification(VALUE self)
{
    GtkTextView *g_self;
    GtkJustification ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_justification(g_self);
    return UINT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_left_margin(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_left_margin(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_left_margin(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_left_margin(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_right_margin(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_right_margin(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_right_margin(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_right_margin(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_indent(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    gint gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 = NUM2INT(v0);
    gtk_text_view_set_indent(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_indent(VALUE self)
{
    GtkTextView *g_self;
    gint ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_indent(g_self);
    return INT2NUM(ret);

}

static VALUE
rbgtk_text_view_set_tabs(VALUE self, VALUE v0)
{
    GtkTextView *g_self;
    PangoTabArray *gv0;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    gv0 =
        NIL_P(v0) ? NULL : (PangoTabArray *) RVAL2BOXED(v0,
                                                         PANGO_TYPE_TAB_ARRAY);
    gtk_text_view_set_tabs(g_self, gv0);
    return self;

}

static VALUE
rbgtk_text_view_get_tabs(VALUE self)
{
    GtkTextView *g_self;
    PangoTabArray *ret;
    g_self =
        G_TYPE_CHECK_INSTANCE_CAST(RVAL2GOBJ(self), GTK_TYPE_TEXT_VIEW,
                                   GtkTextView);
    ret = gtk_text_view_get_tabs(g_self);
    return ret ? BOXED2RVAL(ret, PANGO_TYPE_TAB_ARRAY) : Qnil;

}

#endif

static VALUE
rbgtk_text_view_get_default_attributes(VALUE self)
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
initialize(argc, argv, self)
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
    rb_define_method(cTextView, "set_buffer", rbgtk_text_view_set_buffer,
                     1);
    rb_define_method(cTextView, "get_buffer", rbgtk_text_view_get_buffer,
                     0);
    rb_define_method(cTextView, "scroll_to_iter",
                     rbgtk_text_view_scroll_to_iter, 5);
    rb_define_method(cTextView, "scroll_to_mark",
                     rbgtk_text_view_scroll_to_mark, 5);
    rb_define_method(cTextView, "scroll_mark_onscreen",
                     rbgtk_text_view_scroll_mark_onscreen, 1);
    rb_define_method(cTextView, "move_mark_onscreen",
                     rbgtk_text_view_move_mark_onscreen, 1);
    rb_define_method(cTextView, "place_cursor_onscreen",
                     rbgtk_text_view_place_cursor_onscreen, 0);
    rb_define_method(cTextView, "get_visible_rect",
                     rbgtk_text_view_get_visible_rect, 1);
    rb_define_method(cTextView, "set_cursor_visible",
                     rbgtk_text_view_set_cursor_visible, 1);
    rb_define_method(cTextView, "get_cursor_visible",
                     rbgtk_text_view_get_cursor_visible, 0);
    rb_define_method(cTextView, "get_iter_location",
                     rbgtk_text_view_get_iter_location, 2);
    rb_define_method(cTextView, "get_iter_at_location",
                     rbgtk_text_view_get_iter_at_location, 3);
#if 0
    rb_define_method(cTextView, "get_line_yrange",
                     rbgtk_text_view_get_line_yrange, 3);
#endif
#if 0
    rb_define_method(cTextView, "get_line_at_y",
                     rbgtk_text_view_get_line_at_y, 3);
#endif
#if 0
    rb_define_method(cTextView, "buffer_to_window_coords",
                     rbgtk_text_view_buffer_to_window_coords, 5);
#endif
#if 0
    rb_define_method(cTextView, "window_to_buffer_coords",
                     rbgtk_text_view_window_to_buffer_coords, 5);
#endif
    rb_define_method(cTextView, "get_window", rbgtk_text_view_get_window,
                     1);
    rb_define_method(cTextView, "get_window_type",
                     rbgtk_text_view_get_window_type, 1);
    rb_define_method(cTextView, "set_border_window_size",
                     rbgtk_text_view_set_border_window_size, 2);
    rb_define_method(cTextView, "get_border_window_size",
                     rbgtk_text_view_get_border_window_size, 1);
    rb_define_method(cTextView, "forward_display_line",
                     rbgtk_text_view_forward_display_line, 1);
    rb_define_method(cTextView, "backward_display_line",
                     rbgtk_text_view_backward_display_line, 1);
    rb_define_method(cTextView, "forward_display_line_end",
                     rbgtk_text_view_forward_display_line_end, 1);
    rb_define_method(cTextView, "backward_display_line_start",
                     rbgtk_text_view_backward_display_line_start, 1);
    rb_define_method(cTextView, "starts_display_line",
                     rbgtk_text_view_starts_display_line, 1);
    rb_define_method(cTextView, "move_visually",
                     rbgtk_text_view_move_visually, 2);
    rb_define_method(cTextView, "add_child_at_anchor",
                     rbgtk_text_view_add_child_at_anchor, 2);
    rb_define_method(cTextView, "add_child_in_window",
                     rbgtk_text_view_add_child_in_window, 4);
    rb_define_method(cTextView, "move_child", rbgtk_text_view_move_child,
                     3);
#if 0
    rb_define_method(cTextView, "set_wrap_mode",
                     rbgtk_text_view_set_wrap_mode, 1);
    rb_define_method(cTextView, "get_wrap_mode",
                     rbgtk_text_view_get_wrap_mode, 0);
    rb_define_method(cTextView, "set_editable",
                     rbgtk_text_view_set_editable, 1);
    rb_define_method(cTextView, "get_editable",
                     rbgtk_text_view_get_editable, 0);
    rb_define_method(cTextView, "set_pixels_above_lines",
                     rbgtk_text_view_set_pixels_above_lines, 1);
    rb_define_method(cTextView, "get_pixels_above_lines",
                     rbgtk_text_view_get_pixels_above_lines, 0);
    rb_define_method(cTextView, "set_pixels_below_lines",
                     rbgtk_text_view_set_pixels_below_lines, 1);
    rb_define_method(cTextView, "get_pixels_below_lines",
                     rbgtk_text_view_get_pixels_below_lines, 0);
    rb_define_method(cTextView, "set_pixels_inside_wrap",
                     rbgtk_text_view_set_pixels_inside_wrap, 1);
    rb_define_method(cTextView, "get_pixels_inside_wrap",
                     rbgtk_text_view_get_pixels_inside_wrap, 0);
    rb_define_method(cTextView, "set_justification",
                     rbgtk_text_view_set_justification, 1);
    rb_define_method(cTextView, "get_justification",
                     rbgtk_text_view_get_justification, 0);
    rb_define_method(cTextView, "set_left_margin",
                     rbgtk_text_view_set_left_margin, 1);
    rb_define_method(cTextView, "get_left_margin",
                     rbgtk_text_view_get_left_margin, 0);
    rb_define_method(cTextView, "set_right_margin",
                     rbgtk_text_view_set_right_margin, 1);
    rb_define_method(cTextView, "get_right_margin",
                     rbgtk_text_view_get_right_margin, 0);
    rb_define_method(cTextView, "set_indent", rbgtk_text_view_set_indent,
                     1);
    rb_define_method(cTextView, "get_indent", rbgtk_text_view_get_indent,
                     0);
    rb_define_method(cTextView, "set_tabs", rbgtk_text_view_set_tabs, 1);
    rb_define_method(cTextView, "get_tabs", rbgtk_text_view_get_tabs, 0);
#endif
    rb_define_method(cTextView, "get_default_attributes",
                     rbgtk_text_view_get_default_attributes, 0);

    rb_define_method(cTextView, "initialize", initialize, -1);
}
